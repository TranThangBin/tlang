#include "parser/ast.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <stdexcept>

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
    return std::make_shared<NumberValue>((int)leftHand->GetValue() %
                                         (int)rightHand->GetValue());

  default:
    throw std::runtime_error("Unexpected binary operator " +
                             BinaryOperatorToString(op));
  }
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateBinaryOperation(std::shared_ptr<RuntimeValue> left,
                                     std::shared_ptr<RuntimeValue> right,
                                     BinaryOperator op) {
  DataType leftDt = left->DataTypeID();

  switch (leftDt) {
  case DataType::Number:
    return evaluateNumericOperation(
        std::static_pointer_cast<NumberValue>(left),
        std::static_pointer_cast<NumberValue>(right), op);

  default:
    throw std::runtime_error("No binary operator " +
                             BinaryOperatorToString(op) + " for " +
                             DataTypeToString(leftDt));
  }
}
