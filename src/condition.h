#ifndef CONDITION_H
#define CONDITION_H

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <utility>

#include "state.h"

typedef std::pair<std::string, std::string> ParsedCondition;

class ConditionVisitor;
class Condition {
 public:
  virtual bool Check(ProgramState* state) = 0;
  virtual std::string Accept(ConditionVisitor* visitor) = 0;
};

class ConditionFactory {
 public:
  virtual Condition* Create(ParsedCondition condition) = 0;
};


class Equals : public Condition {
 public:
  explicit Equals(ParsedCondition);
  virtual bool Check(ProgramState*);
  virtual std::string Accept(ConditionVisitor*);

  uint64_t value_;
  bool arg_is_value;
  bool rotate_operands;
  static const std::string symbol;
};

class Greater : public Condition {
 public:
  explicit Greater(ParsedCondition);
  virtual bool Check(ProgramState*);
  virtual std::string Accept(ConditionVisitor*);

  uint64_t value_;
  bool arg_is_value;
  bool rotate_operands;
  static const std::string symbol;
};

class Less : public Condition {
 public:
  explicit Less(ParsedCondition);
  virtual bool Check(ProgramState*);
  virtual std::string Accept(ConditionVisitor*);

  uint64_t value_;
  bool arg_is_value;
  bool rotate_operands;
  static const std::string symbol;
};

class Divides : public Condition {
 public:
  explicit Divides(ParsedCondition);
  virtual bool Check(ProgramState*);
  virtual std::string Accept(ConditionVisitor*);

  uint64_t value_;
  bool arg_is_value;
  bool rotate_operands;
  static const std::string symbol;
};

class Empty : public Condition {
 public:
  explicit Empty(ParsedCondition);
  virtual bool Check(ProgramState*);
  virtual std::string Accept(ConditionVisitor*);

  static const std::string symbol;
};

class ConditionVisitor {
 public:
  virtual std::string VisitEmpty(Empty*) = 0;
  virtual std::string VisitEquals(Equals*) = 0;
  virtual std::string VisitGreater(Greater*) = 0;
  virtual std::string VisitLess(Less*) = 0;
  virtual std::string VisitDivides(Divides*) = 0;
};

class ConditionBuilder {
 public:
  static ConditionBuilder& Instance();
  Condition* BuildCondition(std::string condition);
  void Register(
      std::string,
      std::string,
      ConditionFactory*);
  std::string ConditionName(std::string symbol);
 protected:
  ConditionBuilder() { }
 private:
  ParsedCondition ConsumeCondition(std::string action);
  std::set<std::string> symbols;
  std::map<std::string, ConditionFactory*> condition_factories;
  std::map<std::string, std::string> condition_names;
};

#endif
