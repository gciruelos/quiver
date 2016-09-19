#include "program.h"

#include <fstream>
#include <vector>

#include <iostream>

Program::Program(std::string filename) {
  std::ifstream infile(filename);
  std::string line;
  std::vector<std::string> lines;
  while (std::getline(infile, line)) {
    lines.push_back(line);
    std::cout << line << std::endl;
  }
}


