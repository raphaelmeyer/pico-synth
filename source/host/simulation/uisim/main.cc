#include "x11.h"

#include <synth/control/control.h>
#include <synth/control/focus.h>
#include <synth/control/model.h>
#include <synth/ui/ui.h>

#include <lvgl.h>

#include <chrono>
#include <thread>

namespace {

class Display {
public:
  Display() {
    lv_init();

    lv_x11_init("UI simulator", DISP_HOR_RES, DISP_VER_RES);

    /*Create a display buffer*/
    lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, DISP_HOR_RES * 100);

    /*Create a display*/
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf1;
    disp_drv.flush_cb = lv_x11_flush;
    disp_drv.hor_res = DISP_HOR_RES;
    disp_drv.ver_res = DISP_VER_RES;
    disp_drv.antialiasing = 1;

    lv_disp_drv_register(&disp_drv);

    running_ = true;
    tick_ = std::thread{[this] {
      while (running_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        lv_tick_inc(5);
      }
    }};
  }

  ~Display() {
    running_ = false;
    tick_.join();
    lv_x11_deinit();
  }

  void run() {
    while (running_) {
      lv_timer_handler();
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  }

  void stop() { running_ = false; }

private:
  std::thread tick_;
  bool running_{false};

  lv_disp_drv_t disp_drv{};
  lv_disp_draw_buf_t disp_buf1{};
  lv_color_t buf1_1[DISP_HOR_RES * 100];
  lv_color_t buf1_2[DISP_HOR_RES * 100];
};

class InputHandler {
public:
  InputHandler(Control &control, Display &display)
      : control_{control}, display_{display} {
    timer_ = lv_timer_create(dispatch_, 100, this);
  }

  ~InputHandler() {
    if (timer_) {
      lv_timer_del(timer_);
    }
  }

private:
  static void dispatch_(lv_timer_t *timer) {
    if (timer) {
      auto self = static_cast<InputHandler *>(timer->user_data);
      self->handle_();
    }
  }

  void handle_() {
    lv_indev_data_t keyboard{};
    lv_x11_get_keyboard(nullptr, &keyboard);
    while (keyboard.state == LV_INDEV_STATE_PRESSED) {
      switch (keyboard.key) {
      default:
        break;

      case 27:
      case 'q':
        display_.stop();
        break;

      case '<':
        control_.handle(Rotate{-1});
        break;

      case '>':
        control_.handle(Rotate{1});
        break;

      case 13:
      case ' ':
        control_.handle(Click{});
        break;
      }

      lv_x11_get_keyboard(nullptr, &keyboard);
    }

    lv_indev_data_t mouse{};
    lv_x11_get_mousewheel(nullptr, &mouse);
    if (mouse.enc_diff != 0) {
      control_.handle(Rotate{mouse.enc_diff});
    }

    lv_indev_data_t pointer{};
    lv_x11_get_pointer(nullptr, &pointer);
    if (prev_ == LV_INDEV_STATE_PRESSED &&
        pointer.state == LV_INDEV_STATE_RELEASED) {
      control_.handle(Click{});
    }
    prev_ = pointer.state;
  }

  Control &control_;
  Display &display_;
  lv_timer_t *timer_{nullptr};
  lv_indev_state_t prev_{};
};

} // namespace

int main() {
  Display display{};

  Model model{};
  Focus focus{};
  UI ui{model, focus};

  Control control{model, focus};
  InputHandler input_handler{control, display};

  control.onEvent([&ui](ControlEvent event) { ui.handle(event); });
  ui.show();
  display.run();
}
