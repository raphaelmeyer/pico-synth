add_library(io)
add_library(device::io ALIAS io)

target_sources(io PRIVATE
  gpio_interrupt_handler.cc
  push_button.cc
  rotary_encoder.cc
)

target_link_libraries(io
  PUBLIC
    hardware_gpio
    settings
)

target_include_directories(io PUBLIC .)
