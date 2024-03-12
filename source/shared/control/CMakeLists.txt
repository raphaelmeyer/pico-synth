add_library(control)
add_library(synth::control ALIAS control)

target_sources(control PRIVATE midi_control.cc)
target_include_directories(control
  INTERFACE
    include
  PRIVATE
    include/synth/control
)

target_link_libraries(control
  PRIVATE
    synth::message
    settings
)
