#include "spec.h"

#include <cstdio>

namespace {

struct specification {
  std::string name{};
  std::vector<spec::it> examples{};
};

class registry {
public:
  static int verify();
  static void register_spec(specification spec);

  static void fail();

private:
  static registry &instance() {
    static registry instance{};

    return instance;
  }

  static int failures_;

  std::vector<specification> specifications_{};
};

int registry::failures_ = 0;

int registry::verify() {
  failures_ = 0;

  printf("Start...\n");

  for (auto const &spec : instance().specifications_) {
    printf("  %s:\n", spec.name.c_str());

    for (auto const &example : spec.examples) {
      printf("    - %s\n", example.description.c_str());
      example.fn();
    }
  }

  if (failures_ > 0) {
    printf("\n!! %d checks failed.\n", failures_);
  } else {
    printf("\nAll tests passed.\n");
  }

  return failures_ == 0 ? 0 : 1;
}

void registry::register_spec(specification spec) {
  registry::instance().specifications_.push_back(spec);
}

void registry::fail() { ++failures_; }

} // namespace

namespace runner {

extern int verify(std::function<int()> run_verify);

} // namespace runner

namespace spec {

describe::describe(std::string name, std::vector<it> examples) {
  registry::register_spec({name, examples});
}

void fail(std::string message, std::source_location const location) {
  printf("!!      Failed at %s:%u\n", location.file_name(),
         static_cast<unsigned int>(location.line()));
  printf("          %s\n", message.c_str());
  registry::fail();
}

void check(bool condition, std::source_location const location) {
  if (not condition) {
    printf("!!      Failed at %s:%u\n", location.file_name(),
           static_cast<unsigned int>(location.line()));
    registry::fail();
  }
}

int verify() {
  return runner::verify([] { return registry::verify(); });
}

} // namespace spec
