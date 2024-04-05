add_library(ui)
add_library(synth::ui ALIAS ui)

target_sources(ui
  PRIVATE
    ui.cc
)

target_include_directories(ui
  INTERFACE
    include
  PRIVATE
    include/synth/ui
)

target_link_libraries(ui
  PRIVATE
    lvgl::lvgl
    synth::control
    synth::types
    settings
)
