#include "utils.h"

#include <regex>

std::string ParseString(std::string input_string) {
  std::regex new_line("\\\\n");
  input_string = std::regex_replace(input_string, new_line, "\n");
  return input_string;


}

