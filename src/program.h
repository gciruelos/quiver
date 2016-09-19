#include <string>
#include <unordered_map>

#define Condition int
#define Action int

class Program {
 public:
  Program(std::string filename);
 
 private:
  uint64_t GetNextNode();

  std::unordered_map<uint64_t, std::unordered_map<uint64_t, Action>> actions_;
  std::unordered_map<uint64_t, Condition> conditions_;
  std::unordered_map<uint64_t, uint64_t> if_true_;
  std::unordered_map<uint64_t, uint64_t> if_false_;
};
