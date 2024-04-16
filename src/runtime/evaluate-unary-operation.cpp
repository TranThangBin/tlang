#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <string>

std::shared_ptr<RuntimeValue>
evaluateNumericOperation(std::shared_ptr<NumberValue> value, std::string op) {
  if (op == "+") {
    return value;
  }

  if (op == "-") {
    value->SetValue(-value->GetValue());
    return value;
  }

  throw std::runtime_error("Unexpected operator " + op);
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateUnaryOperation(std::shared_ptr<RuntimeValue> value,
                                    std::string op) {
  DataType dt = value->DataTypeID();
  switch (dt) {
  case DataType::Number:
    return evaluateNumericOperation(
        std::static_pointer_cast<NumberValue>(value), op);

  default:
    std::stringstream ss;
    ss << "No operator " << op << " for " << (int)dt;
    throw std::runtime_error(ss.str());
  }
}
