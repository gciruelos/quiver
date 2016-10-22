#include "state.h"

#include <algorithm>
#include <utility>

ProgramState::ProgramState(uint64_t nodes)
  : accumulator_(0),
    current_node_(0),
    last_node_(current_node_) {
  for (uint64_t i = 0; i < nodes; i++) {
    node_values_.push_back(0);
  }
}

uint64_t& ProgramState::Accumulator() {
  return accumulator_;
}

uint64_t const & ProgramState::Accumulator() const {
  return accumulator_;
}

uint64_t& ProgramState::NodeValue(uint64_t node) {
  return node_values_[node];
}

uint64_t const & ProgramState::NodeValue(uint64_t node) const {
  return node_values_[node];
}

uint64_t& ProgramState::CurrentNodeValue() {
  return node_values_[current_node_];
}

uint64_t const & ProgramState::CurrentNodeValue() const {
  return node_values_[current_node_];
}

uint64_t& ProgramState::CurrentNode() {
  return current_node_;
}

uint64_t const & ProgramState::CurrentNode() const {
  return current_node_;
}

uint64_t& ProgramState::LastNode() {
  return last_node_;
}

uint64_t const & ProgramState::LastNode() const {
  return last_node_;
}

uint64_t& ProgramState::NextNode() {
  return next_node_;
}

uint64_t const & ProgramState::NextNode() const {
  return next_node_;
}

std::ostream& ProgramState::OutputBuffer() {
  return output_buffer_;
}

std::stringstream& ProgramState::OutputBufferString() {
  return output_buffer_;
}
