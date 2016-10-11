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

std::pair<uint64_t*, uint64_t> ActionOperands(
    AffectedValue aff,
    bool arg_is_value,
    uint64_t value,
    ProgramState* s) {
  uint64_t* fst_op;
  uint64_t snd_op;
  if (aff == ACCUMULATOR) {
    fst_op = &s->Accumulator();
    if (arg_is_value) {
      snd_op = value;
    } else {
      snd_op = s->CurrentNodeValue();
    }
  } else {
    if (aff == CURRENT_NODE) {
      fst_op = &s->CurrentNodeValue();
    } else {
      fst_op = &s->NodeValue(s->NextNode());
    }
    if (arg_is_value) {
      snd_op = value;
    } else {
      snd_op = s->Accumulator();
    }
  }
  return std::make_pair(fst_op, snd_op);
}

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

void InitializeActionValue(const ParsedAction& s,
    bool* arg_is_value, uint64_t* value, AffectedValue* affected_value) {
  *affected_value = GetAffectedValue(s.first[0]);
  if (s.second.empty()) {
    *arg_is_value = false;
  } else {
    *value = std::stoi(s.second);
    *arg_is_value = true;
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


Add::Add(ParsedAction s) {
  InitializeActionValue(s, &arg_is_value, &value, &aff);
}
void Add::Do(ProgramState* state) {
  std::pair<uint64_t*, uint64_t> operands =
      ActionOperands(aff, arg_is_value, value, state);
  *(operands.first) = operands.second + *(operands.first);
}
std::string Add::Accept(ActionVisitor* visitor) {
  return visitor->VisitAdd(this);
}
REGISTER_ACTION("+", Add)


Multiply::Multiply(ParsedAction s) {
  InitializeActionValue(s, &arg_is_value, &value, &aff);
}
void Multiply::Do(ProgramState* state) {
  std::pair<uint64_t*, uint64_t> operands =
      ActionOperands(aff, arg_is_value, value, state);
  *(operands.first) = operands.second * *(operands.first);
}
std::string Multiply::Accept(ActionVisitor* visitor) {
  return visitor->VisitMultiply(this);
}
REGISTER_ACTION("*", Multiply)


SquigglyMoveTo::SquigglyMoveTo(ParsedAction s __attribute__((unused))) {
}
void SquigglyMoveTo::Do(ProgramState* state) {
  state->NodeValue(state->NextNode()) = state->CurrentNodeValue();
}
std::string SquigglyMoveTo::Accept(ActionVisitor* visitor) {
  return visitor->VisitSquigglyMoveTo(this);
}
REGISTER_ACTION("~>", SquigglyMoveTo)




ParsedAction ActionBuilder::ConsumeAction(std::string action) {
  for (auto& s : symbols) {
    if (action.compare(0, s.length(), s) == 0) {
      return std::make_pair(s, SubstringFrom(action, s.length()));
    }
  }
  return std::make_pair(NOTHING_SYMBOL, "");
}

Action* ActionBuilder::BuildAction(std::string action) {
  if (action.empty()) {
    return action_factories.at(action_names.at(NOTHING_SYMBOL))->Create(
        ParsedAction());
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
  ParsedAction separated = ConsumeAction(action);
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
