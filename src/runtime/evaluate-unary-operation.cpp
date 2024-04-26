#include "parser/ast.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <stdexcept>

std::runtime_error GetError(UnaryOperator op) {
  throw std::runtime_error("Unexpected unary operator " +
                           UnaryOperatorToString(op));
}

std::shared_ptr<RuntimeValue>
evaluateNumericOperation(std::shared_ptr<NumberValue> value, UnaryOperator op) {
  switch (op) {
  case UnaryOperator::Plus:
    return value;

  case UnaryOperator::Minus:
    return std::make_shared<NumberValue>(-value->GetValue());

  default:
    throw GetError(op);
  }
}

std::shared_ptr<RuntimeValue>
evaluateBooleanOperation(std::shared_ptr<BooleanValue> value,
                         UnaryOperator op) {
  if (op == UnaryOperator::Not) {
    return std::make_shared<BooleanValue>(!value->GetValue());
  }

  throw GetError(op);
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateUnaryOperation(std::shared_ptr<RuntimeValue> value,
                                    UnaryOperator op) {
  DataType dt = value->DataTypeID();
  switch (dt) {
  case DataType::Number:
    return evaluateNumericOperation(
        std::static_pointer_cast<NumberValue>(value), op);

  case DataType::Boolean:
    return evaluateBooleanOperation(
        std::static_pointer_cast<BooleanValue>(value), op);

  default:
    throw std::runtime_error("No unary operator " + UnaryOperatorToString(op) +
                             " for " + DataTypeToString(dt));
  }
}
