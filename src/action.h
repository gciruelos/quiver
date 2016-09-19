#include <cstdint>
#include <string>

class Action {
  public:
   virtual void Do(uint64_t* accumulator) = 0;
}


class Print : public Action {
 public:
  Print(std::string print) : print_(print);

  virtual void Do(uint64_t* accumulator);
 private:
  std::string print_;
}



