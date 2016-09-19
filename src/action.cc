#include "action.h"

#include <iostream>



void Print::Do(uint64_t* accumulator) {
  std::cout << print_;
}
