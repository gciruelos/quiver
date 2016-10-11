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

void LineParser::ParseSection(
    std::string* section,
    std::string::iterator* iter,
    char separator) {
  while (**iter != separator) {
    section->push_back(*(*iter)++);
  }
  (*iter)++;
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

  failed_ = true;
  auto line_iter = line.begin();
  ParseSection(&from, &line_iter, condition_mark_);
  ParseSection(&condition, &line_iter, condition_mark_);
  ParseSection(&to1, &line_iter, open_action_);
  ParseSection(&action1, &line_iter, close_action_);
  ParseSection(&to2, &line_iter, open_action_);
  ParseSection(&action2, &line_iter, close_action_);

  failed_ = false;
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

bool LineParser::Failed() {
  return failed_;
}
