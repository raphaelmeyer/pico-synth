#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_IMG_TRIANGLE_WAVE
#define LV_ATTRIBUTE_IMG_IMG_TRIANGLE_WAVE
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_IMG_TRIANGLE_WAVE uint8_t img_triangle_wave_map[] = {
  0x01, 0x80, 
  0x03, 0xc0, 
  0x02, 0x40, 
  0x06, 0x60, 
  0x0c, 0x30, 
  0x0c, 0x30, 
  0x18, 0x18, 
  0x10, 0x08, 
  0x30, 0x0c, 
  0x60, 0x06, 
  0x60, 0x06, 
  0xc0, 0x03, 
};

const lv_img_dsc_t img_triangle_wave = {
  .header.cf = LV_IMG_CF_ALPHA_1BIT,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 16,
  .header.h = 12,
  .data_size = 24,
  .data = img_triangle_wave_map,
};
