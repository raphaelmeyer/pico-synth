#pragma once

class InputField {
public:
  virtual void focus() = 0;
  virtual void blur() = 0;

  virtual void edit() = 0;
  virtual void confirm() = 0;
};
