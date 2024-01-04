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

#ifndef LV_ATTRIBUTE_IMG_IMG_SAWTOOTH_WAVE
#define LV_ATTRIBUTE_IMG_IMG_SAWTOOTH_WAVE
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_IMG_SAWTOOTH_WAVE uint8_t img_sawtooth_wave_map[] = {
  0x00, 0x07, 
  0x00, 0x0f, 
  0x00, 0x3b, 
  0x00, 0x73, 
  0x00, 0xc3, 
  0x03, 0x83, 
  0x07, 0x03, 
  0x0c, 0x03, 
  0x38, 0x03, 
  0x70, 0x03, 
  0xc0, 0x03, 
  0x80, 0x03, 
};

const lv_img_dsc_t img_sawtooth_wave = {
  .header.cf = LV_IMG_CF_ALPHA_1BIT,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 16,
  .header.h = 12,
  .data_size = 24,
  .data = img_sawtooth_wave_map,
};
