#include "program.h"

#include <fstream>
#include <vector>
#include <iostream>

Program::Program(std::string filename) {
  std::ifstream infile(filename);
  std::string line;
  while (std::getline(infile, line)) {
    LineParser parser(line);
    uint64_t node = parser.GetNode();
    conditions_.insert(std::make_pair(
          node, std::unique_ptr<Condition>(parser.GetCondition())));
    std::pair<uint64_t, Action*> if_true = parser.GetIfTrue();
    std::pair<uint64_t, Action*> if_false = parser.GetIfFalse();
    if_true_.insert(std::make_pair(node, if_true.first));
    if_false_.insert(std::make_pair(node, if_false.first));
    actions_[node].insert(std::make_pair(
        if_true.first, std::unique_ptr<Action>(if_true.second)));
    actions_[node].insert(std::make_pair(
        if_false.first, std::unique_ptr<Action>(if_false.second)));
  }
  current_node_ = 0;
  accumulator_ = 0;
}


void Program::Execute() {
  while (current_node_ != end_node) {
    uint64_t next_node = GetNextNode();
    actions_[current_node_][next_node]->Do(&accumulator_);
    current_node_ = next_node;
  }
}

uint64_t Program::GetNextNode() {
 return (conditions_[current_node_]->Check(accumulator_)
     ? if_true_[current_node_]
     : if_false_[current_node_]);
}



