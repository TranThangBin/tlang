#include "datastructure/array-list.h"
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

std::shared_ptr<RuntimeValue>
Interpreter::evaluateIdentifier(std::unique_ptr<IdentifierNode> &ident,
                                std::unique_ptr<Environment> &env) {
  return env->LookUpVar(ident->GetSymbol());
}

std::shared_ptr<RuntimeValue> Interpreter::evaluateAssignmentExpr(
    std::unique_ptr<AssignmentExprNode> &assignExpr,
    std::unique_ptr<Environment> &env) {

  std::unique_ptr<Expr> &assignee = assignExpr->GetAssignee();

  NodeType assigneeKind = assignee->Kind();

  std::shared_ptr<RuntimeValue> assignValue =
      evaluateExpr(assignExpr->GetValue(), env);

  switch (assigneeKind) {

  case NodeType::Identifier: {

    auto identifier = std::unique_ptr<IdentifierNode>(
        static_cast<IdentifierNode *>(assignee.release()));

    std::string varname = identifier->GetSymbol();

    assignee = std::move(identifier);

    return env->AssignVariable(varname, assignValue);
  }

  case NodeType::IndexingExpression: {

    auto indexingExpr = std::unique_ptr<IndexingExpressionNode>(
        static_cast<IndexingExpressionNode *>(assignee.release()));

    std::shared_ptr<RuntimeValue> accessorValue =
        evaluateExpr(indexingExpr->GetAccessor(), env);

    std::shared_ptr<RuntimeValue> indexValue =
        evaluateExpr(indexingExpr->GetIndex(), env);

    assignee = std::move(indexingExpr);

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
    std::unique_ptr<BinaryAssignmentExprNode> &binAssignExpr,
    std::unique_ptr<Environment> &env) {

  std::unique_ptr<Expr> &assignee = binAssignExpr->GetAssignee();

  BinaryOperator op = binAssignExpr->GetOperator();

  std::shared_ptr<RuntimeValue> value =
      evaluateExpr(binAssignExpr->GetValue(), env);

  switch (assignee->Kind()) {

  case NodeType::Identifier: {

    auto ident = std::unique_ptr<IdentifierNode>(
        static_cast<IdentifierNode *>(assignee.release()));

    std::string varname = ident->GetSymbol();

    assignee = std::move(ident);

    std::shared_ptr<RuntimeValue> varVal = env->LookUpVar(varname);

    return env->AssignVariable(varname,
                               evaluateBinaryOperation(varVal, value, op));
  }

  case NodeType::IndexingExpression: {

    auto indexingExpr = std::unique_ptr<IndexingExpressionNode>(
        static_cast<IndexingExpressionNode *>(assignee.release()));

    std::shared_ptr<RuntimeValue> accessorValue =
        evaluateExpr(indexingExpr->GetAccessor(), env);

    std::shared_ptr<RuntimeValue> indexValue =
        evaluateExpr(indexingExpr->GetIndex(), env);

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
Interpreter::evaluateBinaryExpr(std::unique_ptr<BinaryExprNode> &binaryExprNode,
                                std::unique_ptr<Environment> &env) {

  std::shared_ptr<RuntimeValue> left =
      evaluateExpr(binaryExprNode->GetLeft(), env);

  std::shared_ptr<RuntimeValue> right =
      evaluateExpr(binaryExprNode->GetRight(), env);

  return evaluateBinaryOperation(left, right, binaryExprNode->GetOperator());
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateUnaryExpr(std::unique_ptr<UnaryExprNode> &unaryExpr,
                               std::unique_ptr<Environment> &env) {
  std::shared_ptr<RuntimeValue> value =
      evaluateExpr(unaryExpr->GetValue(), env);

  return evaluateUnaryOperation(value, unaryExpr->GetOperator());
}

std::shared_ptr<RuntimeValue> Interpreter::evaluateObjectLiteral(
    std::unique_ptr<ObjectLiteralNode> &objectLiteralNode,
    std::unique_ptr<Environment> &env) {

  std::map<std::string, std::unique_ptr<Expr>> &properties =
      objectLiteralNode->GetProperties();

  std::map<std::string, std::shared_ptr<RuntimeValue>> objectProperties;

  for (auto it = properties.begin(); it != properties.end(); it++) {
    objectProperties.insert({it->first, evaluateExpr(it->second, env)});
  }

  return std::make_shared<ObjectValue>(std::move(objectProperties));
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateArrayExpr(std::unique_ptr<ArrayExprNode> &arr,
                               std::unique_ptr<Environment> &env) {

  ArrayList<std::unique_ptr<Expr>> &values = arr->GetValues();

  ArrayList<std::shared_ptr<RuntimeValue>> arrayValues =
      ArrayList<std::shared_ptr<RuntimeValue>>(10);

  int valueCount = values.Count();

  for (int i = 0; i < valueCount; i++) {
    arrayValues.Push(evaluateExpr(values.At(i), env));
  }

  return std::make_shared<ArrayValue>(std::move(arrayValues));
}

std::shared_ptr<RuntimeValue> Interpreter::evaluateIndexingExpr(
    std::unique_ptr<IndexingExpressionNode> &indexingExprNode,
    std::unique_ptr<Environment> &env) {

  std::shared_ptr<RuntimeValue> accessorValue =
      evaluateExpr(indexingExprNode->GetAccessor(), env);

  std::shared_ptr<RuntimeValue> indexValue =
      evaluateExpr(indexingExprNode->GetIndex(), env);

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

std::shared_ptr<RuntimeValue>
Interpreter::evaluateCallExpr(std::unique_ptr<CallExpr> &callExpr,
                              std::unique_ptr<Environment> &env) {

  std::shared_ptr<RuntimeValue> fn = evaluateExpr(callExpr->GetCaller(), env);

  ArrayList<std::shared_ptr<RuntimeValue>> argValues =
      ArrayList<std::shared_ptr<RuntimeValue>>(10);

  ArrayList<std::unique_ptr<Expr>> &args = callExpr->GetArgs();

  int argCount = args.Count();

  for (int i = 0; i < argCount; i++) {
    argValues.Push(evaluateExpr(args.At(i), env));
  }

  switch (fn->DataTypeID()) {
  case DataType::NativeFunction: {
    return std::static_pointer_cast<NativeFunctionValue>(fn)->Call(
        std::move(argValues), env);
  }

  case DataType::Function: {
    auto func = std::static_pointer_cast<FunctionValue>(fn);

    ArrayList<std::string> &params = func->GetParams();

    int paramCount = params.Count();

    if (paramCount != argCount) {
      throw std::runtime_error("expected " + std::to_string(paramCount) +
                               " arguments but get " +
                               std::to_string(argCount));
    }

    std::unique_ptr<Environment> functionScope = std::make_unique<Environment>(
        std::move(func->GetDeclaredEnv()), EnvironmentContext::Function);

    for (int i = 0; i < argCount; i++) {
      functionScope->DeclareVariable(params.At(i), argValues.At(i), true);
    }

    std::shared_ptr<RuntimeValue> returnValue =
        evaluateBlockStmt(func->GetBody(), functionScope);

    func->SetDeclaredEnv(std::move(functionScope->GetParent()));

    return std::static_pointer_cast<ReturnValue>(returnValue)->GetValue();
  }

  default:
    throw std::runtime_error(fn->str() + " is not a function");
  }
}
