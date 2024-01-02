add_executable(midi-adapter)
target_sources(midi-adapter PRIVATE
  main.cc
  usb_descriptors.c
  synth.cc
)

target_link_libraries(midi-adapter PRIVATE
  pico_stdlib
  pico_multicore
  tinyusb_device
  tinyusb_board
  device::display
  device::io
  library::message
  settings
)

target_include_directories(midi-adapter PRIVATE .)

pico_add_extra_outputs(midi-adapter)
