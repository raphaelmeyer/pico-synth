add_executable(pico-synth)
target_sources(pico-synth PRIVATE
  main.cc
  control.cc
  demo.cc
  mixer.cc
  tone-generator.cc
)

target_link_libraries(pico-synth PRIVATE
  device::i2s
  device::random
  library::synth
  library::message
  pico_multicore
  pico_rand
  pico_stdlib
  hardware_spi
  settings
)

pico_add_extra_outputs(pico-synth)
