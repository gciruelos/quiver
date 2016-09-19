#ifndef CONDITION_H
#define CONDITION_H

#include <cstdint>
#include <string>

#include "state.h"

class Condition {
  public:
   virtual bool Check(ProgramState* state) = 0;
};


class Equality : public Condition {
 public:
  Equality(uint64_t value) : value_(value) {}
  virtual bool Check(ProgramState* state);
 private:
  uint64_t value_;
};

class Greater : public Condition {
 public:
  Greater(uint64_t value) : value_(value) {}
  virtual bool Check(ProgramState* state);
 private:
  uint64_t value_;
};

class Empty : public Condition {
 public:
  Empty() {}
  virtual bool Check(ProgramState* state);
};

Condition* BuildCondition(std::string condition);

#endif
