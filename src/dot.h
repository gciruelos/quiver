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
    std::string filename,
    const std::set<uint64_t>&,
    const std::map<uint64_t, std::map<uint64_t, std::unique_ptr<Action>>>&,
    const std::map<uint64_t, std::unique_ptr<Condition>>&,
    const std::unordered_map<uint64_t, uint64_t>&,
    const std::unordered_map<uint64_t, uint64_t>&);

#endif
