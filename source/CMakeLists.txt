cmake_minimum_required(VERSION 3.25)

include(cmake/pico-project.cmake)

project(PicoSynth)

setup_project()

include(CTest)

add_subdirectory(library)
add_subdirectory(spec)

if(BUILD_TARGET_PICO)
  add_subdirectory(device)
  add_subdirectory(executable)
  add_subdirectory(vendor)
endif()
