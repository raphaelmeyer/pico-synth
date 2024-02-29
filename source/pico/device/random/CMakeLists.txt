add_library(random)
add_library(device::random ALIAS random)

target_sources(random PRIVATE pico-random.cc)
target_link_libraries(random
  PUBLIC
    pico_rand
    synth::types
    settings
)

target_include_directories(random
  INTERFACE
    include
  PRIVATE
    include/synth/device
)