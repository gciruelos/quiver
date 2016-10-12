#include "condition.h"

#include <sstream>
#include <iostream>
#include <utility>

#include "utils.h"

#define EMPTY_SYMBOL "_???"

#define REGISTER_CONDITION(sym_, klass) \
    const std::string klass::symbol = sym_; \
    class klass##Factory : public ConditionFactory { \
     public: \
      klass##Factory() { \
        ConditionBuilder::Instance().Register(sym_, #klass, this); \
      } \
      virtual Condition* Create(ParsedCondition s) { \
        return new klass(s); \
      } \
    }; \
    static klass##Factory global_##klass##Factory;

void InitializeValue(const ParsedCondition& s,
    bool* arg_is_value, uint64_t* value, bool* rotate_operands) {
  *rotate_operands = (s.first[0] == '@');
  if (s.second.empty()) {
    *arg_is_value = false;
  } else {
    *value = std::stoi(s.second);
    *arg_is_value = true;
  }
}

std::pair<uint64_t, uint64_t> Operands(bool rotate_operands, bool arg_is_value,
    uint64_t value, ProgramState* s) {
  uint64_t fst_op, snd_op;
  fst_op = s->Accumulator();
  snd_op = arg_is_value ? snd_op = value : snd_op = s->CurrentNodeValue(); 
  return rotate_operands ?
      std::make_pair(snd_op, fst_op) : std::make_pair(fst_op, snd_op);
}


Equals::Equals(ParsedCondition s) {
  InitializeValue(s, &arg_is_value, &value_, &rotate_operands);
}
bool Equals::Check(ProgramState* state) {
  std::pair<uint64_t, uint64_t> operands(Operands(
        rotate_operands, arg_is_value, value_, state));
  return operands.first == operands.second;
}
std::string Equals::Accept(ConditionVisitor* visitor) {
  return visitor->VisitEquals(this);
}
REGISTER_CONDITION("==", Equals)


Greater::Greater(ParsedCondition s) {
  InitializeValue(s, &arg_is_value, &value_, &rotate_operands);
}
bool Greater::Check(ProgramState* state) {
  std::pair<uint64_t, uint64_t> operands(Operands(
        rotate_operands, arg_is_value, value_, state));
  return operands.first > operands.second;
}
std::string Greater::Accept(ConditionVisitor* visitor) {
  return visitor->VisitGreater(this);
}
REGISTER_CONDITION(">", Greater)


Less::Less(ParsedCondition s) {
  InitializeValue(s, &arg_is_value, &value_, &rotate_operands);
}
bool Less::Check(ProgramState* state) {
  std::pair<uint64_t, uint64_t> operands(Operands(
        rotate_operands, arg_is_value, value_, state));
  return operands.first < operands.second;
}
std::string Less::Accept(ConditionVisitor* visitor) {
  return visitor->VisitLess(this);
}
REGISTER_CONDITION("<", Less)


Divides::Divides(ParsedCondition s) {
  InitializeValue(s, &arg_is_value, &value_, &rotate_operands);
}
bool Divides::Check(ProgramState* state) {
  std::pair<uint64_t, uint64_t> operands(Operands(
        rotate_operands, arg_is_value, value_, state));
  return (operands.second % operands.first) == 0;
}
std::string Divides::Accept(ConditionVisitor* visitor) {
  return visitor->VisitDivides(this);
}
REGISTER_CONDITION("|", Divides)


Empty::Empty(ParsedCondition __attribute__((unused))) {
}
bool Empty::Check(ProgramState* state __attribute__((unused))) {
  return true;
}
std::string Empty::Accept(ConditionVisitor* visitor) {
  return visitor->VisitEmpty(this);
}
REGISTER_CONDITION(EMPTY_SYMBOL, Empty)


ParsedCondition ConditionBuilder::ConsumeCondition(std::string condition) {
  for (auto& s : symbols) {
    if (condition.compare(0, s.length(), s) == 0) {
      return std::make_pair(s, SubstringFrom(condition, s.length()));
    }
  }
  return std::make_pair(EMPTY_SYMBOL, "");
}


Condition* ConditionBuilder::BuildCondition(std::string condition) {
  ParsedCondition separated = ConsumeCondition(condition);
  char affected = condition[0];
  if (affected == '@') {
    condition.erase(0, 1);
  }
  for (auto& x : symbols) {
    if (condition.compare(0, x.length(), x) == 0) {
      if (affected == '@') {
        separated.first = affected + separated.first;
      }
      return condition_factories.at(condition_names.at(x))->Create(separated);
    }
  }
  return condition_factories.at(
      condition_names.at(EMPTY_SYMBOL))->Create(separated);
}


ConditionBuilder& ConditionBuilder::Instance() {
  static ConditionBuilder the_instance;
  return the_instance;
}


void ConditionBuilder::Register(
    std::string symbol,
    std::string name,
    ConditionFactory* factory) {
  ConditionBuilder::symbols.insert(symbol);
  ConditionBuilder::condition_names.insert(std::make_pair(symbol, name));
  ConditionBuilder::condition_factories.insert(std::make_pair(name, factory));
}


std::string ConditionBuilder::ConditionName(std::string symbol) {
  return condition_names.at(symbol);
}
