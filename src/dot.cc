#include "dot.h"

#include <fstream>

#include "node.h"
#include "pretty.h"

void GenerateDot(
    std::string filename,
    const std::set<uint64_t>& nodes,
    const std::unordered_map<uint64_t, uint64_t>& node_to_index,
    const std::vector<bool>& same_action,
    const std::vector<std::unique_ptr<Action>>& action_if_true,
    const std::vector<std::unique_ptr<Action>>& action_if_false,
    const std::vector<std::unique_ptr<Condition>>& conditions,
    const std::vector<uint64_t>& if_true,
    const std::vector<uint64_t>& if_false) {

  std::ofstream fout(filename.c_str(), std::ios::out | std::ios::trunc);
  if (!fout) {
    std::cerr << "Failed to open file: " << filename;
    return;
  }
  uint64_t min = 0;  // The minimum node number is always 0.

  ConditionPretty condition_pretty;
  ActionPretty action_pretty;

  fout << "digraph quiver {" << std::endl;
  for (uint64_t node : nodes) {
    uint64_t index = node_to_index.find(node) != node_to_index.end()
                     ? node_to_index.at(node)
                     : end_node;
    std::string cond = conditions.at(index)->Accept(&condition_pretty);
    fout << "n" << index
         << " [label=\""
         << (cond == "<empty>" ? "" : cond)
         << "\""
         << (index == min ? " color=blue shape=box" : "")
         << "];" << std::endl;
  }
  fout << "n" << end_node << "[label=\"END\" shape=box];" << std::endl;
  for (uint64_t i = 0; i < if_true.size(); i++) {
    std::string a = action_if_true[i]->Accept(&action_pretty);
    fout << "n" << i
         << " -> "
         << "n" << if_true[i]
         << " [label=\""
         << (a == "<nothing>" ? "" : a)
         << "\""
         << "color=green"
         << "];" << std::endl;
  }
  for (uint64_t i = 0; i < if_false.size(); i++) {
    if (conditions[i]->Accept(&condition_pretty) != "<empty>") {
      std::string a = (same_action[i] ? action_if_true[i] : action_if_false[i])
          ->Accept(&action_pretty);
      fout << "n" << i
           << " -> "
           << "n" << if_false[i]
           << " [label=\""
           << (a == "<nothing>" ? "" : a)
           << "\""
           << "color=red"
           << "];" << std::endl;
    }
  }
  fout << "}" << std::endl;
}

