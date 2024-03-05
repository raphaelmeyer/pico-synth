add_executable(specs)
target_sources(specs
  PRIVATE
    main.cc
    message/message.spec.cc
    midi/midi.spec.cc
)

target_link_libraries(specs
  PRIVATE
    settings
    doctest::doctest
    synth::message
    synth::midi
)

add_test(NAME "Specs" COMMAND specs)
