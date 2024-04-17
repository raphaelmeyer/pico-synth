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

#ifndef LV_ATTRIBUTE_IMG_IMG_NOISE_WAVE
#define LV_ATTRIBUTE_IMG_IMG_NOISE_WAVE
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_IMG_NOISE_WAVE uint8_t img_noise_wave_map[] = {
  0x00, 0x40, 
  0x00, 0x40, 
  0x00, 0x68, 
  0x21, 0x68, 
  0x23, 0x58, 
  0x65, 0x98, 
  0x64, 0x95, 
  0x98, 0x96, 
  0x18, 0x96, 
  0x18, 0x84, 
  0x10, 0x00, 
  0x10, 0x00, 
};

const lv_img_dsc_t img_noise_wave = {
  .header.cf = LV_IMG_CF_ALPHA_1BIT,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 16,
  .header.h = 12,
  .data_size = 24,
  .data = img_noise_wave_map,
};
