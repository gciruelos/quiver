#include "utils.h"

std::string ReplaceAll(
    std::string str,
    const std::string& from,
    const std::string& to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
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

std::string UndoParseString(std::string input_string) {
  return ReplaceAll(
      ReplaceAll(input_string, "\n", "\\\\n"),
      ")", "\\)");
}

std::string SubstringFrom(std::string str, size_t from) {
  return str.substr(from, str.size() - from);
}

uint64_t Stoi(std::string str, bool* error) {
  uint64_t accum = 0;
  uint64_t place = 1;
  for (size_t i = 0; i < str.length(); i++) {
    size_t idx = str.length() - i - 1;
    if ('0' <= str[idx] && str[idx] <= '9') {
      accum += (str[idx] - '0') * place;
    } else {
      *error = true;
    }
    place *= 10;
  }
  *error = false;
  return accum;
}

