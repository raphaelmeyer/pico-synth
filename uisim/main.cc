#include "ui/wave_form_selection.h"
#include "x11.h"

#include "ui/ui.h"

#include <lvgl.h>

#include <chrono>
#include <iostream>
#include <thread>

int main() {
  lv_init();

  // lv_group_t *g = lv_group_create();
  // lv_group_set_default(g);

  // lv_disp_t *disp = NULL;

  lv_x11_init("LVGL Simulator", DISP_HOR_RES, DISP_VER_RES);

  /*Create a display buffer*/
  lv_disp_draw_buf_t disp_buf1{};
  lv_color_t buf1_1[DISP_HOR_RES * 100];
  lv_color_t buf1_2[DISP_HOR_RES * 100];
  lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, DISP_HOR_RES * 100);

  /*Create a display*/
  lv_disp_drv_t disp_drv{};
  lv_disp_drv_init(&disp_drv);
  disp_drv.draw_buf = &disp_buf1;
  disp_drv.flush_cb = lv_x11_flush;
  disp_drv.hor_res = DISP_HOR_RES;
  disp_drv.ver_res = DISP_VER_RES;
  disp_drv.antialiasing = 1;

  // disp = lv_disp_drv_register(&disp_drv);
  lv_disp_drv_register(&disp_drv);

  bool running = true;
  std::thread tick{[&running] {
    while (running) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      lv_tick_inc(5);
    }
  }};

  UI ui{};
  ui.show();

  ui.select_oscillator(2);
  ui.select_wave_form(WaveForm::Noise);

  for (;;) {
    lv_timer_handler();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  running = false;

  tick.join();
  lv_x11_deinit();
}
