#include <functional>

namespace runner {

int verify(std::function<int()> run_verify) { return run_verify(); }

} // namespace runner
