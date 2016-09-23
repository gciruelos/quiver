#include "utils.h"

#include <regex>

std::string ParseString(std::string input_string) {
  /*
  std::regex newline("\\\\n");
  std::regex parens("\\)");
  std::string s1, s2;
  s1 = std::regex_replace(input_string, newline, "\n");
  s2 = std::regex_replace(s1, parens, ")");
  */
  return input_string;
}

std::string SubstringFrom(std::string str, size_t from) {
  return str.substr(from, str.size() - from);
}


