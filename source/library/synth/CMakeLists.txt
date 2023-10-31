add_library(synth)
add_library(library::synth ALIAS synth)

target_sources(synth PRIVATE
  oscillator.cc
  envelope-generator.cc
)

target_link_libraries(synth PRIVATE
  device::i2s
  pico_rand
  settings
)

target_include_directories(synth PUBLIC .)
