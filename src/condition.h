#ifndef CONDITION_H
#define CONDITION_H

#include <cstdint>
#include <map>
#include <string>

#include "state.h"

typedef std::pair<std::string, std::string> ParsedCondition;

class Condition {
  public:
   virtual bool Check(ProgramState* state) = 0;
};

class ConditionFactory {
 public:
  virtual Condition* Create(std::pair<std::string, std::string>) = 0;
};


class Equality : public Condition {
 public:
  Equality(ParsedCondition);
  virtual bool Check(ProgramState* state);
 private:
  uint64_t value_;
};

class Greater : public Condition {
 public:
  Greater(ParsedCondition);
  virtual bool Check(ProgramState* state);
 private:
  uint64_t value_;
};

class Empty : public Condition {
 public:
  Empty(ParsedCondition);
  virtual bool Check(ProgramState* state);
};

class ConditionBuilder {
 public:
  static ConditionBuilder& Instance();
  Condition* BuildCondition(std::string condition);
  void Register(
      std::string,
      std::string,
      ConditionFactory*);
 protected:
  ConditionBuilder() { }
 private:
  std::pair<std::string, std::string> ConsumeCondition(std::string action);
  std::set<std::string> symbols;
  std::map<std::string, ConditionFactory*> condition_factories;
  std::map<std::string, std::string> condition_names;
};

#endif
