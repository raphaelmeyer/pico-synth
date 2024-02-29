add_library(message)
add_library(synth::message ALIAS message)

target_sources(message PRIVATE message.cc)
target_include_directories(message
  INTERFACE
    include
  PRIVATE
    include/synth/message
)

target_link_libraries(message
  PUBLIC
    synth::types
  PRIVATE
    settings
)
