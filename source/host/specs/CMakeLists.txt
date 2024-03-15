add_executable(specs)
target_sources(specs
  PRIVATE
    main.cc
    control/focus.spec.cc
    control/midi.spec.cc
    message/message.spec.cc
)

target_link_libraries(specs
  PRIVATE
    settings
    doctest::doctest
    synth::control
    synth::message
)

add_test(NAME "Specs" COMMAND specs)
