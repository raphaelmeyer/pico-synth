include_guard()

if(DEFINED PICO_BOARD)
  include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "-O2 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

function(setup_project)
  if(DEFINED PICO_BOARD)
    pico_sdk_init()
  endif()

  add_library(settings INTERFACE)
  target_compile_options(settings INTERFACE -Wall -Wextra -Werror)
endfunction()
