add_library(synth)
add_library(synth::synth ALIAS synth)

target_sources(synth
  PRIVATE
    envelope-generator.cc
    oscillator.cc
    tick-counter.cc
    wave-generation.cc
)

target_link_libraries(synth
  PRIVATE
    settings
    synth::types
)

target_include_directories(synth
  INTERFACE
    include
  PRIVATE
    include/synth/synth
)
