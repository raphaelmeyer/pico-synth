add_library(random)
add_library(device::random ALIAS random)

target_sources(random PRIVATE pico-random.cc)
target_link_libraries(random
  PUBLIC
    pico-api
    pico_rand
    settings
)

target_include_directories(random PUBLIC .)
