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
   virtual std::string Debug() = 0;
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
  virtual std::string Debug();
 private:
  static const std::string symbol;
};

class Print : public Action {
 public:
  Print(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Debug();
 private:
  std::string print;
  static const std::string symbol;
};

class PrintValue : public Action {
 public:
  PrintValue(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Debug();
 private:
  AffectedValue aff;
  static const std::string symbol;
};

class Decrement : public Action {
 public:
  Decrement(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Debug();
 private:
  AffectedValue aff;
  static const std::string symbol;
};

class Increment : public Action {
 public:
  Increment(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Debug();
 private:
  AffectedValue aff;
  static const std::string symbol;
};

class Assign : public Action {
 public:
  Assign(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Debug();
 private:
  uint64_t new_val;
  AffectedValue aff;
  static const std::string symbol;
};

class SquigglyMoveTo : public Action {
 public:
  SquigglyMoveTo(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Debug();
 private:
  static const std::string symbol;
};








class Substract : public Action {
 public:
  Substract(uint64_t v) : value(v) {}
  virtual void Do(ProgramState* state) {
    state->Accumulator() -= value;
  }
  uint64_t value;
  AffectedValue aff;
  static const std::string symbol;
};

class Add : public Action {
 public:
  Add(uint64_t v) : value(v) {}
  virtual void Do(ProgramState* state) {
    state->Accumulator() += value;
  } 
  uint64_t value;
  AffectedValue aff;
  static const std::string symbol;
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
  static const std::string symbol;
};



class ActionBuilder {
 public:
  static ActionBuilder& Instance();
  Action* BuildAction(std::string action);
  void Register(
      std::string,
      std::string,
      ActionFactory*);
  std::string ActionName(std::string symbol);
 protected:
  ActionBuilder() { }
 private:
  std::pair<std::string, std::string> ConsumeAction(std::string action);
  std::set<std::string> symbols;
  std::map<std::string, ActionFactory*> action_factories;
  std::map<std::string, std::string> action_names;
};


#endif
