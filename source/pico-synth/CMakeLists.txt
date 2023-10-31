add_executable(pico-synth)
target_sources(pico-synth PRIVATE
  main.cc
)

target_link_libraries(pico-synth PRIVATE
  device::i2s
  library::synth
  pico_stdlib
  pico_rand
  settings
)

pico_add_extra_outputs(pico-synth)
