#ifndef STATE_H
#define STATE_H

#include <cstdint>
#include <vector>
#include <sstream>
#include <iostream>

class ProgramState {
 public:
  explicit ProgramState(uint64_t nodes);

  uint64_t& Accumulator();
  uint64_t const& Accumulator() const;

  uint64_t& CurrentNodeValue();
  uint64_t const& CurrentNodeValue() const;

  uint64_t& NodeValue(uint64_t node);
  uint64_t const& NodeValue(uint64_t node) const;

  uint64_t& CurrentNode();
  uint64_t const& CurrentNode() const;

  uint64_t& LastNode();
  uint64_t const& LastNode() const;

  uint64_t& NextNode();
  uint64_t const& NextNode() const;

  std::ostream& OutputBuffer();

  std::string OutputBufferString();
  void EmptyOutputBuffer();

 private:
  uint64_t accumulator_;
  uint64_t next_node_;
  uint64_t current_node_;
  uint64_t last_node_;
  std::vector<uint64_t> node_values_;
  std::stringstream output_buffer_;
};

#endif
