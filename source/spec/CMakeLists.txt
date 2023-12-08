
add_library(spec)
add_library(spec::spec ALIAS spec)

target_sources(spec PRIVATE spec.cc)

target_include_directories(spec INTERFACE .)

target_link_libraries(spec
  PUBLIC
    $<$<PLATFORM_ID:PICO>:pico_stdlib>
    $<$<PLATFORM_ID:PICO>:pico_stdio_usb>
    settings
)
