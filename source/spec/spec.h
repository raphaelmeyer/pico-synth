#pragma once

#include <concepts>
#include <functional>
#include <source_location>
#include <string>
#include <vector>

namespace spec {

struct it {
  std::string description;
  std::function<void()> fn;
};

class describe {
public:
  describe(std::string name, std::vector<it> examples);
};

void fail(std::string message, std::source_location const location =
                                   std::source_location::current());

void check(bool condition, std::source_location const location =
                               std::source_location::current());

template <std::equality_comparable T> class matcher {
public:
  matcher(T actual) : actual_{actual} {}

  void
  to_be(T expected,
        std::source_location const location = std::source_location::current())
    requires std::integral<T>
  {
    if (actual_ != expected) {
      fail("actual: " + std::to_string(actual_) +
               ", expected: " + std::to_string(expected),
           location);
    }
  }

private:
  T actual_;
};

template <std::equality_comparable T> matcher<T> expect(T actual) {
  return {actual};
}

int verify();

} // namespace spec
