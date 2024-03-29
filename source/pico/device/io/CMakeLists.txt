add_library(io)
add_library(device::io ALIAS io)

target_sources(io
  PRIVATE
    gpio_irq.cc
)
target_link_libraries(io
  PUBLIC
    hardware_gpio
    settings
)

target_include_directories(io
  INTERFACE
    include
  PRIVATE
    include/synth/io
)
