#ifndef ACTION_H
#define ACTION_H

#include <cstdint>
#include <string>
#include <iostream>
#include <memory>
#include <map>
#include <set>
#include <utility>

#include "state.h"
#include "utils.h"

enum AffectedValue {
  ACCUMULATOR = 1,
  CURRENT_NODE,
  NEXT_NODE
};

typedef std::pair<std::string, std::string> ParsedAction;

class ActionVisitor;

class Action {
 public:
  virtual void Do(ProgramState* state) = 0;
  virtual std::string Accept(ActionVisitor* visitor) = 0;
};

class ActionFactory {
 public:
  virtual Action* Create(ParsedAction) = 0;
};


class Nothing : public Action {
 public:
  explicit Nothing(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Accept(ActionVisitor*);

  static const std::string symbol;
};

class Print : public Action {
 public:
  explicit Print(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Accept(ActionVisitor*);

  std::string print;
  static const std::string symbol;
};

class PrintValue : public Action {
 public:
  explicit PrintValue(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Accept(ActionVisitor*);

  AffectedValue aff;
  static const std::string symbol;
};

class Decrement : public Action {
 public:
  explicit Decrement(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Accept(ActionVisitor*);

  AffectedValue aff;
  static const std::string symbol;
};

class Increment : public Action {
 public:
  explicit Increment(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Accept(ActionVisitor*);

  AffectedValue aff;
  static const std::string symbol;
};

class Assign : public Action {
 public:
  explicit Assign(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Accept(ActionVisitor*);

  uint64_t new_val;
  AffectedValue aff;
  static const std::string symbol;
};

class SquigglyMoveTo : public Action {
 public:
  explicit SquigglyMoveTo(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Accept(ActionVisitor*);

  static const std::string symbol;
};

class Add : public Action {
 public:
  explicit Add(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Accept(ActionVisitor*);

  uint64_t value;
  bool arg_is_value;
  AffectedValue aff;
  static const std::string symbol;
};

class Multiply : public Action {
 public:
  explicit Multiply(ParsedAction);
  virtual void Do(ProgramState*);
  virtual std::string Accept(ActionVisitor*);

  uint64_t value;
  bool arg_is_value;
  AffectedValue aff;
  static const std::string symbol;
};

class ActionVisitor {
 public:
  virtual std::string VisitNothing(Nothing*) = 0;
  virtual std::string VisitPrint(Print*) = 0;
  virtual std::string VisitPrintValue(PrintValue*) = 0;
  virtual std::string VisitDecrement(Decrement*) = 0;
  virtual std::string VisitIncrement(Increment*) = 0;
  virtual std::string VisitAssign(Assign*) = 0;
  virtual std::string VisitAdd(Add*) = 0;
  virtual std::string VisitMultiply(Multiply*) = 0;
  virtual std::string VisitSquigglyMoveTo(SquigglyMoveTo*) = 0;
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
  ParsedAction ConsumeAction(std::string action);
  std::set<std::string> symbols;
  std::map<std::string, ActionFactory*> action_factories;
  std::map<std::string, std::string> action_names;
};


#endif
