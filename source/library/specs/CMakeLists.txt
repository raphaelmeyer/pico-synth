add_executable(library-specs)
target_sources(library-specs PRIVATE
  spec.main.cc
  ../message/message.spec.cc
  ../synth/oscillator.spec.cc
)

target_link_libraries(library-specs PRIVATE
  spec::spec
  library::message
  library::synth
  library::pico-api
)

if(BUILD_TARGET_PICO)
  pico_add_extra_outputs(library-specs)
endif()

add_test(NAME "Library Specs" COMMAND library-specs)
