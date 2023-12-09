#include "spec.h"

#include <cstdio>

namespace {

struct specification {
  std::string name{};
  std::vector<spec::it> examples{};
};

class registry {
public:
  static void verify();
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

void registry::verify() {
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
}

void registry::register_spec(specification spec) {
  registry::instance().specifications_.push_back(spec);
}

void registry::fail() { ++failures_; }

} // namespace

namespace runner {

extern void verify(std::function<void()> run_verify);

} // namespace runner

namespace spec {

describe::describe(std::string name, std::vector<it> examples) {
  registry::register_spec({name, examples});
}

void check(bool condition, std::source_location const location) {
  if (not condition) {
    printf("!!      Failed at %s:%u\n", location.file_name(),
           static_cast<unsigned int>(location.line()));
    registry::fail();
  }
}

void verify() {
  runner::verify([] { registry::verify(); });
}

} // namespace spec
