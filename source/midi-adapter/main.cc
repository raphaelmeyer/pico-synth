#include "display.h"

#include <bsp/board.h>
#include <src/misc/lv_area.h>
#include <src/misc/lv_printf.h>
#include <tusb.h>

#include <lvgl.h>

namespace {

Config const config{

    .display = {.clock = 18,
                .mosi = 19,
                .chip_select = 17,
                .data_command = 20,
                .reset = 21,
                .spi = spi0}

};

lv::Display display{config.display};

lv_obj_t *note = nullptr;

void midi_task() {

  while (tud_midi_available()) {
    uint8_t packet[4]{};
    tud_midi_packet_read(packet); // [cable|code index][midi][midi?][midi?]

    if ((packet[1] & 0xf0) == 0x90) {
      lv_label_set_text_fmt(note, "On %d, vel %d", static_cast<int>(packet[2]),
                            static_cast<int>(packet[3]));
    } else if ((packet[1] & 0xf0) == 0x80) {
      lv_label_set_text_fmt(note, "Off %d", static_cast<int>(packet[2]));
    } else {
      lv_label_set_text_fmt(note, "%02x %02x %02x %02x", packet[0], packet[1],
                            packet[2], packet[3]);
    }
  }
}

} // namespace

int main() {
  board_init();
  lv_init();

  tud_init(BOARD_TUD_RHPORT);

  display.initialize();

  lv_obj_t *label1 = lv_label_create(lv_scr_act());
  lv_label_set_text(label1, "Pico Synth Midi Adapter");
  lv_obj_set_width(label1, 128);
  lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(label1, LV_ALIGN_TOP_MID, 0, 0);

  note = lv_label_create(lv_scr_act());
  lv_obj_set_width(note, 128);
  lv_label_set_text(note, "");
  lv_obj_set_style_text_align(note, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(note, LV_ALIGN_TOP_MID, 0, 40);

  lv_label_set_text(note, "start");

  while (1) {
    tud_task();
    midi_task();
  }
}
