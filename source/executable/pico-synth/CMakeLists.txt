add_executable(pico-synth)
target_sources(pico-synth PRIVATE
  main.cc
  tone-generator.cc
  demo.cc
)

target_link_libraries(pico-synth PRIVATE
  device::i2s
  library::synth
  pico_multicore
  pico_rand
  pico_stdlib
  settings
)

pico_add_extra_outputs(pico-synth)
