add_executable(simulator)

target_sources(simulator PRIVATE main.cc)

target_link_libraries(simulator
  PRIVATE
    settings
    synth::synth
    synth::types
)

add_executable(synth_output)
target_sources(synth_output PRIVATE synth.cc)
target_link_libraries(synth_output PRIVATE synth::synth synth::message)
