add_library(pico-api INTERFACE)
add_library(library::pico-api ALIAS pico-api)

target_link_libraries(pico-api INTERFACE settings)
target_include_directories(pico-api INTERFACE .)
