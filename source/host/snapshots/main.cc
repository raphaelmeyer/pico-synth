#define APPROVALS_DOCTEST
#include <ApprovalTests/ApprovalTests.hpp>

auto directoryDisposer =
    ApprovalTests::Approvals::useApprovalsSubdirectory("snapshots");
