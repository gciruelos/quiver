#include "action.h"

#include <string>

#include "utils.h"


std::pair<std::string, std::string> ConsumeAction(std::string action) {
  if (action.empty()) {
    return std::make_pair("", "");
  } else if (action[0] == '\'') {
    return std::make_pair("'", action.substr(1, action.size() - 1)); 
  } else if (action[0] == '-') {
    if (action.size() > 1) {
      if (action[1] == '-') {
        return std::make_pair("--", action.substr(2, action.size() - 2));
      }
    }
  } else if (action[0] == '+') {
    if (action.size() > 1) {
      if (action[1] == '+') {
        return std::make_pair("++", action.substr(2, action.size() - 2));
      } else {
        return std::make_pair("+", action.substr(1, action.size() - 1));
      }
    }
  } else if (action[0] == 'p'){
    return std::make_pair("p", "");
  } else if (action[0] == 's'){
    return std::make_pair("s", "");
  } else if (action[0] == 'g'){
    return std::make_pair("g", "");
  } else {
    if (action.find_first_not_of("1234567890") == std::string::npos) {
      return std::make_pair("=", action);
    } else {
      return std::make_pair("", "");
    }
  }
  return std::make_pair("", "");
}

Action* BuildAction(std::string action) {
  std::pair<std::string, std::string> separated = ConsumeAction(action);
  if (separated.first.empty()) {
    return new Nothing;
  } else if (separated.first == "--") {
    return new Decrement;
  } else if (separated.first == "'") {
    return new Print(ParseString(action.substr(1, action.size() - 1)));
  } else if (separated.first == "p") {
    return new PrintAccumulator;
  } else if (separated.first == "s") {
    return new SaveAccumulator;
  } else if (separated.first == "g") {
    return new LoadAccumulator;
  } else if (separated.first == "=") {
    return new Assign(std::stoi(separated.second));
  }
  return new Nothing;
}
