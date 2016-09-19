#ifndef CONDITION_H
#define CONDITION_H

#include <cstdint>
#include <string>

class Condition {
  public:
   virtual bool Check(uint64_t accumulator) = 0;
};


class Equality : public Condition {
 public:
  Equality(uint64_t value) : value_(value) {}
  virtual bool Check(uint64_t accumulator);
 private:
  uint64_t value_;
};

class Greater : public Condition {
 public:
  Greater(uint64_t value) : value_(value) {}
  virtual bool Check(uint64_t accumulator);
 private:
  uint64_t value_;
};

class Empty : public Condition {
 public:
  Empty() {}
  virtual bool Check(uint64_t accumulator);
};

Condition* BuildCondition(std::string condition);

#endif
