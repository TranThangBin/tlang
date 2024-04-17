#include "parser/ast.h"
#include "runtime/environment.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

std::shared_ptr<RuntimeValue>
Interpreter::evaluateIdentifier(std::unique_ptr<IdentifierNode> identifierNode,
                                std::unique_ptr<Environment> &environment) {
  return environment->LookUpVar(identifierNode->GetSymbol());
}

std::shared_ptr<RuntimeValue> Interpreter::evaluateAssignmentExpr(
    std::unique_ptr<AssignmentExprNode> assignmentExprNode,
    std::unique_ptr<Environment> &environment) {

  std::string varname;

  NodeType assigneeKind = assignmentExprNode->GetAssignee()->Kind();

  switch (assigneeKind) {
  case NodeType::Identifier:
    varname = std::unique_ptr<IdentifierNode>(
                  static_cast<IdentifierNode *>(
                      assignmentExprNode->GetAssignee().release()))
                  ->GetSymbol();
    break;
  default:
    std::stringstream ss;
    ss << "Unexpected node " << (int)assigneeKind;
    throw std::runtime_error(ss.str());
  }

  std::shared_ptr<RuntimeValue> value =
      evaluate(std::move(assignmentExprNode->GetValue()), environment);

  return environment->AssignVariable(varname, std::move(value));
}

std::shared_ptr<RuntimeValue> Interpreter::evaluateBinaryAssignmentExpr(
    std::unique_ptr<BinaryAssignmentExprNode> binaryAssignmentExprNode,
    std::unique_ptr<Environment> &environment) {

  std::string varname;

  NodeType assigneeKind = binaryAssignmentExprNode->GetAssignee()->Kind();

  switch (assigneeKind) {
  case NodeType::Identifier:
    varname = std::unique_ptr<IdentifierNode>(
                  static_cast<IdentifierNode *>(
                      binaryAssignmentExprNode->GetAssignee().release()))
                  ->GetSymbol();
    break;
  default:
    std::stringstream ss;
    ss << "Unexpected node " << (int)assigneeKind;
    throw std::runtime_error(ss.str());
  }

  std::shared_ptr<RuntimeValue> value =
      evaluate(std::move(binaryAssignmentExprNode->GetValue()), environment);

  std::string op = binaryAssignmentExprNode->GetOperator();
  op.erase(op.length() - 1);

  return environment->AssignVariable(
      varname,
      evaluateBinaryOperation(environment->LookUpVar(varname), value, op));
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateBinaryExpr(std::unique_ptr<BinaryExprNode> binaryExprNode,
                                std::unique_ptr<Environment> &environment) {

  std::shared_ptr<RuntimeValue> left =
      evaluate(std::move(binaryExprNode->GetLeft()), environment);
  std::shared_ptr<RuntimeValue> right =
      evaluate(std::move(binaryExprNode->GetRight()), environment);

  if (left->DataTypeID() != right->DataTypeID()) {
    throw std::runtime_error("Mismatched type in the binary expression");
  }

  return evaluateBinaryOperation(left, right, binaryExprNode->GetOperator());
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateUnaryExpr(std::unique_ptr<UnaryExprNode> unaryExpr,
                               std::unique_ptr<Environment> &environment) {
  std::shared_ptr<RuntimeValue> value =
      evaluate(std::move(unaryExpr->GetValue()), environment);

  return evaluateUnaryOperation(value, unaryExpr->GetOperator());
}
