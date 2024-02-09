#pragma once

class Source;

class EnvelopeGenerator {
public:
  EnvelopeGenerator(Source &source);

private:
  Source &source_;
};
