#pragma once

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

void check(bool condition, std::source_location const location =
                               std::source_location::current());

void verify();

} // namespace spec
