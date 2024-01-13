add_executable(midi-adapter)
target_sources(midi-adapter PRIVATE
  main.cc
  usb_descriptors.c
  controller.cc
  knob.cc
  selector.cc
  synth.cc

  ui/ui.cc
  ui/oscillator_selection.cc
  ui/range_input.cc
  ui/wave_form_selection.cc

  icons/img_noise_wave.c
  icons/img_sawtooth_wave.c
  icons/img_square_wave.c
  icons/img_triangle_wave.c
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
