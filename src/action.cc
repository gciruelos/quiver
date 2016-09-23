#include "action.h"

#include <string>

#define NOTHING_SYMBOL "_???"

/*

std::unique_ptr<ActionBuilder> ActionBuilder::instance_ = nullptr;
std::map<std::string, ActionFactory*> ActionBuilder::action_factories;
std::map<std::string, std::string> ActionBuilder::action_names;
std::set<std::string> ActionBuilder::symbols;
*/

#define REGISTER_ACTION(sym,klass) \
    class klass##Factory : public ActionFactory { \
    public: \
        klass##Factory() \
        { \
            ActionBuilder::Instance().Register(sym, #klass, this); \
        } \
        virtual Action* Create(ParsedAction s) { \
            return new klass(s); \
        } \
    }; \
    static klass##Factory global_##klass##Factory;

REGISTER_ACTION("'", Print)
REGISTER_ACTION("p", PrintAccumulator)
REGISTER_ACTION("--", Decrement)
REGISTER_ACTION("=", Assign)
REGISTER_ACTION(NOTHING_SYMBOL, Nothing)



std::pair<std::string, std::string> ActionBuilder::ConsumeAction(std::string action) {

  for (auto& s : symbols) {
    if (action.compare(0, s.length(), s) == 0) {
      return std::make_pair(s, SubstringFrom(action, s.length())); 
    }
  }
  return std::make_pair(NOTHING_SYMBOL, "");
}

Action* ActionBuilder::BuildAction(std::string action) {
  if (action.find_first_not_of("1234567890") == std::string::npos &&
      action.length() > 0) {
    std::string eq = "=";
    eq.append(action);
    action = eq;
  }
  std::pair<std::string, std::string> separated = ConsumeAction(action);
  for (auto& x : symbols) {
    if (action.compare(0, x.length(), x) == 0) {
      return action_factories.at(action_names.at(x))->Create(separated); 
    }
  }
  return action_factories.at(action_names.at(NOTHING_SYMBOL))->Create(separated);
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
