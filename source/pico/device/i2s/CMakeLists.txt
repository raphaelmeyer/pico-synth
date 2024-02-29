add_library(i2s)
add_library(device::i2s ALIAS i2s)

target_sources(i2s PRIVATE i2s.cc)
target_link_libraries(i2s
  PUBLIC
    hardware_pio
    hardware_clocks
    settings
)

target_include_directories(i2s
  INTERFACE
    include
  PRIVATE
    include/synth/device
)

pico_generate_pio_header(i2s ${CMAKE_CURRENT_SOURCE_DIR}/i2s.pio)
