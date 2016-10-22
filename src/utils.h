#ifndef UTILS_H
#define UTILS_H
#include <string>

std::string ParseString(std::string input_string);
std::string UndoParseString(std::string input_string);

std::string SubstringFrom(std::string str, size_t from);

uint64_t Stoi(std::string str, bool* error);

#endif
