#include "parser/ast.h"
#include "runtime/environment.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <bits/types/error_t.h>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

std::shared_ptr<RuntimeValue>
Interpreter::evaluateIdentifier(std::unique_ptr<IdentifierNode> ident,
                                std::unique_ptr<Environment> &env) {
  return env->LookUpVar(ident->GetSymbol());
}

std::shared_ptr<RuntimeValue> Interpreter::evaluateAssignmentExpr(
    std::unique_ptr<AssignmentExprNode> assignExpr,
    std::unique_ptr<Environment> &env) {

  NodeType assigneeKind = assignExpr->GetAssignee()->Kind();

  std::shared_ptr<RuntimeValue> assignValue =
      evaluate(std::move(assignExpr->GetValue()), env);

  switch (assigneeKind) {

  case NodeType::Identifier: {

    std::string varname =
        std::unique_ptr<IdentifierNode>(
            static_cast<IdentifierNode *>(assignExpr->GetAssignee().release()))
            ->GetSymbol();

    return env->AssignVariable(varname, assignValue);
  }

  case NodeType::IndexingExpression: {

    auto indexingExpression = std::unique_ptr<IndexingExpressionNode>(
        static_cast<IndexingExpressionNode *>(
            assignExpr->GetAssignee().release()));

    std::shared_ptr<RuntimeValue> accessorValue =
        evaluate(std::move(indexingExpression->GetAccessor()), env);

    std::shared_ptr<RuntimeValue> indexValue =
        evaluate(std::move(indexingExpression->GetIndex()), env);

    DataType accessorType = accessorValue->DataTypeID();

    switch (accessorType) {

    case DataType::Object:
    case DataType::Array:
      return std::static_pointer_cast<IndexAbleValue>(accessorValue)
          ->SetIndexedValue(indexValue, assignValue);

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
    std::unique_ptr<BinaryAssignmentExprNode> binAssignExpr,
    std::unique_ptr<Environment> &env) {

  std::unique_ptr<Expr> assignee = std::move(binAssignExpr->GetAssignee());

  BinaryOperator op = binAssignExpr->GetOperator();

  std::shared_ptr<RuntimeValue> value =
      evaluate(std::move(binAssignExpr->GetValue()), env);

  switch (assignee->Kind()) {

  case NodeType::Identifier: {

    auto ident = std::unique_ptr<IdentifierNode>(
        static_cast<IdentifierNode *>(assignee.release()));

    std::string varname = ident->GetSymbol();

    std::shared_ptr<RuntimeValue> varVal = evaluate(std::move(ident), env);

    return env->AssignVariable(varname,
                               evaluateBinaryOperation(varVal, value, op));
  }

  case NodeType::IndexingExpression: {

    auto indexingExpr = std::unique_ptr<IndexingExpressionNode>(
        static_cast<IndexingExpressionNode *>(assignee.release()));

    std::shared_ptr<RuntimeValue> accessorValue =
        evaluate(std::move(indexingExpr->GetAccessor()), env);

    std::shared_ptr<RuntimeValue> indexValue =
        evaluate(std::move(indexingExpr->GetIndex()), env);

    DataType accessorType = accessorValue->DataTypeID();

    switch (accessorType) {

    case DataType::Object:
    case DataType::Array: {

      auto indexAbleValue =
          std::static_pointer_cast<IndexAbleValue>(accessorValue);

      return indexAbleValue->SetIndexedValue(
          indexValue,
          evaluateBinaryOperation(indexAbleValue->GetIndexedValue(indexValue),
                                  value, op));
    }

    default:
      throw std::runtime_error(DataTypeToString(accessorType) +
                               " is not indexable");
    }
  }

  default:
    throw std::runtime_error("Unexpected node " +
                             NodeTypeToString(assignee->Kind()));
  }
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

  std::map<std::string, std::shared_ptr<RuntimeValue>> objectProperties;

  for (auto it = properties.begin(); it != properties.end(); it++) {
    objectProperties.insert(
        {it->first, evaluate(std::move(it->second), environment)});
  }

  return std::make_shared<ObjectValue>(std::move(objectProperties));
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateArrayExpr(std::unique_ptr<ArrayExprNode> arr,
                               std::unique_ptr<Environment> &env) {

  std::vector<std::unique_ptr<Expr>> values = std::move(arr->GetValues());

  std::vector<std::shared_ptr<RuntimeValue>> arrayValues;

  int valueCount = values.size();

  for (int i = 0; i < valueCount; i++) {
    arrayValues.push_back(evaluate(std::move(values[i]), env));
  }

  return std::make_shared<ArrayValue>(std::move(arrayValues));
}

std::shared_ptr<RuntimeValue> Interpreter::evaluateIndexingExpr(
    std::unique_ptr<IndexingExpressionNode> indexingExprNode,
    std::unique_ptr<Environment> &environment) {

  std::shared_ptr<RuntimeValue> accessorValue =
      evaluate(std::move(indexingExprNode->GetAccessor()), environment);

  std::shared_ptr<RuntimeValue> indexValue =
      evaluate(std::move(indexingExprNode->GetIndex()), environment);

  DataType accessorType = accessorValue->DataTypeID();

  switch (accessorType) {

  case DataType::Object:
  case DataType::Array:

    return std::static_pointer_cast<IndexAbleValue>(accessorValue)
        ->GetIndexedValue(indexValue);

  default:
    throw std::runtime_error(DataTypeToString(accessorType) +
                             " is not indexable");
  }
}
