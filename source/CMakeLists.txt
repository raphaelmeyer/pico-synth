cmake_minimum_required(VERSION 3.24)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(PicoSynth)

pico_sdk_init()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(pico-synth)
target_sources(pico-synth PRIVATE
  main.cc
  i2s.cc
  oscillator.cc
)

pico_generate_pio_header(pico-synth ${CMAKE_CURRENT_SOURCE_DIR}/pio/i2s.pio)

target_link_libraries(pico-synth PRIVATE
  pico_stdlib
  hardware_pio
  settings
)

pico_add_extra_outputs(pico-synth)

add_library(settings INTERFACE)
target_compile_options(settings INTERFACE -Wall -Wextra -Werror -Wno-ignored-qualifiers)
