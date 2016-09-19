#include "condition.h"

bool Equality::Check(uint64_t accumulator) {
  return accumulator == value_;
}

bool Greater::Check(uint64_t accumulator) {
  return accumulator > value_;
}

bool Empty::Check(uint64_t accumulator __attribute__((unused))) {
  return true;
}


std::pair<std::string, std::string> ConsumeOperation(std::string condition) {
  if (condition.empty()) {
    return std::make_pair("", "");
  } else if (condition[0] == '>') {
    if (condition.size() > 1) {
      if (condition[1] == '=') {
        return std::make_pair(
            ">=", condition.substr(2, condition.size() - 2));
      } else {
        return std::make_pair(
            ">", condition.substr(1, condition.size() - 1));
      }
    } else {
      return std::make_pair("", "");
    }
  }
  return std::make_pair("", "");
}


Condition* BuildCondition(std::string condition) {
  std::pair<std::string, std::string> separated = ConsumeOperation(condition);
  if (separated.first == "") {
    return new Empty;
  } else if (separated.first == ">") {
    return new Greater(std::stoi(separated.second));
  }
  return new Empty;
}

