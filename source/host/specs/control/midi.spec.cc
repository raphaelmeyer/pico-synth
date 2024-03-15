#include <doctest/doctest.h>

#include <synth/control/midi_control.h>
#include <synth/message/message.h>

#include <stdexcept>
#include <variant>
#include <vector>

namespace {

class MessageSpy : public Sender {
public:
  void send(Word data) override { data_.push_back(data); }

  std::vector<Message> decode() const {
    std::vector<Message> messages{};
    Decoder decoder{data_};

    while (decoder.it != decoder.end) {
      messages.push_back(receive_message(decoder));
    }
    return messages;
  }

private:
  std::vector<Word> data_{};

  struct Decoder : public Receiver {
    Decoder(std::vector<Word> const &data)
        : it{data.cbegin()}, end{data.cend()} {}

    Word receive() override {
      if (it != end) {
        auto result = *it;
        ++it;
        return result;
      }
      throw std::logic_error{"message queue empty"};
    }

    std::vector<Word>::const_iterator it;
    std::vector<Word>::const_iterator end;
  };
};

TEST_CASE("Play a note") {
  uint8_t const note = 60;
  uint16_t const frequency = 1046; // 4 * 261.63

  MessageSpy spy{};
  MidiControl midi{spy};

  midi.handle({0, 0x90, note, 0});
  auto const messages = spy.decode();

  REQUIRE(messages.size() == 2);

  auto const set_frequency = std::get_if<SetFrequency>(&messages.at(0).command);
  REQUIRE(set_frequency != nullptr);
  REQUIRE(set_frequency->frequency == frequency);
  REQUIRE(std::holds_alternative<Trigger>(messages.at(1).command));
}

TEST_CASE("Oscillator frequency is a 16bit value with two fractional bits") {
  struct Note {
    uint8_t midi;
    uint16_t frequency;
  };

  std::vector<Note> notes{
      {0, 32},      // C-1
      {17, 87},     // F0
      {34, 233},    // A♯1, B♭1
      {51, 622},    // D♯3, E♭3
      {68, 1661},   // G♯4, A♭4
      {85, 4434},   // C♯6, D♭6
      {102, 11839}, // F♯7, G♭7
      {119, 31608}, // B8
      {127, 50175}  // G9
  };

  for (auto const &note : notes) {
    MessageSpy spy{};
    MidiControl midi{spy};

    midi.handle({0, 0x90, note.midi, 0});
    auto const messages = spy.decode();

    REQUIRE(messages.size() == 2);

    auto const set_frequency =
        std::get_if<SetFrequency>(&messages.at(0).command);
    REQUIRE(set_frequency != nullptr);
    REQUIRE(set_frequency->frequency == note.frequency);
  }
}

TEST_CASE("Stop playing a note") {
  MessageSpy spy{};
  MidiControl midi{spy};

  midi.handle({0, 0x80, 0, 0});
  auto const messages = spy.decode();

  REQUIRE(messages.size() == 1);
  REQUIRE(std::holds_alternative<Release>(messages.at(0).command));
}

} // namespace
