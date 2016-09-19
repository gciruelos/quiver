#ifndef STATE_H
#define STATE_H

#include <unordered_map>
#include <cstdint>
#include <set>

class ProgramState {
 public:
  ProgramState(std::set<uint64_t> nodes);

  uint64_t& GetAccumulator();
  uint64_t const & GetAccumulator() const;

  uint64_t& GetNodeValue();
  uint64_t const & GetNodeValue() const;

  uint64_t& GetCurrentNode();
  uint64_t const & GetCurrentNode() const;

 private:
  uint64_t accumulator_;
  uint64_t current_node_;
  std::unordered_map<uint64_t, uint64_t> node_values_;
};

#endif
