#include "condition.h"

#include <sstream>
#include <iostream>

#include "utils.h"

#define EMPTY_SYMBOL "_???"

#define REGISTER_CONDITION(sym_,klass) \
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

std::string GetConditionName(std::string symbol) {
  return ConditionBuilder::Instance().ConditionName(symbol);
}

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

std::string ConditionDebugString(std::string symbol, uint64_t value,
    bool arg_is_value, bool rotate_operands) {
  std::stringstream buffer;
  buffer << GetConditionName(symbol)
         << "{"
         << (arg_is_value ? std::to_string(value) : "curr")
         << (rotate_operands ? "(@)" : "")
         << "}";
  return buffer.str();
}

std::pair<uint64_t, uint64_t> Operands(bool rotate_operands, bool arg_is_value,
    uint64_t value, ProgramState* s) {
  uint64_t fst_op, snd_op;
  if (!rotate_operands) {
    fst_op = s->Accumulator();
    if (arg_is_value) {
      snd_op = value;
    } else {
      snd_op = s->CurrentNodeValue();
    }
  } else {
    snd_op = s->Accumulator();
    if (arg_is_value) {
      fst_op = value;
    } else {
      fst_op = s->CurrentNodeValue();
    }
  }
  return std::make_pair(fst_op, snd_op);
}


Equality::Equality(ParsedCondition s) {
  InitializeValue(s, &arg_is_value, &value_, &rotate_operands);
}
bool Equality::Check(ProgramState* state) {
  std::pair<uint64_t, uint64_t> operands(Operands(
        rotate_operands, arg_is_value, value_, state));
  return operands.first == operands.second;
}
std::string Equality::Debug() {
  return ConditionDebugString(symbol, value_, arg_is_value, rotate_operands);
}
REGISTER_CONDITION("==", Equality)


Greater::Greater(ParsedCondition s) {
  InitializeValue(s, &arg_is_value, &value_, &rotate_operands);
}
bool Greater::Check(ProgramState* state) {
  std::pair<uint64_t, uint64_t> operands(Operands(
        rotate_operands, arg_is_value, value_, state));
  return operands.first > operands.second;
}
std::string Greater::Debug() {
  return ConditionDebugString(symbol, value_, arg_is_value, rotate_operands);
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
std::string Less::Debug() {
  return ConditionDebugString(symbol, value_, arg_is_value, rotate_operands);
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
std::string Divides::Debug() {
  return ConditionDebugString(symbol, value_, arg_is_value, rotate_operands);
}
REGISTER_CONDITION("|", Divides)


Empty::Empty(ParsedCondition __attribute__((unused))) {
}
bool Empty::Check(ProgramState* state __attribute__((unused))) {
  return true;
}
std::string Empty::Debug() {
  std::stringstream buffer;
  buffer << GetConditionName(symbol);
  return buffer.str();
}
REGISTER_CONDITION(EMPTY_SYMBOL, Empty)


std::pair<std::string, std::string> ConditionBuilder::ConsumeCondition(std::string condition) {
  for (auto& s : symbols) {
    if (condition.compare(0, s.length(), s) == 0) {
      return std::make_pair(s, SubstringFrom(condition, s.length())); 
    }
  }
  return std::make_pair(EMPTY_SYMBOL, "");
}

Condition* ConditionBuilder::BuildCondition(std::string condition) {
  std::pair<std::string, std::string> separated = ConsumeCondition(condition);
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
  return;
}

std::string ConditionBuilder::ConditionName(std::string symbol) {
  return condition_names.at(symbol);
}
