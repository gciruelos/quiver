#include <utility>

#include "condition.h"
#include "action.h"


class LineParser {
 public:
  LineParser(std::string line);
  
  uint64_t GetNode() const;
  Condition GetCondition() const;
  std::pair<uint64_t, Action> GetIfTrue() const;
  std::pair<uint64_t, Action> GetIfFalse() const;

 private:
  uint64_t from_;
  Condition c_;
  std::pair<uint64_t, Action> if_true_;
  std::pair<uint64_t, Action> if_false_;
}
