add_library(display)
add_library(device::display ALIAS display)

target_sources(display PRIVATE display.cc st7735.cc)
target_link_libraries(display
  PUBLIC
    hardware_spi
    lvgl::lvgl
    settings
)

target_include_directories(display PUBLIC .)
