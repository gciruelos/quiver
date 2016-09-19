#ifndef ACTION_H
#define ACTION_H

#include <cstdint>
#include <string>

class Action {
  public:
   virtual void Do(uint64_t* accumulator) = 0;
};


class Print : public Action {
 public:
  Print(std::string print) : print_(print) {}

  virtual void Do(uint64_t* accumulator);
 private:
  std::string print_;
};

class Nothing : public Action {
 public:
  virtual void Do(uint64_t* accumulator);
};

Action* BuildAction(std::string action);

#endif
