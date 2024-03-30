add_executable(snapshots)
target_sources(snapshots
  PRIVATE
    main.cc
    synth/synth.spec.cc
)

target_link_libraries(snapshots
  PRIVATE
    doctest::doctest
    ApprovalTests::ApprovalTests
    settings
    synth::message
    synth::types
    synth::synth
)

add_test(NAME "Snapshots" COMMAND snapshots)
