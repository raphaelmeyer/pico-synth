#include <functional>

namespace runner {

void verify(std::function<void()> run_verify) { run_verify(); }

} // namespace runner
