#include "program.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <utility>

#include "debug.h"
#include "dot.h"

#define PRINT_END(x) (((x) == end_node)? "<END>" : std::to_string(x))

Program::Program(Argv* args) : args_(args) {
  std::ifstream infile(args_->Rest()[0]);
  std::string line;
  size_t line_number = 1;
  while (std::getline(infile, line)) {
    std::unique_ptr<LineParser> parser;
    try {
      parser = std::unique_ptr<LineParser>(new LineParser(line));
    } catch (const std::exception& e) {
      HaltAndCatchFire(line_number, line);
    }
    if (parser->Failed()) {
      HaltAndCatchFire(line_number, line);
    }
    uint64_t node = parser->GetNode();
    conditions_.insert(std::make_pair(
          node, std::unique_ptr<Condition>(parser->GetCondition())));
    std::pair<uint64_t, Action*> if_true = parser->GetIfTrue();
    std::pair<uint64_t, Action*> if_false = parser->GetIfFalse();
    if_true_.insert(std::make_pair(node, if_true.first));
    if_false_.insert(std::make_pair(node, if_false.first));
    actions_[node].insert(std::make_pair(
        if_true.first, std::unique_ptr<Action>(if_true.second)));
    actions_[node].insert(std::make_pair(
        if_false.first, std::unique_ptr<Action>(if_false.second)));
    nodes_.insert(node);
    line_number++;
  }
  state_ = std::unique_ptr<ProgramState>(new ProgramState(nodes_));
  if (args_->Rest().size() > 1) {
    state_->Accumulator() = std::stoi(args_->Rest()[1]);
  }
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
  ActionDebug action_debug;
  ConditionDebug condition_debug;
  for (auto& node : nodes_) {
    std::cout << node << " "
              << "?" << conditions_.at(node)->Accept(&condition_debug) << "?  "
              << PRINT_END(if_true_.at(node)) << " "
              << actions_[node][if_true_.at(node)]->Accept(&action_debug)
              << "  "
              << PRINT_END(if_false_.at(node)) << " "
              << actions_[node][if_false_.at(node)]->Accept(&action_debug)
              << " "
              << std::endl;
  }
}

void Program::Execute() {
  while (state_->CurrentNode() != end_node) {
    uint64_t current_node = state_->CurrentNode();
    state_->NextNode() = GetNextNode();
    if (args_->Check("debug")) {
      std::cerr << "Current node: " << current_node << "\t"
                << "Next node: " << state_->NextNode() << "\t"
                << "Accumulator value: " << state_->Accumulator() << "\t"
                << "Current node value: " << state_->CurrentNodeValue()
                << std::endl;
    }
    actions_[current_node][state_->NextNode()]->Do(state_.get());
    state_->CurrentNode() = state_->NextNode();
    state_->LastNode() = current_node;
  }
}

void Program::Dot() {
  GenerateDot("graph.dot", nodes_, actions_, conditions_, if_true_, if_false_);
}

uint64_t Program::GetNextNode() {
  uint64_t current_node = state_->CurrentNode();
  return (conditions_[current_node]->Check(state_.get())
      ? if_true_[current_node]
      : if_false_[current_node]);
}

void Program::HaltAndCatchFire(size_t line_number, std::string line) {
  std::cerr << "Fatal error while parsing line " << line_number
            << ": " << line << std::endl;
  exit(1);
}

