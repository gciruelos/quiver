#ifndef STATE_H
#define STATE_H

#include <unordered_map>
#include <cstdint>
#include <set>

class ProgramState {
 public:
  ProgramState(std::set<uint64_t> nodes);

  uint64_t& Accumulator();
  uint64_t const & Accumulator() const;

  uint64_t& CurrentNodeValue();
  uint64_t const & CurrentNodeValue() const;

  uint64_t& NodeValue(uint64_t node);
  uint64_t const & NodeValue(uint64_t node) const;

  uint64_t& CurrentNode();
  uint64_t const & CurrentNode() const;

  uint64_t& LastNode();
  uint64_t const & LastNode() const;

 private:
  uint64_t accumulator_;
  uint64_t current_node_;
  uint64_t last_node_;
  std::unordered_map<uint64_t, uint64_t> node_values_;
};

#endif
