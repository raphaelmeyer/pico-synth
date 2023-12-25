#include "hw_config.h"

#include "synth.h"

#include <display.h>

#include <lvgl.h>

#include <bsp/board.h>
#include <tusb.h>

#include <hardware/gpio.h>
#include <pico/binary_info.h>
#include <pico/multicore.h>

namespace {

Config const config{

    .display = {.clock = 18,
                .mosi = 19,
                .chip_select = 17,
                .data_command = 20,
                .reset = 21,
                .spi = spi0},

    .synth =
        {.mosi = 15, .miso = 12, .clock = 14, .chip_select = 13, .spi = spi1},

};

Synth synth{config.synth};

lv::Display display{config.display};

void midi_task() {
  while (tud_midi_available()) {
    std::array<uint8_t, 4> packet{};
    tud_midi_packet_read(packet.data());

    multicore_fifo_push_blocking(packet[0] | packet[1] << 8 | packet[2] << 16 |
                                 packet[3] << 24);
  }
}

void second_core_entry() {
  for (;;) {
    auto const packet = multicore_fifo_pop_blocking();
    synth.handle(packet);
  }
}

} // namespace

int main() {
  bi_decl(bi_3pins_with_func(17, 18, 19, GPIO_FUNC_SPI));
  bi_decl(bi_2pins_with_names(20, "D/C", 21, "RST"));
  bi_decl(bi_4pins_with_func(12, 13, 14, 15, GPIO_FUNC_SPI));
  bi_decl(bi_1pin_with_name(22, "LED"));

  board_init();
  lv_init();

  tud_init(BOARD_TUD_RHPORT);

  display.initialize();
  synth.init();

  lv_style_t style{};
  lv_style_init(&style);
  lv_style_set_bg_color(&style, lv_color_hex(0x000000));
  lv_style_set_text_color(&style, lv_color_hex(0x80C0FF));
  lv_obj_add_style(lv_scr_act(), &style, 0);

  lv_obj_t *title = lv_label_create(lv_scr_act());
  lv_label_set_text(title, "Pico Synth\nMidi Adapter");
  lv_obj_set_width(title, 128);
  lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

  multicore_launch_core1(second_core_entry);

  while (1) {
    tud_task();
    midi_task();
  }
}
