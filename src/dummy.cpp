#include "libpng/png.h"

#include "lz4/lz4.h"

void PNGAPI
png_destroy_write_struct(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr) {};
void PNGAPI
png_write_rows(png_structrp png_ptr, png_bytepp row,
               png_uint_32 num_rows) {};
void PNGAPI
png_set_expand_gray_1_2_4_to_8(png_structrp png_ptr) {};
jmp_buf *PNGAPI
png_set_longjmp_fn(png_structrp png_ptr, png_longjmp_ptr longjmp_fn,
                   size_t jmp_buf_size)
{
    return nullptr;
};
void PNGAPI
png_set_gray_to_rgb(png_structrp png_ptr) {};
void PNGAPI
png_set_bgr(png_structrp png_ptr) {};
png_structp png_create_write_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn)
{
    return nullptr;
};
void png_write_info(png_struct *png_ptr,
                    const png_info *info_ptr) {};
void PNGAPI
png_init_io(png_structrp png_ptr, png_FILE_p fp) {};
int LZ4_decompress_fast(const char *source, char *dest, int originalSize)
{
    return 0;
};
void png_set_palette_to_rgb(png_struct *png_ptr) {};
void png_set_strip_16(png_struct *png_ptr) {};
void png_set_tRNS_to_alpha(png_struct *png_ptr) {};
void PNGAPI png_read_info(png_structrp png_ptr, png_inforp info_ptr) {};
void PNGAPI
png_set_sig_bytes(png_structrp png_ptr, int num_bytes) {};
void PNGAPI
png_destroy_read_struct(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr,
                        png_infopp end_info_ptr_ptr) {};
png_uint_32 PNGAPI
png_get_valid(png_const_structrp png_ptr, png_const_inforp info_ptr,
              png_uint_32 flag)
{
    return 0;
};
void PNGAPI
png_set_IHDR(png_const_structrp png_ptr, png_inforp info_ptr,
             png_uint_32 width, png_uint_32 height, int bit_depth,
             int color_type, int interlace_type, int compression_type,
             int filter_type) {};
void PNGAPI
png_read_end(png_structrp png_ptr, png_inforp info_ptr) {};
void png_read_rows(png_struct *png_ptr, png_bytepp row, png_bytepp display_row, png_uint_32 num_rows) {};
png_structp png_create_read_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn)
{
    return nullptr;
};
png_infop png_create_info_struct(const png_struct *png_ptr)
{
    return nullptr;
};
png_uint_32 PNGAPI png_get_IHDR(png_const_structrp png_ptr, png_const_inforp info_ptr,
                                png_uint_32 *width, png_uint_32 *height, int *bit_depth,
                                int *color_type, int *interlace_type, int *compression_type,
                                int *filter_type)
{
    return 0;
};
void PNGAPI
png_set_filler(png_structrp png_ptr, png_uint_32 filler, int filler_loc) {}
void PNGAPI
png_write_end(png_structrp png_ptr, png_inforp info_ptr) {};