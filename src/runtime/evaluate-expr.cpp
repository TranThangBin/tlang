#include "parser/ast.h"
#include "runtime/environment.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

std::shared_ptr<RuntimeValue> Interpreter::evaluateAssignmentExpr(
    std::unique_ptr<AssignmentExprNode> assignmentExprNode,
    std::unique_ptr<Environment> &environment) {

  std::unique_ptr<IdentifierNode> identifier =
      std::unique_ptr<IdentifierNode>(static_cast<IdentifierNode *>(
          assignmentExprNode->GetAssignee().release()));

  return environment->AssignVariable(
      identifier->GetSymbol(),
      evaluate(std::move(assignmentExprNode->GetValue()), environment));
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateIdentifier(std::unique_ptr<IdentifierNode> identifierNode,
                                std::unique_ptr<Environment> &environment) {
  return environment->LookUpVar(identifierNode->GetSymbol());
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateBinaryExpr(std::unique_ptr<BinaryExprNode> binaryExprNode,
                                std::unique_ptr<Environment> &environment) {

  std::shared_ptr<RuntimeValue> left =
      evaluate(std::move(binaryExprNode->GetLeft()), environment);
  std::shared_ptr<RuntimeValue> right =
      evaluate(std::move(binaryExprNode->GetRight()), environment);

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
