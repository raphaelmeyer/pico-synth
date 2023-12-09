add_executable(library-spec)
target_sources(library-spec PRIVATE
  spec.main.cc
  ../message/message.spec.cc
  ../synth/oscillator.spec.cc
)

target_link_libraries(library-spec PRIVATE
  spec::spec
  library::message
  library::synth
  library::pico-api
)

if(BUILD_TARGET_PICO)
  pico_add_extra_outputs(library-spec)
endif()
