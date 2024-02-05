add_executable(specs)
target_sources(specs PRIVATE main.cc)

target_link_libraries(specs
  PRIVATE
    settings
    doctest::doctest
)
