#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include "runtime/value.h"
#include <memory>
#include <stdexcept>

std::shared_ptr<RuntimeValue>
Interpreter::evaluateNumberOperation(std::shared_ptr<NumberValue> leftHand,
                                     std::shared_ptr<NumberValue> rightHand,
                                     std::string op) {
  if (op == "+") {
    return std::make_shared<NumberValue>(leftHand->GetValue() +
                                         rightHand->GetValue());
  } else if (op == "-") {
    return std::make_shared<NumberValue>(leftHand->GetValue() -
                                         rightHand->GetValue());
  } else if (op == "*") {
    return std::make_shared<NumberValue>(leftHand->GetValue() *
                                         rightHand->GetValue());
  } else if (op == "/") {
    float rightValue = rightHand->GetValue();
    if (rightValue == 0) {
      throw std::runtime_error("Divide by 0 error");
    }
    return std::make_shared<NumberValue>(leftHand->GetValue() /
                                         rightHand->GetValue());
  } else if (op == "%") {
    return std::make_shared<NumberValue>((int)leftHand->GetValue() %
                                         (int)rightHand->GetValue());
  }
  throw std::runtime_error("Unexpected operator " + op);
}
