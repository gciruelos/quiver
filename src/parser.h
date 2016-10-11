#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <string>
#include <utility>

#include "condition.h"
#include "action.h"
#include "node.h"


class LineParser {
 public:
  explicit LineParser(std::string line);

  uint64_t GetNode() const;
  Condition* GetCondition();
  std::pair<uint64_t, Action*> GetIfTrue();
  std::pair<uint64_t, Action*> GetIfFalse();

  bool Failed();

 private:
  uint64_t from_;
  std::unique_ptr<Condition> c_;
  std::pair<uint64_t, std::unique_ptr<Action>> if_true_;
  std::pair<uint64_t, std::unique_ptr<Action>> if_false_;

  const char condition_mark_ = '?';
  const char open_action_ = '(';
  const char close_action_ = ')';

  uint64_t ParseNode(std::string node);
  void ParseSection(
    std::string*,
    std::string::iterator*,
    char,
    std::string);

  bool failed_;
};

#endif
