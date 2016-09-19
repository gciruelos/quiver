#include <cstdint>

class Condition {
  public:
   virtual bool Check(uint64_t accumulator) = 0;
}


class Equality : public Condition {
 public:
  Equality(uint64_t value) : value_(value);

  virtual bool Check(uint64_t accumulator);
 private:
  uint64_t value_;
};

