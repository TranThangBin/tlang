#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <stdexcept>
#include <string>

std::shared_ptr<RuntimeValue>
evaluateNumericOperation(std::shared_ptr<NumberValue> leftHand,
                         std::shared_ptr<NumberValue> rightHand,
                         std::string op) {
  if (op == "+") {
    return std::make_shared<NumberValue>(leftHand->GetValue() +
                                         rightHand->GetValue());
  }

  if (op == "-") {
    return std::make_shared<NumberValue>(leftHand->GetValue() -
                                         rightHand->GetValue());
  }

  if (op == "*") {
    return std::make_shared<NumberValue>(leftHand->GetValue() *
                                         rightHand->GetValue());
  }

  if (op == "/") {
    float rightValue = rightHand->GetValue();
    if (rightValue == 0) {
      throw std::runtime_error("Divide by 0 error");
    }
    return std::make_shared<NumberValue>(leftHand->GetValue() /
                                         rightHand->GetValue());
  }

  if (op == "%") {
    return std::make_shared<NumberValue>((int)leftHand->GetValue() %
                                         (int)rightHand->GetValue());
  }

  throw std::runtime_error("Unexpected operator " + op);
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateBinaryOperation(std::shared_ptr<RuntimeValue> left,
                                     std::shared_ptr<RuntimeValue> right,
                                     std::string op) {
  DataType leftDt = left->DataTypeID();

  switch (leftDt) {
  case DataType::Number:
    return evaluateNumericOperation(
        std::static_pointer_cast<NumberValue>(left),
        std::static_pointer_cast<NumberValue>(right), op);

  default:
    std::stringstream ss;
    ss << "No operator " << op << " for " << (int)leftDt;
    throw std::runtime_error(ss.str());
  }
}
