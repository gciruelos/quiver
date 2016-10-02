#include "condition.h"

#include <sstream>

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

Equality::Equality(ParsedCondition s) {
  if (s.second.empty()) {
    arg_is_value = false;
  } else {
    value_ = std::stoi(s.second);
    arg_is_value = true;
  }
}
bool Equality::Check(ProgramState* state) {
  if (arg_is_value) {
    return state->Accumulator() == value_;
  } else {
    return state->Accumulator() == state->CurrentNodeValue();
  }
}
std::string Equality::Debug() {
  std::stringstream buffer;
  buffer << GetConditionName(symbol)
         << "{" << (arg_is_value ? std::to_string(value_) : "curr") << "}";
  return buffer.str();
}
REGISTER_CONDITION("==", Equality)

Greater::Greater(ParsedCondition s) : value_(std::stoi(s.second)) {
}
bool Greater::Check(ProgramState* state) {
  return state->Accumulator() > value_;
}
std::string Greater::Debug() {
  std::stringstream buffer;
  buffer << GetConditionName(symbol) << "{}";
  return buffer.str();
}
REGISTER_CONDITION(">", Greater)

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
  for (auto& x : symbols) {
    if (condition.compare(0, x.length(), x) == 0) {
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
