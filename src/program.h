#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <unordered_map>
#include <set>
#include <map>
#include <memory>

#include "argv.h"
#include "condition.h"
#include "action.h"
#include "parser.h"
#include "node.h"
#include "state.h"

class Program {
 public:
  Program(Argv* args);
  ~Program();
  void Execute();
  void ShowParsed();
 
 private:
  uint64_t GetNextNode();

  Argv* args_;
  std::map<uint64_t, std::map<uint64_t, std::unique_ptr<Action>>> actions_;
  std::map<uint64_t, std::unique_ptr<Condition>> conditions_;
  std::unordered_map<uint64_t, uint64_t> if_true_;
  std::unordered_map<uint64_t, uint64_t> if_false_;
  std::set<uint64_t> nodes_;
  std::unique_ptr<ProgramState> state_;
};
#endif
