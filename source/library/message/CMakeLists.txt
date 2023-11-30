add_library(message)
add_library(library::message ALIAS message)

target_sources(message PRIVATE
  message.cc
)

target_link_libraries(message PRIVATE
  settings
)

target_include_directories(message PUBLIC .)
