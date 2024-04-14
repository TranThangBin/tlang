#include "parser/ast.h"
#include "runtime/environment.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

std::shared_ptr<RuntimeValue> Interpreter::evaluateAssignmentExpr(
    std::shared_ptr<AssignmentExprNode> assignmentExprNode,
    std::shared_ptr<Environment> environment) {

  std::shared_ptr<IdentifierNode> identifier =
      std::static_pointer_cast<IdentifierNode>(
          assignmentExprNode->GetAssignee());

  return environment->AssignVariable(
      identifier->GetSymbol(),
      evaluate(assignmentExprNode->GetValue(), environment));
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateIdentifier(std::shared_ptr<IdentifierNode> identifierNode,
                                std::shared_ptr<Environment> environment) {
  return environment->LookUpVar(identifierNode->GetSymbol());
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateBinaryExpr(std::shared_ptr<BinaryExprNode> binaryExprNode,
                                std::shared_ptr<Environment> environment) {
  std::shared_ptr<RuntimeValue> left =
      evaluate(binaryExprNode->GetLeft(), environment);
  std::shared_ptr<RuntimeValue> right =
      evaluate(binaryExprNode->GetRight(), environment);

  DataType leftDt = left->DataTypeID();

  if (leftDt != right->DataTypeID()) {
    throw std::runtime_error("Mismatched type in the binary expression");
  }

  switch (leftDt) {
  case DataType::Number:
    return evaluateNumberOperation(std::static_pointer_cast<NumberValue>(left),
                                   std::static_pointer_cast<NumberValue>(right),
                                   binaryExprNode->GetOperator());
  default:
    std::stringstream ss;
    ss << "No operator " << binaryExprNode->GetOperator() << " for "
       << (int)leftDt;
    throw std::runtime_error(ss.str());
  }
}
