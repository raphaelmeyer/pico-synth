include(FetchContent)

FetchContent_Declare(
  doctest
  GIT_REPOSITORY  https://github.com/doctest/doctest.git
  GIT_TAG         v2.4.11
)

FetchContent_MakeAvailable(doctest)

FetchContent_Declare(
  ApprovalTests
  GIT_REPOSITORY https://github.com/approvals/ApprovalTests.cpp.git
  GIT_TAG        v.10.13.0
)

FetchContent_MakeAvailable(ApprovalTests)
