#include "action.h"

#include <string>
#include <sstream>
#include <utility>

#define NOTHING_SYMBOL "_???"

#define REGISTER_ACTION(sym_, klass) \
    const std::string klass::symbol = sym_; \
    class klass##Factory : public ActionFactory { \
     public: \
      klass##Factory() { \
        ActionBuilder::Instance().Register(sym_, #klass, this); \
      } \
      virtual Action* Create(ParsedAction s) { \
        return new klass(s); \
      } \
    }; \
    static klass##Factory global_##klass##Factory;

AffectedValue GetAffectedValue(char indicator) {
  switch (indicator) {
    case '[':
      return CURRENT_NODE;
    case ']':
      return NEXT_NODE;
    default:
      return ACCUMULATOR;
  }
}

uint64_t& AffectedNode(ProgramState* state, AffectedValue v) {
  switch (v) {
    case CURRENT_NODE:
      return state->NodeValue(state->CurrentNode());
    case NEXT_NODE:
      return state->NodeValue(state->NextNode());
    default:
      return state->Accumulator();
  }
}


Nothing::Nothing(ParsedAction s __attribute__((unused))) {
}
void Nothing::Do(ProgramState* state __attribute__((unused))) {
  return;
}
std::string Nothing::Accept(ActionVisitor* visitor) {
  return visitor->VisitNothing(this);
}
REGISTER_ACTION(NOTHING_SYMBOL, Nothing)


Print::Print(ParsedAction s) : print(ParseString(s.second)) {
}
void Print::Do(ProgramState* state __attribute__((unused))) {
  std::cout << print;
}
std::string Print::Accept(ActionVisitor* visitor) {
  return visitor->VisitPrint(this);
}
REGISTER_ACTION("'", Print)


PrintValue::PrintValue(ParsedAction s) {
  aff = GetAffectedValue(s.first[0]);
}
void PrintValue::Do(ProgramState* state) {
  std::cout << AffectedNode(state, aff);
}
std::string PrintValue::Accept(ActionVisitor* visitor) {
  return visitor->VisitPrintValue(this);
}
REGISTER_ACTION("p", PrintValue)


Decrement::Decrement(ParsedAction s) {
  aff = GetAffectedValue(s.first[0]);
}
void Decrement::Do(ProgramState* state) {
  AffectedNode(state, aff)--;
}
std::string Decrement::Accept(ActionVisitor* visitor) {
  return visitor->VisitDecrement(this);
}
REGISTER_ACTION("--", Decrement)


Increment::Increment(ParsedAction s) {
  aff = GetAffectedValue(s.first[0]);
}
void Increment::Do(ProgramState* state) {
  AffectedNode(state, aff)++;
}
std::string Increment::Accept(ActionVisitor* visitor) {
  return visitor->VisitIncrement(this);
}
REGISTER_ACTION("++", Increment)


Assign::Assign(ParsedAction s) : new_val(std::stoi(s.second)) {
  aff = GetAffectedValue(s.first[0]);
}
void Assign::Do(ProgramState* state) {
  AffectedNode(state, aff) = new_val;
}
std::string Assign::Accept(ActionVisitor* visitor) {
  return visitor->VisitAssign(this);
}
REGISTER_ACTION("=", Assign)


SquigglyMoveTo::SquigglyMoveTo(ParsedAction s __attribute__((unused))) {
}
void SquigglyMoveTo::Do(ProgramState* state) {
  state->NodeValue(state->NextNode()) = state->CurrentNodeValue();
}
std::string SquigglyMoveTo::Accept(ActionVisitor* visitor) {
  return visitor->VisitSquigglyMoveTo(this);
}
REGISTER_ACTION("~>", SquigglyMoveTo)




std::pair<std::string, std::string> ActionBuilder::ConsumeAction(
    std::string action) {
  for (auto& s : symbols) {
    if (action.compare(0, s.length(), s) == 0) {
      return std::make_pair(s, SubstringFrom(action, s.length()));
    }
  }
  return std::make_pair(NOTHING_SYMBOL, "");
}

Action* ActionBuilder::BuildAction(std::string action) {
  if (action.empty()) {
    action_factories.at(action_names.at(NOTHING_SYMBOL))->Create(
        std::pair<std::string, std::string>());
  }
  if (action.find_first_not_of("1234567890") == std::string::npos &&
      action.length() > 0) {
    std::string eq = "=";
    eq.append(action);
    action = eq;
  }
  char affected = action[0];
  if (affected == '[' || affected == ']') {
    action.erase(0, 1);
  }
  std::pair<std::string, std::string> separated = ConsumeAction(action);
  for (auto& x : symbols) {
    if (action.compare(0, x.length(), x) == 0) {
      if (affected == '[' || affected == ']') {
        separated.first = affected + separated.first;
      }
      return action_factories.at(action_names.at(x))->Create(separated);
    }
  }
  return action_factories.at(
      action_names.at(NOTHING_SYMBOL))->Create(separated);
}

ActionBuilder& ActionBuilder::Instance() {
  static ActionBuilder the_instance;
  return the_instance;
}

void ActionBuilder::Register(
    std::string symbol,
    std::string name,
    ActionFactory* factory) {
  ActionBuilder::symbols.insert(symbol);
  ActionBuilder::action_names.insert(std::make_pair(symbol, name));
  ActionBuilder::action_factories.insert(std::make_pair(name, factory));
  return;
}

std::string ActionBuilder::ActionName(std::string symbol) {
  return action_names.at(symbol);
}
