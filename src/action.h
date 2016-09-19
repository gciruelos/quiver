#ifndef ACTION_H
#define ACTION_H

#include <cstdint>
#include <string>
#include <iostream>

#include "state.h"

class Action {
  public:
   virtual void Do(ProgramState* state) = 0;
};


class Print : public Action {
 public:
  Print(std::string print) : print_(print) {}
  virtual void Do(ProgramState* state __attribute__((unused))) {
    std::cout << print_;
  }
 private:
  std::string print_;
};

class PrintAccumulator : public Action {
 public:
  virtual void Do(ProgramState* state) {
    std::cout << state->GetAccumulator();
  }
};

class Substract : public Action {
 public:
  Substract(uint64_t value) : value_(value) {}
  virtual void Do(ProgramState* state) {
    state->GetAccumulator() -= value_;
  }
 private:
  uint64_t value_;
};

class Add : public Action {
 public:
  Add(uint64_t value) : value_(value) {}
  virtual void Do(ProgramState* state) {
    state->GetAccumulator() += value_;
  } 
 private:
  uint64_t value_;
};

class Decrement : public Action {
 public:
  Decrement() {}
  virtual void Do(ProgramState* state) {
    state->GetAccumulator()--;
  }
};

class Assign : public Action {
 public:
  Assign(uint64_t new_val) : new_val_(new_val) {}
  virtual void Do(ProgramState* state) {
    state->GetAccumulator() = new_val_;
  }
 private:
  uint64_t new_val_;
};

class LoadAccumulator : public Action {
 public:
  virtual void Do(ProgramState* state) {
    state->GetAccumulator() = state->GetNodeValue();
  }
};

class SaveAccumulator : public Action {
 public:
  virtual void Do(ProgramState* state) {
    state->GetNodeValue() = state->GetAccumulator();
  }
};

class Nothing : public Action {
 public:
  Nothing() {}
  virtual void Do(ProgramState* state __attribute__((unused))) {
    return;
  }
};

Action* BuildAction(std::string action);

#endif
