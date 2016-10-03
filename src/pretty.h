#ifndef PRETTY_H
#define PRETTY_H

#include "action.h"
#include "condition.h"

class ActionPretty : public ActionVisitor {
 public:
  virtual std::string VisitNothing(Nothing*);
  virtual std::string VisitPrint(Print*);
  virtual std::string VisitPrintValue(PrintValue*);
  virtual std::string VisitDecrement(Decrement*);
  virtual std::string VisitIncrement(Increment*);
  virtual std::string VisitAssign(Assign*);
  virtual std::string VisitSquigglyMoveTo(SquigglyMoveTo*);
};

class ConditionPretty : public ConditionVisitor {
 public:
  virtual std::string VisitEmpty(Empty*);
  virtual std::string VisitEquals(Equals*);
  virtual std::string VisitGreater(Greater*);
  virtual std::string VisitLess(Less*);
  virtual std::string VisitDivides(Divides*);
};

#endif
