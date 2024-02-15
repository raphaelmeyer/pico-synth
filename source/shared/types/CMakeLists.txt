add_library(types INTERFACE)
add_library(synth::types ALIAS types)

target_include_directories(types
  INTERFACE
    include
)

target_link_libraries(types
  INTERFACE
    settings
)
