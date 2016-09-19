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

class PrintAccumulator : public Action {
 public:
  virtual void Do(uint64_t* accumulator);
};

class Substract : public Action {
 public:
  Substract(uint64_t value) : value_(value) {}
  virtual void Do(uint64_t* accumulator);
 private:
  uint64_t value_;
};

class Add : public Action {
 public:
  Add(uint64_t value) : value_(value) {}
  virtual void Do(uint64_t* accumulator);
 private:
  uint64_t value_;
};

class Decrement : public Action {
 public:
  Decrement() {}
  virtual void Do(uint64_t* accumulator);
};

class Assign : public Action {
 public:
  Assign(uint64_t new_val) : new_val_(new_val) {}
  virtual void Do(uint64_t* accumulator);
 private:
  uint64_t new_val_;
};

class Nothing : public Action {
 public:
  Nothing() {}
  virtual void Do(uint64_t* accumulator);
};

Action* BuildAction(std::string action);

#endif
