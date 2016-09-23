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
    nodes_.insert(node);
  }
  state_ = std::unique_ptr<ProgramState>(new ProgramState(nodes_));
}


void Program::Execute() {
  while (state_->CurrentNode() != end_node) {
    uint64_t current_node = state_->CurrentNode();
    uint64_t next_node = GetNextNode();
    /*
    std::cerr << "Current node: " << current_node << "\t"
              << "Next node: " << next_node << "\t"
              << "Accumulator value: " << state_->Accumulator() << "\t"
              << "Current node value: " << state_->CurrentNodeValue()
              << std::endl;
    */
    state_->CurrentNode() = next_node;
    state_->LastNode() = current_node;
    actions_[current_node][next_node]->Do(state_.get());
  }
}

uint64_t Program::GetNextNode() {
  uint64_t current_node = state_->CurrentNode();
  return (conditions_[current_node]->Check(state_.get())
      ? if_true_[current_node]
      : if_false_[current_node]);
}



