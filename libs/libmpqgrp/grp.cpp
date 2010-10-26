
#include "config.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <inttypes.h>

#include "grp.h"

#ifdef ENABLE_MPQ
#include "../libmpq/SFmpqapi/SFmpqapi.h"

static FILE *mpq_fopen(const char *filename, const char *mode)
{
	HANDLE hFile;
	if(SFileOpenFileEx(0, filename, 1, &hFile) == false)
		return (FILE *)NULL;
	return (FILE *)hFile;
}
static long mpq_fgetfilesize(FILE *fp)
{
	return SFileGetFileSize((HANDLE)fp, 0);
}
static size_t mpq_fread(void *ptr, size_t size, size_t nmemb, FILE *fp)
{
	if(SFileReadFile((HANDLE)fp, ptr, size, 0, 0) == false)
		return 0;
	return nmemb;
}
static int mpq_fseek(FILE *fp, long off, int whence)
{
	return SFileSetFilePointer((HANDLE)fp, 0, 0, whence);
}
static int mpq_fclose(FILE *fp)
{
	SFileCloseFile((HANDLE)fp);
	return 0;
}



#endif

static long fgetfilesize(FILE *fp)
{
	fseek(fp, 0, SEEK_END);
	long filesize = ftell(fp);
	rewind(fp);
	return filesize;
}



static FILE *(*my_fopen)(const char *filename, const char *mode) = fopen;
static long (*my_fgetfilesize)(FILE *fp) = fgetfilesize;
static size_t (*my_fread)(void *ptr, size_t size, size_t nmemb, FILE *fp) = fread;
static int (*my_fseek)(FILE *fp, long off, int whence) = fseek;
static int (*my_fclose)(FILE *fp) = fclose;


void grp_set_file_method(unsigned int v)
{
	if(v == GRP_USE_MPQ)
	{
		my_fopen = mpq_fopen;
		my_fgetfilesize = mpq_fgetfilesize;
		my_fread = mpq_fread;
		my_fseek = mpq_fseek;
		my_fclose = mpq_fclose;
	}
	else
	{
		my_fopen = fopen;
		my_fgetfilesize = fgetfilesize;
		my_fread = fread;
		my_fseek = fseek;
		my_fclose = fclose;
	}
}


grp_palette_t *load_palette(const char *filename)
{
	FILE *fp = my_fopen(filename, "rb");
	if(!fp)
		return NULL;
	
	long filesize = my_fgetfilesize(fp);
	// filesize is maximum 1024 bytes
	if(filesize > 1024)
		filesize = 1024;
	
	uint32_t *palette_buf;
	if(filesize == 768)
	{
		palette_buf = (uint32_t *)calloc(1, 1024);
		uint8_t buf[768] = {0, };
		int nread = my_fread(buf, 768, 1, fp);
		for(int i = 0; i < 768/3; i++)
		{
			uint8_t *p = (uint8_t *)&palette_buf[i];
			memcpy(p, &buf[i * 3], 3);
			p[3] = 0;
		}
	}
	else
	{
		palette_buf = (uint32_t *)calloc(1, filesize);
		my_fread(palette_buf, filesize, 1, fp);
	}
	my_fclose(fp);
	
	return palette_buf;
}

grp_data_t *load_grp(const char *filename)
{
	FILE *fp = my_fopen(filename, "rb");
	if(!fp)
		return NULL;
	
	long filesize = my_fgetfilesize(fp);
	if(filesize < 6)
	{
		fclose(fp);
		return NULL;
	}
	
	grp_data_t *grpdata = (grp_data_t *)malloc(filesize);
	int nread = my_fread(grpdata, filesize, 1, fp);
	my_fclose(fp);
	
	return grpdata;
}



static void grp_set_pixel(grp_pixel_funcs_t *pixelfuncs, void *dest, int x, int y, uint8_t pal_color, grp_palette_t *palette_data, unsigned int flags, unsigned int alphaval)
{
	union
	{
		uint32_t u32;
		uint8_t u8[4];
	} color, alpha, backcolor;
	alpha.u32 = alphaval;
	
#if 0
	if(!(flags & USE_INDEX))
#endif
	{
		if(flags & SHADOW_COLOR)
			color.u32 = (flags >> 8) & 0x00FFFFFF;
		else
			color.u32 = palette_data[pal_color];
#if 0
		if(flags & ALPHA_BLEND)
		{
			assert(!"Alpha blending in libgrp is disabled");
			backcolor.u32 = pixelfuncs->get_pixel(dest, x, y);
			
			for(int i = 0; i < 3; i++)
			{
				color.u8[i] = ((color.u8[i] * alpha.u8[i] + 1) >> 8)
								+ ((backcolor.u8[i] * (256 - alpha.u8[i])) >> 8);
			}
		}
#endif
	}
	
	pixelfuncs->set_pixel(dest, x, y, color.u32);
}


bool draw_grp(void *dest, int dest_x, int dest_y, grp_pixel_funcs_t *pixelfuncs, grp_data_t *grpdata, grp_palette_t *palette_data, uint16_t framenum, uint32_t flags, uint32_t alpha)
{
	assert(grpdata != NULL);
	if(!palette_data && (flags & USE_INDEX))
		return false;
	
	grp_header_t *grphdr = (grp_header_t *)grpdata;
	framenum %= grphdr->nframes;
	grp_frameheader_t *grp_frames = (grp_frameheader_t *)(((char *)grpdata)+6);
	grp_frameheader_t *grp_frame = &grp_frames[framenum];
	unsigned int frame_size = 0xffffff;
	//unsigned int dest_xright = dest_x + (grphdr->max_width-1 - grp_frame->left);
	//unsigned int dest_ybottom = dest_y + (grphdr->max_height-1 - grp_frame->top);
	
	dest_x += grp_frame->left;
	dest_y += grp_frame->top;
	uint16_t *grp_offsets = (uint16_t *)( (char *)grpdata + grp_frame->offset );
	uint8_t *grp_raw = (uint8_t *)grp_offsets;
	uint8_t *row_data;
	unsigned short x, ofs, i;
	unsigned int y, max_offset = 0; 
	int j, first_nonblank_frame = 0;
	
	for(i = 0; i < grphdr->nframes; i++)
	{
		j = grp_frames[i].offset - grp_frame->offset;
		if(j > 0 && (unsigned int)j < frame_size)
			frame_size = j;
		if(grp_frames[i].offset > max_offset)
			max_offset = grp_frames[i].offset;
	}
	
	if(frame_size == 0xffffff || frame_size == (grp_frame->width * grp_frame->height))
	{
		frame_size = 0xffffff;
		for(i = 0; i < grphdr->nframes; i++)
		{
			if(grp_frames[i].width != 0 
				&& grp_frames[i].height != 0 
				&& grp_frames[i].offset != grp_frame->offset 
				&& grp_frames[i].offset != max_offset)
			{
				first_nonblank_frame = i;
				break;
			}
		}
		
		if(i == grphdr->nframes)
			first_nonblank_frame = framenum;
		
		for(i = 0; i < grphdr->nframes; i++)
		{
			j = grp_frames[i].offset - grp_frames[first_nonblank_frame].offset;
			if(j > 0 && (unsigned int)j < frame_size)
				frame_size = j;
		}
		if(frame_size == grp_frames[first_nonblank_frame].width * grp_frames[first_nonblank_frame].height)
			frame_size = grp_frame->width * grp_frame->height;
	}
	
	if(frame_size != grp_frame->width * grp_frame->height)
	{
		for(y = 0; y < grp_frame->height; y++)
		{
			uint16_t offs_add = grp_offsets[y];
			row_data = (uint8_t *)((char *)grp_offsets + offs_add);
			
			x = 0; ofs = 0;
			while(x < grp_frame->width)
			{
				if(!(row_data[ofs] & 0x80))
				{
					if(!(row_data[ofs] & 0x40))
					{
						for(i = 1; i <= row_data[ofs] && x < grp_frame->width; i++)
						{
							grp_set_pixel(pixelfuncs, dest, dest_x + x, dest_y + y, row_data[ofs + i], palette_data, flags, alpha);
							x++;
						}
						ofs += row_data[ofs] + 1;
					}
					else
					{
						for(i = 0; i < row_data[ofs] - 64 && x < grp_frame->width; i++)
						{
							grp_set_pixel(pixelfuncs, dest, dest_x + x, dest_y + y, row_data[ofs + 1], palette_data, flags, alpha);
							x++;
						}
						ofs += 2;
					}
				}
				else
				{
					x += row_data[ofs] - 128;
					ofs++;
				}
			}
		}
	}
	else
	{
		for(y = 0; y < grp_frame->height; y++)
		{
			for(x = 0; x < grp_frame->width; x++)
			{
				grp_set_pixel(pixelfuncs, dest, dest_x + x, dest_y + y, grp_raw[y * grp_frame->width + x], palette_data, flags, alpha);
			}
		}
	}

	return true;
}




grp_header_t *get_grp_info(grp_data_t *grpdata)
{
	return (grp_header_t *)grpdata;
}

grp_frameheader_t *grp_get_frame_info(grp_data_t *grpdata, short framenum)
{
	grp_frameheader_t *frames = (grp_frameheader_t *)((char *)grpdata + 6);
	return &frames[framenum];
}





