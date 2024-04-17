add_library(ui)
add_library(synth::ui ALIAS ui)

target_sources(ui
  PRIVATE
    ui.cc
    oscillator_selection.cc
    range_input.cc
    wave_form_selection.cc

    icons/img_noise_wave.c
    icons/img_sawtooth_wave.c
    icons/img_square_wave.c
    icons/img_triangle_wave.c
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
