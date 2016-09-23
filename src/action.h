#ifndef ACTION_H
#define ACTION_H

#include <cstdint>
#include <string>
#include <iostream>
#include <memory>
#include <map>

#include "state.h"
#include "utils.h"

class Action {
  public:
   virtual void Do(ProgramState* state) = 0;
};

class ActionFactory {
 public:
  virtual Action* Create(std::pair<std::string, std::string>) = 0;
};
  
typedef std::pair<std::string, std::string> ParsedAction;



class Print : public Action {
 public:
  Print(ParsedAction s) : print_(ParseString(s.second)) {}
  virtual void Do(ProgramState* state __attribute__((unused))) {
    std::cout << print_;
  }
 private:
  std::string print_;
};

class Nothing : public Action {
 public:
  Nothing(ParsedAction s __attribute__((unused))) {}
  virtual void Do(ProgramState* state __attribute__((unused))) {
    return;
  }
};


class PrintAccumulator : public Action {
 public:
  PrintAccumulator(ParsedAction s __attribute__((unused))) {}
  virtual void Do(ProgramState* state __attribute__((unused))) {
     std::cout << state->Accumulator();
  }
};


class Decrement : public Action {
 public:
  Decrement(ParsedAction s __attribute__((unused))) {}
  virtual void Do(ProgramState* state) {
    state->Accumulator()--;
  }
};

class Assign : public Action {
 public:
  Assign(ParsedAction s) : new_val_(std::stoi(s.second)) {}
  virtual void Do(ProgramState* state) {
    state->Accumulator() = new_val_;
  }
 private:
  uint64_t new_val_;
};


class Substract : public Action {
 public:
  Substract(uint64_t value) : value_(value) {}
  virtual void Do(ProgramState* state) {
    state->Accumulator() -= value_;
  }
 private:
  uint64_t value_;
};

class Add : public Action {
 public:
  Add(uint64_t value) : value_(value) {}
  virtual void Do(ProgramState* state) {
    state->Accumulator() += value_;
  } 
 private:
  uint64_t value_;
};


class Increment : public Action {
 public:
  Increment() {}
  virtual void Do(ProgramState* state) {
    state->Accumulator()++;
  }
};

class MoveTo : public Action {
 public:
  MoveTo(std::pair<std::string, std::string> s){
    if (s.first == s.second) {
      test = 0;
    } else {
      test = 1;
    }
  }
  virtual void Do(ProgramState* state) {
    state->CurrentNodeValue() = state->NodeValue(state->LastNode());
  }
 private:
  int test;
};



class ActionBuilder {
 public:
  static ActionBuilder& Instance();
  Action* BuildAction(std::string action);
  void Register(
      std::string,
      std::string,
      ActionFactory*);
 protected:
  ActionBuilder() { }
 private:
  std::pair<std::string, std::string> ConsumeAction(std::string action);
  std::set<std::string> symbols;
  std::map<std::string, ActionFactory*> action_factories;
  std::map<std::string, std::string> action_names;
};


#endif
