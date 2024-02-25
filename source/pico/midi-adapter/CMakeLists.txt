add_executable(midi-adapter)
target_sources(midi-adapter PRIVATE
  main.cc
  usb_descriptors.c
  synth_spi.cc
)

target_link_libraries(midi-adapter PRIVATE
  pico_stdlib
  pico_multicore
  hardware_spi
  tinyusb_device
  tinyusb_board
  synth::message
  synth::midi
  settings
)

target_include_directories(midi-adapter PRIVATE .)

pico_add_extra_outputs(midi-adapter)
