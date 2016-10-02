#include "utils.h"

std::string ReplaceAll(
    std::string str,
    const std::string& from,
    const std::string& to) {
  size_t start_pos = 0;
  while((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return str;
}

std::string ParseString(std::string input_string) {
  return ReplaceAll(
      ReplaceAll(input_string, "\\n", "\n"),
      "\\)", ")");
}

std::string SubstringFrom(std::string str, size_t from) {
  return str.substr(from, str.size() - from);
}


