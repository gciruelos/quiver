#ifndef ACTION_H
#define ACTION_H

#include <cstdint>
#include <string>
#include <iostream>
#include <memory>
#include <map>

#include "state.h"
#include "utils.h"

enum AffectedValue {
  ACCUMULATOR = 1,
  CURRENT_NODE,
  NEXT_NODE
};

class Action {
  public:
   virtual void Do(ProgramState* state) = 0;
};

class ActionFactory {
 public:
  virtual Action* Create(std::pair<std::string, std::string>) = 0;
};
  
typedef std::pair<std::string, std::string> ParsedAction;


class Nothing : public Action {
 public:
  Nothing(ParsedAction);
  virtual void Do(ProgramState*);
};

class Print : public Action {
 public:
  Print(ParsedAction);
  virtual void Do(ProgramState*);
  std::string print;
};

class PrintValue : public Action {
 public:
  PrintValue(ParsedAction);
  virtual void Do(ProgramState*);
  AffectedValue aff;
};


class Decrement : public Action {
 public:
  Decrement(ParsedAction);
  virtual void Do(ProgramState*);
  AffectedValue aff;
};

class Assign : public Action {
 public:
  Assign(ParsedAction);
  virtual void Do(ProgramState*);
  uint64_t new_val;
  AffectedValue aff;
};


class Substract : public Action {
 public:
  Substract(uint64_t v) : value(v) {}
  virtual void Do(ProgramState* state) {
    state->Accumulator() -= value;
  }
  uint64_t value;
  AffectedValue aff;
};

class Add : public Action {
 public:
  Add(uint64_t v) : value(v) {}
  virtual void Do(ProgramState* state) {
    state->Accumulator() += value;
  } 
  uint64_t value;
  AffectedValue aff;
};


class Increment : public Action {
 public:
  Increment() {}
  virtual void Do(ProgramState* state) {
    state->Accumulator()++;
  }
  AffectedValue aff;
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
