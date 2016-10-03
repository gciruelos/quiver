#include "dot.h"

#include <fstream>

#include "node.h"
#include "pretty.h"

void GenerateDot(
    std::string filename,
    const std::set<uint64_t>& nodes,
    const std::map<uint64_t, std::map<uint64_t, std::unique_ptr<Action>>>&
    actions,
    const std::map<uint64_t, std::unique_ptr<Condition>>& conditions,
    const std::unordered_map<uint64_t, uint64_t>& if_true,
    const std::unordered_map<uint64_t, uint64_t>& if_false) {

  std::ofstream fout(filename.c_str(), std::ios::out | std::ios::trunc);
  if (!fout) {
    std::cerr << "Failed to open file: " << filename;
    return;
  }
  uint64_t min = *nodes.begin();

  ConditionPretty condition_pretty;
  ActionPretty action_pretty;

  fout << "digraph quiver {" << std::endl;
  for (uint64_t node : nodes) {
    std::string cond = conditions.at(node)->Accept(&condition_pretty);
    fout << "n" << node
         << " [label=\""
         << (cond == "<empty>" ? "" : cond)
         << "\""
         << (node == min ? " color=blue shape=box" : "")
         << "];" << std::endl;
  }
  fout << "n" << end_node << "[label=\"END\" shape=box];" << std::endl;
  for (const auto& p : if_true) {
    std::string a = actions.at(p.first).at(p.second)->Accept(&action_pretty);
    fout << "n" << p.first
         << " -> "
         << "n" << p.second
         << " [label=\""
         << (a == "<nothing>" ? "" : a)
         << "\""
         << "color=green"
         << "];" << std::endl;
  }
  for (const auto& p : if_false) {
    if (conditions.at(p.first)->Accept(&condition_pretty) != "<empty>") {
      std::string a = actions.at(p.first).at(p.second)->Accept(&action_pretty);
      fout << "n" << p.first
           << " -> "
           << "n" << p.second
           << " [label=\""
           << (a == "<nothing>" ? "" : a)
           << "\""
           << "color=red"
           << "];" << std::endl;
    }
  }
  fout << "}" << std::endl;
}

