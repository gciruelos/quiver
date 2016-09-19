#include "condition.h"

bool Equality::Check(uint64_t accumulator) {
  return accumulator == value_;
}

bool Empty::Check(uint64_t accumulator __attribute__((unused))) {
  return true;
}

Condition* BuildCondition(std::string condition) {
  if (condition.empty()) {
    return new Empty;
  }
  return nullptr;
}

