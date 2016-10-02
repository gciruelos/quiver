#include <iostream>
#include <unordered_map>
#include <cstdint>

#include "program.h"

int main(int argc, char** argv) {
  if (argc > 1) {
    Program p(argv[1]);
    p.ShowParsed();
    //p.Execute();
  }
  return 0;
}
