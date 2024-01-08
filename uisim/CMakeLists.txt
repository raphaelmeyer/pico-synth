cmake_minimum_required(VERSION 3.28)

project(UISimulator)

find_package(X11 REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(uisim)

target_sources(uisim PRIVATE
  main.cc
  x11.c

  ../source/executable/midi-adapter/ui/ui.cc
  ../source/executable/midi-adapter/ui/oscillator_selection.cc
  ../source/executable/midi-adapter/ui/range_input.cc
  ../source/executable/midi-adapter/ui/wave_form_selection.cc

  ../source/executable/midi-adapter/icons/img_noise_wave.c
  ../source/executable/midi-adapter/icons/img_sawtooth_wave.c
  ../source/executable/midi-adapter/icons/img_square_wave.c
  ../source/executable/midi-adapter/icons/img_triangle_wave.c
)

target_link_libraries(uisim PRIVATE
  settings
  lvgl::lvgl
  X11::X11
)

target_include_directories(uisim PRIVATE
  ../source/executable/midi-adapter
)

add_library(settings INTERFACE)
target_compile_options(settings INTERFACE -Wall -Wextra -Werror)

include(FetchContent)

set(LV_CONF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h CACHE STRING "" FORCE)

FetchContent_Declare(
  lvgl
  GIT_REPOSITORY  https://github.com/lvgl/lvgl.git
  GIT_TAG         v8.3.9
)

FetchContent_MakeAvailable(lvgl)
