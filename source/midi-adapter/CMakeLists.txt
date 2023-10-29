add_executable(midi-adapter)
target_sources(midi-adapter PRIVATE
  main.cc
  usb_descriptors.c
  display.cc
  st7735.cc
)

target_link_libraries(midi-adapter PRIVATE
  pico_stdlib
  hardware_spi
  tinyusb_device
  tinyusb_board
  lvgl::lvgl
  settings
)

target_include_directories(midi-adapter PRIVATE .)

pico_add_extra_outputs(midi-adapter)
