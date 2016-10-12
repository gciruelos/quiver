#include "program.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <tuple>
#include <utility>

#include "debug.h"
#include "dot.h"

#define PRINT_END(x) (((x) == end_node)? "<END>" : std::to_string(x))


typedef std::tuple<
    uint64_t, std::unique_ptr<Condition>,
    uint64_t, std::unique_ptr<Action>,
    uint64_t, std::unique_ptr<Action>> LineResult;


Program::Program(Argv* args) : args_(args) {
  std::ifstream infile(args_->Rest()[0]);
  if (!infile) {
    std::cerr << "Failed to open file: " << args_->Rest()[0] << std::endl;;
    exit(0);
  }
  std::string line;
  size_t line_number = 1;
  std::vector<LineResult> results;
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
    std::pair<uint64_t, Action*> if_true = parser->GetIfTrue();
    std::pair<uint64_t, Action*> if_false = parser->GetIfFalse();

    results.emplace_back(
        node, std::unique_ptr<Condition>(parser->GetCondition()),
        if_true.first, std::unique_ptr<Action>(if_true.second),
        if_false.first, std::unique_ptr<Action>(if_false.second));

    nodes_.insert(node);
    line_number++;
  }

  uint64_t index = 0;
  for (uint64_t node : nodes_) {
    node_to_index_.insert(std::make_pair(node, index++));
    index_to_node_.push_back(node);
  }

  for (auto& result : results) {
    // Everyhing comes in order, so we can do push_back.
    uint64_t if_true = NodeToIndex(std::get<2>(result));
    uint64_t if_false = NodeToIndex(std::get<4>(result));
    conditions_.push_back(std::move(std::get<1>(result)));
    if_true_.push_back(if_true);
    if_false_.push_back(if_false);
    if (if_true == if_false) {
      action_if_true_.emplace_back(std::move(std::get<3>(result)));
      action_if_false_.push_back(std::unique_ptr<Action>());
      same_action_.push_back(true);
    } else {
      action_if_true_.emplace_back(std::move(std::get<3>(result)));
      action_if_false_.emplace_back(std::move(std::get<5>(result)));
      same_action_.push_back(false);
    }
  }

  state_ = std::unique_ptr<ProgramState>(new ProgramState(nodes_.size()));
  if (args_->Rest().size() > 1) {
    state_->Accumulator() = std::stoi(args_->Rest()[1]);
  }
}

void Program::ShowParsed() {
  ActionDebug action_debug;
  ConditionDebug condition_debug;
  for (auto& node : nodes_) {
    uint64_t index = NodeToIndex(node);
    std::cerr << node << " " << "?"
              << conditions_.at(index)->Accept(&condition_debug)
              << "?  "
              << PRINT_END(if_true_[index]) << " "
              << GetAction(index, true)->Accept(&action_debug)
              << "  "
              << PRINT_END(if_false_[index]) << " "
              << GetAction(index, false)->Accept(&action_debug)
              << " "
              << std::endl;
  }
}

void Program::Execute() {
  bool debug_on = args_->Check("debug");
  while (state_->CurrentNode() != end_node) {
    bool condition = conditions_[state_->CurrentNode()]->Check(state_.get());
    uint64_t current_node = state_->CurrentNode();
    state_->NextNode() = GetNextNode(condition);
    if (debug_on) {
      std::cerr << "Current node: " << index_to_node_[current_node] << "\t"
                << "Next node: " << state_->NextNode() << "\t"
                << "Accumulator value: " << state_->Accumulator() << "\t"
                << "Current node value: " << state_->CurrentNodeValue()
                << std::endl;
    }
    GetAction(current_node, condition)->Do(state_.get());
    state_->CurrentNode() = state_->NextNode();
    state_->LastNode() = current_node;
  }
}

void Program::Dot() {
  GenerateDot(nodes_, node_to_index_, same_action_,
      action_if_true_, action_if_false_, conditions_, if_true_, if_false_);
}

uint64_t Program::NodeToIndex(uint64_t node) {
  if (node_to_index_.find(node) != node_to_index_.end()) {
    return node_to_index_.at(node);
  } else {
    return end_node;
  }
}

Action* Program::GetAction(uint64_t index, bool condition) {
  if (same_action_[index]) {
    return action_if_true_[index].get();
  } else if (condition) {
    return action_if_true_[index].get();
  } else {
    return action_if_false_[index].get();
  }
}


uint64_t Program::GetNextNode(bool condition) {
  uint64_t current_node = state_->CurrentNode();
  return condition ? if_true_[current_node] : if_false_[current_node];
}

void Program::HaltAndCatchFire(size_t line_number, std::string line) {
  std::cerr << "Fatal error while parsing line " << line_number
            << ": " << line << std::endl;
  exit(1);
}

