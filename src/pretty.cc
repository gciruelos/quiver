#include "pretty.h"

#include <sstream>
#include <utility>

#include "utils.h"

#define STRING_CURRENT "current_node"
#define STRING_NEXT "next_node"
#define STRING_ACCUMULATOR "accumulator"

std::string IndicatorName(char indicator) {
  switch (indicator) {
    case CURRENT_NODE:
      return STRING_CURRENT;
    case NEXT_NODE:
      return STRING_NEXT;
    default:
      return STRING_ACCUMULATOR;
  }
}

std::pair<std::string, std::string> PrettyActionOperands(
    AffectedValue aff, bool arg_is_value, uint64_t value) {
  std::string fst_op, snd_op;
  if (aff == ACCUMULATOR) {
    fst_op = STRING_ACCUMULATOR;
    if (arg_is_value) {
      snd_op = std::to_string(value);
    } else {
      snd_op = STRING_CURRENT;
    }
  } else {
    if (aff == CURRENT_NODE) {
      fst_op = STRING_CURRENT;
    } else {
      fst_op = STRING_NEXT;
    }
    if (arg_is_value) {
      snd_op = std::to_string(value);
    } else {
      snd_op = STRING_ACCUMULATOR;
    }
  }
  return std::make_pair(fst_op, snd_op);
}


std::string ActionPretty::VisitNothing(
    Nothing* nothing __attribute__((unused))) {
  return "<nothing>";
}

std::string ActionPretty::VisitPrint(Print* print) {
  return "print('" + UndoParseString(print->print) + "')";
}

std::string ActionPretty::VisitPrintValue(PrintValue* print_value) {
  return "print(" + IndicatorName(print_value->aff) + ")";
}

std::string ActionPretty::VisitDecrement(Decrement* decrement) {
  return IndicatorName(decrement->aff) + "--";
}

std::string ActionPretty::VisitIncrement(Increment* increment) {
  return IndicatorName(increment->aff) + "++";
}

std::string ActionPretty::VisitAssign(Assign* assign) {
  return IndicatorName(assign->aff) + " = " + std::to_string(assign->new_val);
}

std::string ActionPretty::VisitAdd(Add* add) {
  std::pair<std::string, std::string> operands(PrettyActionOperands(
        add->aff, add->arg_is_value, add->value));
  return operands.first + " += " + operands.second;
}

std::string ActionPretty::VisitMultiply(Multiply* mul) {
  std::pair<std::string, std::string> operands(PrettyActionOperands(
        mul->aff, mul->arg_is_value, mul->value));
  return operands.first + " *= " + operands.second;
}

std::string ActionPretty::VisitSquigglyMoveTo(
    SquigglyMoveTo* smt __attribute__((unused))) {
  return std::string(STRING_NEXT) + " = " + std::string(STRING_CURRENT);
}


std::pair<std::string, std::string> PrettyOperands(
    bool rotate_operands, bool arg_is_value, uint64_t value) {
  std::string fst_op, snd_op;
  if (!rotate_operands) {
    fst_op = STRING_ACCUMULATOR;
    if (arg_is_value) {
      snd_op = std::to_string(value);
    } else {
      snd_op = STRING_CURRENT;
    }
  } else {
    snd_op = STRING_ACCUMULATOR;
    if (arg_is_value) {
      fst_op = std::to_string(value);
    } else {
      fst_op = STRING_CURRENT;
    }
  }
  return std::make_pair(fst_op, snd_op);
}


std::string ConditionPretty::VisitEquals(Equals* eq) {
  std::pair<std::string, std::string> operands(PrettyOperands(
        eq->rotate_operands, eq->arg_is_value, eq->value_));
  return operands.first + " == " + operands.second;
}

std::string ConditionPretty::VisitGreater(Greater* gr) {
  std::pair<std::string, std::string> operands(PrettyOperands(
        gr->rotate_operands, gr->arg_is_value, gr->value_));
  return operands.first + " > " + operands.second;
}

std::string ConditionPretty::VisitLess(Less* le) {
  std::pair<std::string, std::string> operands(PrettyOperands(
        le->rotate_operands, le->arg_is_value, le->value_));
  return operands.first + " < " + operands.second;
}

std::string ConditionPretty::VisitDivides(Divides* di) {
  std::pair<std::string, std::string> operands(PrettyOperands(
        di->rotate_operands, di->arg_is_value, di->value_));
  return operands.first + " | " + operands.second;
}


std::string ConditionPretty::VisitEmpty(Empty* em __attribute__((unused))) {
  return "<empty>";
}
