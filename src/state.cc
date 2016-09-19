#include "state.h"

ProgramState::ProgramState(std::set<uint64_t> nodes)
  : accumulator_(0),
    current_node_(0) {
  for (auto n : nodes) {
    node_values_.insert(std::make_pair(n, 0));
  }
}

uint64_t& ProgramState::GetAccumulator() {
  return accumulator_;
};

uint64_t const & ProgramState::GetAccumulator() const {
  return accumulator_;
}

uint64_t& ProgramState::GetNodeValue() {
  return node_values_.at(current_node_);
};

uint64_t const & ProgramState::GetNodeValue() const {
  return node_values_.at(current_node_);
}

uint64_t& ProgramState::GetCurrentNode() {
  return current_node_;
}

uint64_t const & ProgramState::GetCurrentNode() const {
  return current_node_;
}
