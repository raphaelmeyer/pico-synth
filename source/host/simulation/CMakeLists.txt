add_executable(uisim)

find_package(X11 REQUIRED)

target_sources(uisim
  PRIVATE
    uisim/main.cc
    uisim/x11.cc
)

target_link_libraries(uisim
  PRIVATE
    lvgl::lvgl
    synth::ui
    synth::control
    synth::types
    settings
    X11::X11
)
