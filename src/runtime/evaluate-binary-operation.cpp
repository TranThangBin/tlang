#include "parser/ast.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <stdexcept>

std::runtime_error GetError(BinaryOperator op) {
  throw std::runtime_error("Unexpected binary operator " +
                           BinaryOperatorToString(op));
}

std::shared_ptr<RuntimeValue>
evaluateNumericOperation(std::shared_ptr<NumberValue> leftHand,
                         std::shared_ptr<NumberValue> rightHand,
                         BinaryOperator op) {
  switch (op) {
  case BinaryOperator::Addition:
    return std::make_shared<NumberValue>(leftHand->GetValue() +
                                         rightHand->GetValue());

  case BinaryOperator::Subtraction:
    return std::make_shared<NumberValue>(leftHand->GetValue() -
                                         rightHand->GetValue());

  case BinaryOperator::Multiplication:
    return std::make_shared<NumberValue>(leftHand->GetValue() *
                                         rightHand->GetValue());

  case BinaryOperator::Division:
    if (rightHand->GetValue() == 0) {
      throw std::runtime_error("Divide by 0 error");
    }
    return std::make_shared<NumberValue>(leftHand->GetValue() /
                                         rightHand->GetValue());

  case BinaryOperator::Modulo:
    if ((int)rightHand->GetValue() == 0) {
      throw std::runtime_error("Modulo by 0 error");
    }
    return std::make_shared<NumberValue>((int)leftHand->GetValue() %
                                         (int)rightHand->GetValue());

  case BinaryOperator::Equality:
    return std::make_shared<BooleanValue>(leftHand->GetValue() ==
                                          rightHand->GetValue());

  case BinaryOperator::InEquality:
    return std::make_shared<BooleanValue>(leftHand->GetValue() !=
                                          rightHand->GetValue());

  case BinaryOperator::Greater:
    return std::make_shared<BooleanValue>(leftHand->GetValue() >
                                          rightHand->GetValue());

  case BinaryOperator::Lower:
    return std::make_shared<BooleanValue>(leftHand->GetValue() <
                                          rightHand->GetValue());

  case BinaryOperator::GreaterEqual:
    return std::make_shared<BooleanValue>(leftHand->GetValue() >=
                                          rightHand->GetValue());

  case BinaryOperator::LowerEqual:
    return std::make_shared<BooleanValue>(leftHand->GetValue() <=
                                          rightHand->GetValue());

  default:
    throw GetError(op);
  }
}

std::shared_ptr<RuntimeValue>
evaluateStringOperation(std::shared_ptr<StringValue> leftHand,
                        std::shared_ptr<StringValue> rightHand,
                        BinaryOperator op) {
  switch (op) {
  case BinaryOperator::Addition:
    return std::make_shared<StringValue>(leftHand->GetValue() +
                                         rightHand->GetValue());

  case BinaryOperator::Equality:
    return std::make_shared<BooleanValue>(leftHand->GetValue() ==
                                          rightHand->GetValue());

  case BinaryOperator::InEquality:
    return std::make_shared<BooleanValue>(leftHand->GetValue() !=
                                          rightHand->GetValue());

  default:
    throw GetError(op);
  }
}

std::shared_ptr<RuntimeValue>
evaluateBooleanOperation(std::shared_ptr<BooleanValue> leftHand,
                         std::shared_ptr<BooleanValue> rightHand,
                         BinaryOperator op) {
  switch (op) {
  case BinaryOperator::Equality:
    return std::make_shared<BooleanValue>(leftHand->GetValue() ==
                                          rightHand->GetValue());

  case BinaryOperator::InEquality:
    return std::make_shared<BooleanValue>(leftHand->GetValue() !=
                                          rightHand->GetValue());

  case BinaryOperator::And:
    return std::make_shared<BooleanValue>(leftHand->GetValue() &&
                                          rightHand->GetValue());

  case BinaryOperator::Or:
    return std::make_shared<BooleanValue>(leftHand->GetValue() ||
                                          rightHand->GetValue());

  default:
    throw GetError(op);
  }
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateBinaryOperation(std::shared_ptr<RuntimeValue> left,
                                     std::shared_ptr<RuntimeValue> right,
                                     BinaryOperator op) {

  DataType leftDt = left->DataTypeID();
  DataType rightDt = right->DataTypeID();

  if (leftDt == DataType::Null || rightDt == DataType::Null) {
    if (op == BinaryOperator::Equality) {

      return std::make_shared<BooleanValue>(leftDt == rightDt);
    } else if (op == BinaryOperator::InEquality) {

      return std::make_shared<BooleanValue>(leftDt != rightDt);
    }
  }

  if (leftDt != rightDt) {

    throw std::runtime_error("Mismatched type in the binary expression");
  }

  switch (leftDt) {
  case DataType::Number:
    return evaluateNumericOperation(
        std::static_pointer_cast<NumberValue>(left),
        std::static_pointer_cast<NumberValue>(right), op);

  case DataType::String:
    return evaluateStringOperation(std::static_pointer_cast<StringValue>(left),
                                   std::static_pointer_cast<StringValue>(right),
                                   op);

  case DataType::Boolean:
    return evaluateBooleanOperation(
        std::static_pointer_cast<BooleanValue>(left),
        std::static_pointer_cast<BooleanValue>(right), op);

  default:
    throw std::runtime_error("No binary operator " +
                             BinaryOperatorToString(op) + " for " +
                             DataTypeToString(leftDt));
  }
}
