add_executable(pico-synth)
target_sources(pico-synth PRIVATE
  main.cc
  adapter_spi.cc
)

target_link_libraries(pico-synth PRIVATE
  pico_stdlib
  pico_multicore
  hardware_spi
  synth::message
  settings
)

target_include_directories(pico-synth PRIVATE .)

pico_add_extra_outputs(pico-synth)
