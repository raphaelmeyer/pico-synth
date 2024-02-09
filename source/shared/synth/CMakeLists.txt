add_library(synth)
add_library(synth::synth ALIAS synth)

target_sources(synth
  PRIVATE
    envelope-generator.cc
    oscillator.cc
    tick_counter.cc
)

target_include_directories(synth
  INTERFACE
    include
  PRIVATE
    include/synth/synth
)
