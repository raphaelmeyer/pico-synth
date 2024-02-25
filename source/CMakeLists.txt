cmake_minimum_required(VERSION 3.25)

include(cmake/pico-project.cmake)

project(PicoSynth)

setup_project()

include(CTest)

add_subdirectory(shared)

if(DEFINED PICO_BOARD)
  add_subdirectory(pico)
else()
  add_subdirectory(host)
endif()
