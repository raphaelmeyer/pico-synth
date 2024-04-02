#define APPROVALS_DOCTEST
#include <ApprovalTests/ApprovalTests.hpp>

auto directoryDisposer =
    ApprovalTests::Approvals::useApprovalsSubdirectory("snapshots");

auto defaultReporterDisposer = ApprovalTests::Approvals::useAsDefaultReporter(
    std::make_shared<ApprovalTests::QuietReporter>());
