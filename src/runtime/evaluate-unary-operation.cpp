#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <stdexcept>
#include <string>

std::runtime_error GetError(std::string op) {
  throw std::runtime_error("Unexpected operator " + op);
}

std::shared_ptr<RuntimeValue>
evaluateNumericOperation(std::shared_ptr<NumberValue> value, std::string op) {
  if (op == "+") {
    return value;
  }

  if (op == "-") {
    return std::make_shared<NumberValue>(-value->GetValue());
  }

  throw GetError(op);
}

std::shared_ptr<RuntimeValue>
evaluateBooleanOperation(std::shared_ptr<BooleanValue> value, std::string op) {
  if (op == "!") {
    return std::make_shared<BooleanValue>(!value->GetValue());
  }

  throw GetError(op);
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateUnaryOperation(std::shared_ptr<RuntimeValue> value,
                                    std::string op) {
  DataType dt = value->DataTypeID();
  switch (dt) {
  case DataType::Number:
    return evaluateNumericOperation(
        std::static_pointer_cast<NumberValue>(value), op);

  case DataType::Boolean:
    return evaluateBooleanOperation(
        std::static_pointer_cast<BooleanValue>(value), op);

  default:
    std::stringstream ss;
    ss << "No operator " << op << " for " << (int)dt;
    throw std::runtime_error(ss.str());
  }
}
