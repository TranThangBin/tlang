#include "parser/ast.h"
#include "runtime/environment.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <map>
#include <memory>
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

  NodeType assigneeKind = assignmentExprNode->GetAssignee()->Kind();

  switch (assigneeKind) {
  case NodeType::Identifier: {
    std::string varname = std::unique_ptr<IdentifierNode>(
                              static_cast<IdentifierNode *>(
                                  assignmentExprNode->GetAssignee().release()))
                              ->GetSymbol();

    std::shared_ptr<RuntimeValue> value =
        evaluate(std::move(assignmentExprNode->GetValue()), environment);

    return environment->AssignVariable(varname, std::move(value));
  }

  case NodeType::IndexingExpression: {
    std::unique_ptr<IndexingExpressionNode> indexingExpression =
        std::unique_ptr<IndexingExpressionNode>(
            static_cast<IndexingExpressionNode *>(
                assignmentExprNode->GetAssignee().release()));

    std::shared_ptr<RuntimeValue> accessor =
        evaluate(std::move(indexingExpression->GetAccessor()), environment);

    DataType accessorType = accessor->DataTypeID();

    switch (accessorType) {
    case DataType::Object: {
      std::shared_ptr<RuntimeValue> keyValue =
          evaluate(std::move(indexingExpression->GetIndex()), environment);

      if (keyValue->DataTypeID() != DataType::String) {
        throw std::runtime_error("Invalid index for object");
      }

      return std::static_pointer_cast<ObjectValue>(accessor)->SetProperty(
          std::static_pointer_cast<StringValue>(keyValue)->GetValue(),
          evaluate(std::move(assignmentExprNode->GetValue()), environment));
    }

    default:
      throw std::runtime_error(DataTypeToString(accessorType) +
                               " is not indexable");
    }
  }

  default:
    throw std::runtime_error("Unexpected node " +
                             NodeTypeToString(assigneeKind));
  }
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
    throw std::runtime_error("Unexpected node " +
                             NodeTypeToString(assigneeKind));
  }

  std::shared_ptr<RuntimeValue> value =
      evaluate(std::move(binaryAssignmentExprNode->GetValue()), environment);

  BinaryOperator op = binaryAssignmentExprNode->GetOperator();

  std::shared_ptr<RuntimeValue> varVal = environment->LookUpVar(varname);

  return environment->AssignVariable(
      varname, evaluateBinaryOperation(varVal, value, op));
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

std::shared_ptr<RuntimeValue> Interpreter::evaluateObjectLiteral(
    std::unique_ptr<ObjectLiteralNode> objectLiteralNode,
    std::unique_ptr<Environment> &environment) {

  std::map<std::string, std::unique_ptr<Expr>> properties =
      std::move(objectLiteralNode->GetProperties());

  auto valueProperties = std::map<std::string, std::shared_ptr<RuntimeValue>>();

  for (auto it = properties.begin(); it != properties.end(); it++) {
    valueProperties.insert(
        {it->first, evaluate(std::move(it->second), environment)});
  }

  return std::make_shared<ObjectValue>(std::move(valueProperties));
}

std::shared_ptr<RuntimeValue> Interpreter::evaluateIndexingExpr(
    std::unique_ptr<IndexingExpressionNode> indexingExprNode,
    std::unique_ptr<Environment> &environment) {
  std::shared_ptr<RuntimeValue> accessor =
      evaluate(std::move(indexingExprNode->GetAccessor()), environment);

  DataType accessorType = accessor->DataTypeID();

  switch (accessorType) {
  case DataType::Object: {
    std::shared_ptr<RuntimeValue> keyValue =
        evaluate(std::move(indexingExprNode->GetIndex()), environment);

    if (keyValue->DataTypeID() != DataType::String) {
      throw std::runtime_error("Invalid index for object");
    }

    return std::static_pointer_cast<ObjectValue>(accessor)->GetProperty(
        std::static_pointer_cast<StringValue>(keyValue)->GetValue());
  }

  default:
    throw std::runtime_error(DataTypeToString(accessorType) +
                             " is not indexable");
  }
}
