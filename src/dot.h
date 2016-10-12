#ifndef DOT_H
#define DOT_H

#include <cstdint>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <memory>
#include <vector>

#include "action.h"
#include "condition.h"

// https://en.wikipedia.org/wiki/DOT_(graph_description_language)
void GenerateDot(
    const std::set<uint64_t>& nodes,
    const std::unordered_map<uint64_t, uint64_t>& node_to_index,
    const std::vector<bool>& same_action,
    const std::vector<std::unique_ptr<Action>>& action_if_true,
    const std::vector<std::unique_ptr<Action>>& action_if_false,
    const std::vector<std::unique_ptr<Condition>>& conditions,
    const std::vector<uint64_t>& if_true,
    const std::vector<uint64_t>& if_false);

#endif
