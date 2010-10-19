

enum
{
	// Causes the graphic to be drawn in one color
	// Second byte of flags is the red component of
	// the shadow's color, third byte is green,
	// fourth byte is blue; like this:
	// SHADOW_COLOR|0xBBGGRR00
	// This can be accomplished by using the left shift
	// operator like this:  SHADOW_COLOR|(color << 8)
	SHADOW_COLOR = 1, 
	
	// Blends the graphic with what it is being drawn over.
	// The dwAlpha parameter will only be used when this
	// flag is specified.  dwAlpha is an RGB value
	// (0xBBGGRR).
	// Note: Because of the extra calculations required,
	// alpha blended graphics take longer to draw
	ALPHA_BLEND = 2, 
	
	// Only valid when used with a custom SetPixel function.
	// This flag cannot be used in combination with
	// ALPHA_BLEND or SHADOW_COLOR
	// When this flag is used, the index to a color in the
	// palette will be passed to your custom SetPixel
	// function instead of the actual color.
	USE_INDEX = 4, 
	
	// Flips the graphic horizontally
	HORIZONTAL_FLIP = 8, 
	// Flips the graphic vertically
	VERTICAL_FLIP = 16, 
};



typedef struct grp_header
{
	uint16_t nframes;
	uint16_t max_width;
	uint16_t max_height;
} grp_header_t;


typedef struct grp_frameheader
{
	uint8_t left;
	uint8_t top;
	uint8_t width;
	uint8_t height;
	uint32_t offset;
} grp_frameheader_t;

typedef struct grp_framedata
{
	uint16_t *row_offsets;
	uint16_t *row_sizes;
	uint8_t **row_data;
	uint32_t size;
} grp_framedata_t;

typedef struct grp_pixel_funcs
{
	void (*set_pixel)(void *dest, int x, int y, uint32_t color);
	uint32_t (*get_pixel)(void *dest, int x, int y);
} grp_pixel_funcs_t;



typedef uint32_t grp_palette_t;
typedef uint8_t grp_data_t;


grp_palette_t *load_palette(const char *filename);
grp_data_t *load_grp(const char *filename);

bool draw_grp(void *dest, int dest_x, int dest_y, grp_pixel_funcs_t *pixelfuncs, 
				grp_data_t *grpdata, grp_palette_t *palette_data, 
				uint16_t framenum, uint32_t flags, uint32_t alpha);
grp_header_t *get_grp_info(grp_data_t *grpdata);
grp_frameheader_t *grp_get_frame_info(grp_data_t *grpdata, short framenum);













