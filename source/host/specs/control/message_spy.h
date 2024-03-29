#pragma once

#include <synth/message/message.h>

#include <stdexcept>

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
