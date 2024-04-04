#include <ApprovalTests.hpp>
#include <doctest/doctest.h>

#include <synth/device/random.h>
#include <synth/synth/oscillator.h>

#include <algorithm>

namespace {

class FakeRandom : public Random {
public:
  uint32_t value() override {
    ++value_;
    value_ %= 16;
    return value_ * 15000;
  }

private:
  uint32_t value_{};
};

TEST_CASE("square") {
  FakeRandom random{};
  Oscillator oscillator{48'000, random};

  oscillator.set_type(WaveForm::Square);
  oscillator.set_frequency(22222);

  std::vector<uint16_t> values{};
  values.resize(500);
  std::ranges::generate(values,
                        [&oscillator] { return oscillator.next_value(); });

  ApprovalTests::Approvals::verifyAll(values);
}

TEST_CASE("triangle") {
  FakeRandom random{};
  Oscillator oscillator{48'000, random};

  oscillator.set_type(WaveForm::Triangle);
  oscillator.set_frequency(1234);

  std::vector<uint16_t> values{};
  values.resize(500);
  std::ranges::generate(values,
                        [&oscillator] { return oscillator.next_value(); });

  ApprovalTests::Approvals::verifyAll(values);
}

TEST_CASE("sawtooth") {
  FakeRandom random{};
  Oscillator oscillator{48'000, random};

  oscillator.set_type(WaveForm::Sawtooth);
  oscillator.set_frequency(8765);

  std::vector<uint16_t> values{};
  values.resize(500);
  std::ranges::generate(values,
                        [&oscillator] { return oscillator.next_value(); });

  ApprovalTests::Approvals::verifyAll(values);
}

TEST_CASE("noise") {
  FakeRandom random{};
  Oscillator oscillator{48'000, random};

  oscillator.set_type(WaveForm::Noise);
  oscillator.set_frequency(1760);

  std::vector<uint16_t> values{};
  values.resize(500);
  std::ranges::generate(values,
                        [&oscillator] { return oscillator.next_value(); });

  ApprovalTests::Approvals::verifyAll(values);
}

TEST_CASE("change frequency") {
  FakeRandom random{};
  Oscillator oscillator{48'000, random};

  oscillator.set_type(WaveForm::Triangle);
  std::vector<uint16_t> values{};
  values.resize(4000);

  oscillator.set_frequency(5000);
  std::ranges::generate_n(values.begin(), 1000,
                          [&oscillator] { return oscillator.next_value(); });

  oscillator.set_frequency(2000);
  std::ranges::generate_n(values.begin() + 1000, 1000,
                          [&oscillator] { return oscillator.next_value(); });

  oscillator.set_frequency(12345);
  std::ranges::generate_n(values.begin() + 2000, 1000,
                          [&oscillator] { return oscillator.next_value(); });

  oscillator.set_frequency(8800);
  std::ranges::generate(values.begin() + 3000, values.end(),
                        [&oscillator] { return oscillator.next_value(); });

  ApprovalTests::Approvals::verifyAll(values);
}

TEST_CASE("change wave form") {
  FakeRandom random{};
  Oscillator oscillator{48'000, random};

  oscillator.set_frequency(8800);
  std::vector<uint16_t> values{};
  values.resize(4000);

  oscillator.set_type(WaveForm::Square);
  std::ranges::generate_n(values.begin(), 1000,
                          [&oscillator] { return oscillator.next_value(); });

  oscillator.set_type(WaveForm::Triangle);
  std::ranges::generate_n(values.begin() + 1000, 1000,
                          [&oscillator] { return oscillator.next_value(); });

  oscillator.set_type(WaveForm::Noise);
  std::ranges::generate_n(values.begin() + 2000, 1000,
                          [&oscillator] { return oscillator.next_value(); });

  oscillator.set_type(WaveForm::Sawtooth);
  std::ranges::generate(values.begin() + 3000, values.end(),
                        [&oscillator] { return oscillator.next_value(); });

  ApprovalTests::Approvals::verifyAll(values);
}

} // namespace
