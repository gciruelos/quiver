#include "program.h"

#include <fstream>
#include <vector>
#include <iostream>

#define PRINT_END(x) (((x) == end_node)? "<END>" : std::to_string(x))

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
    nodes_.insert(node);
  }
  state_ = std::unique_ptr<ProgramState>(new ProgramState(nodes_));
}


Program::~Program() {
  for (auto& c : conditions_) {
    c.second.reset(nullptr);
  }
  for (auto& a : actions_) {
    for (auto& b : a.second) {
      b.second.reset(nullptr);
    }
  }
}

void Program::ShowParsed() {
  for (auto& node : nodes_) {
    std::cout << node << " "
              << "?" << conditions_.at(node)->Debug() << "?  "
              << PRINT_END(if_true_.at(node)) << " "
              << actions_[node][if_true_.at(node)]->Debug() << "  "
              << PRINT_END(if_false_.at(node)) << " "
              << actions_[node][if_false_.at(node)]->Debug() << " "
              << std::endl;
  }
}

void Program::Execute() {
  while (state_->CurrentNode() != end_node) {
    uint64_t current_node = state_->CurrentNode();
    state_->NextNode() = GetNextNode();
    /*
    std::cerr << "Current node: " << current_node << "\t"
              << "Next node: " << state_->NextNode() << "\t"
              << "Accumulator value: " << state_->Accumulator() << "\t"
              << "Current node value: " << state_->CurrentNodeValue()
              << std::endl;
    for (uint64_t j_ = 0; j_ < 100000000; j_++);
    */
    actions_[current_node][state_->NextNode()]->Do(state_.get());
    state_->CurrentNode() = state_->NextNode();
    state_->LastNode() = current_node;
  }
}

uint64_t Program::GetNextNode() {
  uint64_t current_node = state_->CurrentNode();
  return (conditions_[current_node]->Check(state_.get())
      ? if_true_[current_node]
      : if_false_[current_node]);
}



