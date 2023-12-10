include_guard()

if(BUILD_TARGET_PICO)
  include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(setup_project)

  if(BUILD_TARGET_PICO)
    pico_sdk_init()
  endif()

  add_library(settings INTERFACE)
  target_compile_options(settings INTERFACE -Wall -Wextra -Werror)

endfunction()
