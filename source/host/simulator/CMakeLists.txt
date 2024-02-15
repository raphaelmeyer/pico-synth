add_executable(simulator)

target_sources(simulator PRIVATE main.cc)

target_link_libraries(simulator
  PRIVATE
    settings
    synth::synth
    synth::types
)