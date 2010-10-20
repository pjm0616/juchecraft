
#ifndef NDEBUG
#include <stdio.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <assert.h>

#include "grp.h"


static off_t fdgetfilesize(int fd)
{
	struct stat stbuf;
	fstat(fd, &stbuf);
	return stbuf.st_size;
}

grp_palette_t *load_palette(const char *filename)
{
	int fd = open(filename, O_RDONLY);
	off_t filesize = fdgetfilesize(fd);
	uint32_t *palette_buf = (uint32_t *)malloc(filesize); // FIXME
	
	if(filesize >= 1024)
	{
		memset(palette_buf, 0, 1024);
		read(fd, palette_buf, 1024);
	}
	else if(filesize == 768)
	{
		uint8_t *buf = (uint8_t *)malloc(768);
		memset(buf, 0, 768);
		read(fd, buf, 768);
		for(int i = 0; i < 256; i++)
		{
			memcpy(&palette_buf[i], buf + i*3, 3);
			*(((uint8_t *)&palette_buf[i]) + 3) = 0;
		}
	}
	else
	{
		memset(palette_buf, 0, 1024);
		read(fd, palette_buf, filesize);
	}
	close(fd);
	
	return palette_buf;
}


grp_data_t *load_grp(const char *filename)
{
	int fd = open(filename, O_RDONLY);
	off_t filesize = fdgetfilesize(fd);
	if(filesize < 6)
	{
		close(fd);
		return NULL;
	}
	
	grp_data_t *grpdata = (grp_data_t *)malloc(filesize);
	int nread = read(fd, grpdata, filesize);
	close(fd);
	
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
	
	if(!(flags & USE_INDEX))
	{
		if(flags & SHADOW_COLOR)
			color.u32 = (flags >> 8) & 0x00FFFFFF;
		else
			color.u32 = palette_data[pal_color];
		
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
			row_data = (uint8_t *)((char *)grp_offsets + grp_offsets[y]);
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





