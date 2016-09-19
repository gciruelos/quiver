#include "action.h"

#include <iostream>
#include <string>



void Print::Do(uint64_t* accumulator __attribute__((unused))) {
  std::cout << print_;
}

void Nothing::Do(uint64_t* accumulator __attribute__((unused))) {
  return;
}


Action* BuildAction(std::string action) {
  if (action[0] == '\'' && action[action.size()-1] == '\'') {
    return new Print(action.substr(1, action.size() - 2));
  }
  return nullptr;
}
