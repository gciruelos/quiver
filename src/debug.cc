#include "debug.h"

#include <string>
#include <sstream>

#include "utils.h"


std::string GetActionName(std::string symbol) {
  return ActionBuilder::Instance().ActionName(symbol);
}


std::string ActionDebug::VisitNothing(Nothing* n) {
  return GetActionName(n->symbol) + "{" + pretty_.VisitNothing(n) + "}";
}

std::string ActionDebug::VisitPrint(Print* p) {
  return GetActionName(p->symbol) + "{" + pretty_.VisitPrint(p) + "}";
}

std::string ActionDebug::VisitPrintValue(PrintValue* pv) {
  return GetActionName(pv->symbol) + "{" + pretty_.VisitPrintValue(pv) + "}";
}

std::string ActionDebug::VisitDecrement(Decrement* d) {
  return GetActionName(d->symbol) + "{" + pretty_.VisitDecrement(d) + "}";
}

std::string ActionDebug::VisitIncrement(Increment* i) {
  return GetActionName(i->symbol) + "{" + pretty_.VisitIncrement(i) + "}";
}

std::string ActionDebug::VisitAssign(Assign* a) {
  return GetActionName(a->symbol) + "{" + pretty_.VisitAssign(a) + "}";
}

std::string ActionDebug::VisitAdd(Add* a) {
  return GetActionName(a->symbol) + "{" + pretty_.VisitAdd(a) + "}";
}

std::string ActionDebug::VisitMultiply(Multiply* m) {
  return GetActionName(m->symbol) + "{" + pretty_.VisitMultiply(m) + "}";
}

std::string ActionDebug::VisitSquigglyMoveTo(SquigglyMoveTo* s) {
  return GetActionName(s->symbol) + "{" + pretty_.VisitSquigglyMoveTo(s) + "}";
}



std::string GetConditionName(std::string symbol) {
  return ConditionBuilder::Instance().ConditionName(symbol);
}


std::string ConditionDebug::VisitEquals(Equals* e) {
  return GetConditionName(e->symbol) + "{" + pretty_.VisitEquals(e) + "}";
}

std::string ConditionDebug::VisitGreater(Greater* g) {
  return GetConditionName(g->symbol) + "{" + pretty_.VisitGreater(g) + "}";
}

std::string ConditionDebug::VisitLess(Less* l) {
  return GetConditionName(l->symbol) + "{" + pretty_.VisitLess(l) + "}";
}

std::string ConditionDebug::VisitDivides(Divides* d) {
  return GetConditionName(d->symbol) + "{" + pretty_.VisitDivides(d) + "}";
}

std::string ConditionDebug::VisitEmpty(Empty* e) {
  return GetConditionName(e->symbol) + "{" + pretty_.VisitEmpty(e) + "}";
}

