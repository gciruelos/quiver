#include "parser.h"


uint64_t LineParser::GetNode() {
  return from_;
}

Condition LineParser::GetCondition() {
  return c_;
}

std::pair<uint64_t, Action> LineParser::GetIfTrue() {
  return if_true_;
}

std::pair<uint64_t, Action> LineParser::GetIfFalse() {
  return if_false_;
}
