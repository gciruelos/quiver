#include "dot.h"

#include <fstream>

#include "pretty.h"

std::string Indent(std::string s) {
  return s;
}

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
    fout << "n" << node
         << " [label=\""
         << conditions.at(node)->Accept(&condition_pretty)
         << "\""
         << (node == min ? " color=blue shape=box" : "")
         << "];" << std::endl;
  }
  for (const auto& p : if_true) {
    fout << "n" << p.first
         << " -> "
         << "n" << p.second
         << " [label=\""
         << actions.at(p.first).at(p.second)->Accept(&action_pretty)
         << "\""
         << "color=green"
         << "];" << std::endl;
  }
  for (const auto& p : if_false) {
    fout << "n" << p.first
         << " -> "
         << "n" << p.second
         << " [label=\""
         << actions.at(p.first).at(p.second)->Accept(&action_pretty)
         << "\""
         << "color=red"
         << "];" << std::endl;
  }
  fout << "}" << std::endl;

}


