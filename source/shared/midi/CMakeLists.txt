add_library(midi)
add_library(synth::midi ALIAS midi)

target_sources(midi PRIVATE midi_control.cc)
target_include_directories(midi
  INTERFACE
    include
  PRIVATE
    include/synth/midi
)

target_link_libraries(midi
  PRIVATE
    synth::message
    settings
)
