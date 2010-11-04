// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#include "config.h"

#include <vector>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <inttypes.h>
#include <cassert>


namespace zlib
{
#include <zlib.h>
}

#include "smart_ptrs.h"
#include "defs.h"
#include "compat.h"

#include "etc/md5.h"
#include "my_sdl.h"

#define JCIMG_INTERNAL
#include "jcimg.h"





static char *my_z_compress(const char *data, size_t size, size_t *result_size_out)
{
	zlib::uLongf result_size;
	char *destbuf = new char[size + 4];
	
	result_size = size;
	int ret = zlib::compress2((zlib::Bytef *)(destbuf + 4), &result_size, (zlib::Bytef *)data, size, 9);
	if(ret != Z_OK)
	{
		delete[] destbuf;
		return NULL;
	}
	
	*(uint32_t *)destbuf = size ^ 0xdeadbeef;
	
	if(result_size_out)
		*result_size_out = result_size + 4;
	
	return destbuf;
}

static char *my_z_uncompress(const char *data, size_t size, size_t *result_size_out)
{
	zlib::uLongf result_size;
	result_size = *(uint32_t *)data ^ 0xdeadbeef;
	char *destbuf = new char[result_size];
	
	int ret = zlib::uncompress((zlib::Bytef *)destbuf, &result_size, (zlib::Bytef *)(data+4), size-4);
	if(ret != Z_OK)
	{
		delete[] destbuf;
		return NULL;
	}
	
	if(result_size_out)
		*result_size_out = result_size;
	
	return destbuf;
}

static char *my_readfile(const char *file, size_t *filesize_out)
{
	FILE *fp = fopen(file, "rb");
	if(!fp)
		return NULL;
	
	fseek(fp, 0, SEEK_END);
	size_t filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	char *buf = new char[filesize];
	size_t nread = fread(buf, filesize, 1, fp);
	fclose(fp);
	
	if(nread != 1)
	{
		assert(!"failed to read data. 0a93mu24dnmud");
	}
	
	if(filesize_out)
		*filesize_out = filesize;
	
	return buf;
}


void JucheImage::ImageInfo::set(const jcimg_imginfo_t *imginfo)
{
	memcpy(&this->m_imginfo, imginfo, sizeof(jcimg_imginfo_t));
}

JucheImage::JucheImage()
{
	this->clear();
}

JucheImage::JucheImage(const jcimg_info_t *fileinfo)
{
	this->reset(fileinfo);
}

JucheImage::JucheImage(const char *filename, uint32_t sf_flags)
{
	this->load(filename, sf_flags);
}

JucheImage::~JucheImage()
{
}

void JucheImage::clear()
{
	this->m_images.clear();
	this->m_info.nimages = 0;
}

void JucheImage::reset(const jcimg_info_t *fileinfo)
{
	this->setFileInfo(fileinfo);
	this->clear();
}

void JucheImage::setFileInfo(const jcimg_info_t *fileinfo)
{
	memcpy(&this->m_info, fileinfo, sizeof(this->m_info));
}

void JucheImage::insertNewImage(jcimg_imginfo_t *imginfo, SDL::SDL_SurfaceSPtr_t sf)
{
	this->m_images.push_back(
		std::pair<JucheImage::ImageInfo, SDL::SDL_SurfaceSPtr_t>(
			JucheImage::ImageInfo(imginfo), sf
		));
	this->m_info.nimages = this->numOfImages();
}

void JucheImage::insertNewImage(jcimg_imginfo_t *imginfo, uint32_t *pixels, uint32_t sf_flags)
{
	SDL::SDL_Surface *sf = SDL::SDL_CreateRGBSurface(sf_flags, this->m_info.max_width, this->m_info.max_height, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
	size_t pixels_size = sf->w * sf->h * 4;
	memcpy(sf->pixels, pixels, pixels_size);
	
	this->insertNewImage(imginfo, SDL::SDL_SurfaceSPtr_t(sf));
}

void JucheImage::insertNewImage(jcimg_img_t *img, uint32_t sf_flags)
{
	this->insertNewImage(&img->info, (uint32_t *)&img->pixels, sf_flags);
}

bool JucheImage::load(const char *filename, uint32_t sf_flags)
{
	size_t filesize;
	const char *buf = my_readfile(filename, &filesize);
	if(!buf)
		return NULL;
	
	jcimg_hdr_t *hdr = (jcimg_hdr_t *)buf;
	this->reset(&hdr->info);
	
	size_t index_size = (sizeof(uint32_t) * hdr->info.nimages);
	const char *compressed_imgdata = buf + sizeof(jcimg_hdr_t) + index_size;
	size_t compressed_imgdata_size = filesize - (sizeof(jcimg_hdr_t) + index_size);
	size_t imgdata_size;
	const char *imgdata = my_z_uncompress(compressed_imgdata, compressed_imgdata_size, &imgdata_size);
	
	for(int i = 0; i < hdr->info.nimages; i++)
	{
		size_t pos = hdr->img_index[i] ^ 0xbeefdead;
		jcimg_img_t *imghdr = (jcimg_img_t *)(imgdata + pos);
		this->insertNewImage(imghdr, sf_flags);
	}
	delete[] imgdata;
	delete[] buf;
	
	return true;
}

#ifdef JCIMG_SAVE_SUPPORT
bool JucheImage::save(const char *filename)
{
	assert(this->m_info.nimages == this->m_images.size());
	
	// prepare header
	jcimg_hdr_t header;
	header.magic = JCIMG_MAGIC;
	header.version = JCIMG_VERSION;
	header.compression_type = 1; // zlib
	header.padding[0] = 0;
	memcpy(&header.info, &this->m_info, sizeof(jcimg_info_t));
	
	// calculate data size
	size_t pos = 0;
	for(int i = 0; i < this->m_info.nimages; i++)
	{
		const SDL::SDL_SurfaceSPtr_t &sf = this->m_images[i].second;
		size_t pixels_size = sf->w * sf->h * 4;
		pos += sizeof(jcimg_imginfo_t) + pixels_size;
	}
	size_t data_size = pos;
	
	// allocate buffer for jcimg_hdr_t::image_ptrs
	size_t index_size = sizeof(uint32_t) * this->m_info.nimages;
	uint32_t *img_index = new uint32_t[this->m_info.nimages];
	
	// prepare data, calculate checksum, and build image index
	char *buf = new char[data_size];
	pos = 0;
	uint32_t checksum = 0;
	for(int i = 0; i < this->m_info.nimages; i++)
	{
		const jcimg_imginfo_t *info = this->m_images[i].first.get();
		const SDL::SDL_SurfaceSPtr_t &sf = this->m_images[i].second;
		size_t pixels_size = sf->w * sf->h * 4;
		
		img_index[i] = pos ^ 0xbeefdead;
		
		memcpy(&buf[pos], info, sizeof(*info));
		pos += sizeof(*info);
		memcpy(&buf[pos], sf->pixels, pixels_size);
		pos += pixels_size;
		
		checksum ^= md5_simple32((const char *)info, sizeof(*info));
		checksum ^= md5_simple32((const char *)sf->pixels, pixels_size);
	}
	header.checksum = checksum;
	
	// compress data
	size_t imgdata_compressed_size;
	const char *imgdata_compressed = my_z_compress(buf, data_size, &imgdata_compressed_size);
	delete[] buf;
	
	// write header
	FILE *fp = fopen(filename, "w");
	fwrite(&header, sizeof(header), 1, fp);
	// write index
	fwrite(img_index, index_size, 1, fp);
	delete[] img_index;
	// write data
	fwrite(imgdata_compressed, imgdata_compressed_size, 1, fp);
	delete[] imgdata_compressed;
	fclose(fp);
	
	return true;
}
#endif

const SDL::SDL_SurfaceSPtr_t &JucheImage::getImage(unsigned int n, jcimg_imginfo_t **imginfo)
{
	static SDL::SDL_SurfaceSPtr_t null_sf;
	if(unlikely(n > this->numOfImages()))
		return null_sf;
	
	std::pair<JucheImage::ImageInfo, SDL::SDL_SurfaceSPtr_t> &img = this->m_images[n];
	
	if(imginfo)
		*imginfo = img.first.get();
	return img.second;
}






