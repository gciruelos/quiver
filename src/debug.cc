#include "debug.h"

#include <string>
#include <sstream>

#include "utils.h"

std::string IndicatorName(char indicator) {
  switch (indicator) {
    case CURRENT_NODE:
      return "current node";
    case NEXT_NODE:
      return "next node";
    default:
      return "accumulator";
  }
}

std::string GetActionName(std::string symbol) {
  return ActionBuilder::Instance().ActionName(symbol);
}

std::string ActionDebug::VisitNothing(Nothing* nothing) {
  std::stringstream buffer;
  buffer << GetActionName(nothing->symbol);
  return buffer.str();
}

std::string ActionDebug::VisitPrint(Print* print) {
  std::stringstream buffer;
  buffer << GetActionName(print->symbol)
         << "{" << UndoParseString(print->print) << "}";
  return buffer.str();
}

std::string ActionDebug::VisitPrintValue(PrintValue* print_value) {
  std::stringstream buffer;
  buffer << GetActionName(print_value->symbol)
         << "{" << IndicatorName(print_value->aff) << "}";
  return buffer.str();
}

std::string ActionDebug::VisitDecrement(Decrement* decrement) {
  std::stringstream buffer;
  buffer << GetActionName(decrement->symbol)
         << "{" << IndicatorName(decrement->aff) << "}";
  return buffer.str();
}

std::string ActionDebug::VisitIncrement(Increment* increment) {
  std::stringstream buffer;
  buffer << GetActionName(increment->symbol)
         << "{" << IndicatorName(increment->aff) << "}";
  return buffer.str();
}

std::string ActionDebug::VisitAssign(Assign* assign) {
  std::stringstream buffer;
  buffer << GetActionName(assign->symbol) << "{"
         << IndicatorName(assign->aff) << " = " << assign->new_val << "}";
  return buffer.str();
}

std::string ActionDebug::VisitSquigglyMoveTo(SquigglyMoveTo* smt) {
  std::stringstream buffer;
  buffer << GetActionName(smt->symbol);
  return buffer.str();
}

std::string GetConditionName(std::string symbol) {
  return ConditionBuilder::Instance().ConditionName(symbol);
}


std::string ConditionDebugString(std::string symbol, uint64_t value,
    bool arg_is_value, bool rotate_operands) {
  std::stringstream buffer;
  buffer << GetConditionName(symbol)
         << "{"
         << (arg_is_value ? std::to_string(value) : "curr")
         << (rotate_operands ? "(@)" : "")
         << "}";
  return buffer.str();
}


std::string ConditionDebug::VisitEquals(Equals* eq) {
  return ConditionDebugString(
      eq->symbol, eq->value_, eq->arg_is_value, eq->rotate_operands);
}

std::string ConditionDebug::VisitGreater(Greater* gr) {
  return ConditionDebugString(
      gr->symbol, gr->value_, gr->arg_is_value, gr->rotate_operands);
}

std::string ConditionDebug::VisitLess(Less* le) {
  return ConditionDebugString(
      le->symbol, le->value_, le->arg_is_value, le->rotate_operands);
}

std::string ConditionDebug::VisitDivides(Divides* di) {
  return ConditionDebugString(
      di->symbol, di->value_, di->arg_is_value, di->rotate_operands);
}


std::string ConditionDebug::VisitEmpty(Empty* em) {
  std::stringstream buffer;
  buffer << GetConditionName(em->symbol);
  return buffer.str();
}
