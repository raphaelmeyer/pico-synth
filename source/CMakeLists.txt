cmake_minimum_required(VERSION 3.24)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(PicoSynth)

pico_sdk_init()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(settings INTERFACE)
target_compile_options(settings INTERFACE -Wall -Wextra -Werror)

add_subdirectory(pico-synth)

add_subdirectory(executable)
add_subdirectory(device)
add_subdirectory(vendor)
