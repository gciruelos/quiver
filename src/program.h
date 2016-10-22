#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <unordered_map>
#include <set>
#include <map>
#include <memory>
#include <vector>

#include "argv.h"
#include "condition.h"
#include "action.h"
#include "parser.h"
#include "node.h"
#include "state.h"

class Program {
 public:
  explicit Program(Argv* args, std::vector<std::string> lines);
  void Execute(std::ostream& output = std::cout);
  void ShowParsed();
  void Dot();

  uint64_t NodeToIndex(uint64_t node);

 private:
  Action* GetAction(uint64_t index, bool condition);
  uint64_t GetNextNode(bool condition);
  void HaltAndCatchFire(size_t line_number, std::string line);

  Argv* args_;

  std::vector<std::unique_ptr<Condition>> conditions_;
  std::vector<uint64_t> if_true_;
  std::vector<uint64_t> if_false_;
  std::vector<bool> same_action_;
  std::vector<std::unique_ptr<Action>> action_if_true_;
  std::vector<std::unique_ptr<Action>> action_if_false_;

  std::set<uint64_t> nodes_;
  std::unique_ptr<ProgramState> state_;

  std::vector<uint64_t> index_to_node_;
  std::unordered_map<uint64_t, uint64_t> node_to_index_;
};
#endif
