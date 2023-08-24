add_executable(pico-synth)
target_sources(pico-synth PRIVATE
  main.cc
  i2s.cc
  oscillator.cc
)

pico_generate_pio_header(pico-synth ${CMAKE_CURRENT_SOURCE_DIR}/pio/i2s.pio)

target_link_libraries(pico-synth PRIVATE
  pico_stdlib
  hardware_pio
  settings
)

pico_add_extra_outputs(pico-synth)
