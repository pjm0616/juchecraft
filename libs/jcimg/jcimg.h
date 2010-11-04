// Copyright (C) 2010 Park Jeongmin <pjm0616@gmail.com>
// See LICENSE.txt for details

#ifndef JUCHEIMAGE_H_
#define JUCHEIMAGE_H_

// define JCIMG_SAVE_SUPPORT if you need jc_save_image
//#define JCIMG_SAVE_SUPPORT




/*
	File format:
	{
		{
			u16 magic;
			u16 version;
			
			u32 checksum;
			
			u8 compression_type;
			u8 padding[1];
			u16 nimages;
			
			u16 max_width;
			u16 max_height;
			
			u32 img_index[nimages]; // img_index[] = {0, sizeof(next_item), ...};
		}
	///////////////////////// compressed: from here _ ->
		img_index[0 ... nimages-1] = &{
			u16 left;
			u16 top;
			u16 width;
			u16 height;
			
			u8 pixels[width * height * 4];
		}
	///////////////////////// <- compressed: to here ^
	}
	
 */

#define JCIMG_MAGIC 0xf32e
#define JCIMG_VERSION 0x0000

typedef struct jcimg_imginfo_s
{
	int32_t left;
	int32_t top;
	int32_t width;
	int32_t height;
} jcimg_imginfo_t;

typedef struct jcimg_img_s
{
	jcimg_imginfo_t info;
	char pixels[0];
} jcimg_img_t;

typedef struct jcimg_info_s
{
	uint16_t nimages;
	uint16_t max_width;
	uint16_t max_height;
} jcimg_info_t;

#ifdef JCIMG_INTERNAL
typedef struct jcimg_hdr_s
{
	uint16_t magic;
	uint16_t version;
	uint32_t checksum;
	uint8_t compression_type;
	uint8_t padding[1];
	
	jcimg_info_t info;
	
	uint32_t img_index[0]; // array of pointer to jcimg_img_t
} jcimg_hdr_t;
#endif



class JucheImage
{
public:
	JucheImage();
	JucheImage(const jcimg_info_t *fileinfo);
	JucheImage(const char *filename, uint32_t sf_flags = SDL_SWSURFACE);
	~JucheImage();
	
	void clear();
	void reset(const jcimg_info_t *fileinfo);
	
	void insertNewImage(jcimg_imginfo_t *imginfo, SDL::SDL_SurfaceSPtr_t sf);
	void insertNewImage(jcimg_imginfo_t *imginfo, uint32_t *pixels, uint32_t sf_flags = SDL_SWSURFACE);
	
	bool load(const char *filename, uint32_t sf_flags = SDL_SWSURFACE);
#ifdef JCIMG_SAVE_SUPPORT
	bool save(const char *filename);
#endif
	
	const SDL::SDL_SurfaceSPtr_t &getImage(unsigned int n, jcimg_imginfo_t **imginfo = NULL);
	
	size_t numOfImages() const { return this->m_images.size(); }
	size_t getMaxWidth() const { return this->m_info.max_width; }
	size_t getMaxHeight() const { return this->m_info.max_height; }
	
private:
	void setFileInfo(const jcimg_info_t *fileinfo); // only called by JucheImage::reset()
	
	void insertNewImage(jcimg_img_t *img, uint32_t sf_flags = SDL_SWSURFACE);
	
	class ImageInfo
	{
	public:
		ImageInfo(const jcimg_imginfo_t *imginfo) { this->set(imginfo); }
		ImageInfo(const ImageInfo &o) { this->set(o.get()); }
		~ImageInfo(){}
		
		void set(const jcimg_imginfo_t *imginfo);
		jcimg_imginfo_t *get() { return &this->m_imginfo; }
		const jcimg_imginfo_t *get() const { return &this->m_imginfo; }
	private:
		jcimg_imginfo_t m_imginfo;
	};
	
	jcimg_info_t m_info;
	std::vector<std::pair<JucheImage::ImageInfo, SDL::SDL_SurfaceSPtr_t> > m_images;
};




#endif /* JUCHEIMAGE_H_ */

