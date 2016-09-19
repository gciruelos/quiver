#include "utils.h"

#include <regex>

std::string ParseString(std::string input_string) {
  std::regex newline("\\\\n");
  std::regex parens("\\)");
  input_string = std::regex_replace(input_string, newline, "\n");
  input_string = std::regex_replace(input_string, parens, ")");
  return input_string;
}

