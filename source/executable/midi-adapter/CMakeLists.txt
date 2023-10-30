add_executable(midi-adapter)
target_sources(midi-adapter PRIVATE
  main.cc
  usb_descriptors.c
)

target_link_libraries(midi-adapter PRIVATE
  pico_stdlib
  tinyusb_device
  tinyusb_board
  device::display
  settings
)

target_include_directories(midi-adapter PRIVATE .)

pico_add_extra_outputs(midi-adapter)
