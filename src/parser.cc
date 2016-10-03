#include "parser.h"

#include <iostream>
#include <utility>

uint64_t LineParser::ParseNode(std::string node) {
  if (node.size() == 0) {
    return end_node;
  } else {
    return std::stoi(node);
  }
}

LineParser::LineParser(std::string line) {
  std::string from;
  std::string condition;
  std::string to1;
  std::string action1;
  std::string to2;
  std::string action2;

  ActionBuilder& action_builder = ActionBuilder::Instance();
  ConditionBuilder& condition_builder = ConditionBuilder::Instance();

  auto line_iter = line.begin();
  while (*line_iter != condition_mark_) {
    from.push_back(*line_iter++);
  }
  line_iter++;
  while (*line_iter != condition_mark_) {
    condition.push_back(*line_iter++);
  }
  line_iter++;
  while (*line_iter != open_action_) {
    to1.push_back(*line_iter++);
  }
  line_iter++;
  while (*line_iter != close_action_) {
    action1.push_back(*line_iter++);
  }
  line_iter++;
  while (*line_iter != open_action_) {
    to2.push_back(*line_iter++);
  }
  line_iter++;
  while (*line_iter != close_action_) {
    action2.push_back(*line_iter++);
  }
  line_iter++;

  from_ = ParseNode(from);
  c_ = std::unique_ptr<Condition>(condition_builder.BuildCondition(condition));
  if_true_ = std::make_pair(
      ParseNode(to1),
      std::unique_ptr<Action>(action_builder.BuildAction(action1)));
  if_false_ = std::make_pair(
      ParseNode(to2),
      std::unique_ptr<Action>(action_builder.BuildAction(action2)));
}

uint64_t LineParser::GetNode() const {
  return from_;
}

Condition* LineParser::GetCondition() {
  return c_.release();
}

std::pair<uint64_t, Action*> LineParser::GetIfTrue() {
  return std::make_pair(if_true_.first, if_true_.second.release());
}

std::pair<uint64_t, Action*> LineParser::GetIfFalse() {
  return std::make_pair(if_false_.first, if_false_.second.release());
}
