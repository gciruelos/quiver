#ifndef ARGV_H
#define ARGV_H

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>

class Argv {
 public:
  Argv();
  void Add(char little, std::string big, std::string description);
  void Help();
  void Parse(int argc, char* argv[]);
  bool Check(std::string);
  const std::vector<std::string>& Rest();
 private:
  char BigsFiber(std::string big);

  std::set<char> littles_;
  std::set<std::string> bigs_;
  std::map<char, std::string> little_to_big_;
  std::map<std::string, bool> toggled_;
  std::map<std::string, std::string> descriptions_;
  std::vector<std::string> rest_;
  std::string program_name_;
  size_t longest_;
};

#endif
