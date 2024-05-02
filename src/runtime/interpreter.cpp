#include "runtime/interpreter.h"
#include "parser/ast.h"
#include "runtime/environment.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <stdexcept>
#include <utility>

std::shared_ptr<RuntimeValue> Interpreter::Evaluate() {
  std::unique_ptr<Stmt> program = parser->ProduceAST();
  return evaluateStmt(program, environment);
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateStmt(std::unique_ptr<Stmt> &stmtNode,
                          std::unique_ptr<Environment> &env) {

  std::shared_ptr<RuntimeValue> result;

  switch (stmtNode->Kind()) {
  case NodeType::Program: {

    auto program = std::unique_ptr<ProgramNode>(
        static_cast<ProgramNode *>(stmtNode.release()));

    result = evaluateProgram(program, env);

    stmtNode = std::move(program);

    return result;
  }

  case NodeType::VariableDeclaration: {

    auto varDec = std::unique_ptr<VariableDeclarationNode>(
        static_cast<VariableDeclarationNode *>(stmtNode.release()));

    result = evaluateVariableDeclaration(varDec, env);

    stmtNode = std::move(varDec);

    return result;
  }

  case NodeType::BlockStmt: {

    auto block = std::unique_ptr<BlockStmtNode>(
        static_cast<BlockStmtNode *>(stmtNode.release()));

    std::unique_ptr<Environment> blockScope = std::make_unique<Environment>(
        std::move(env), EnvironmentContext::Block);

    result = evaluateBlockStmt(block, blockScope);

    env = std::move(blockScope->GetParent());

    stmtNode = std::move(block);

    return result;
  }

  case NodeType::FunctionDeclaration: {
    auto funcDec = std::unique_ptr<FunctionDeclarationNode>(
        static_cast<FunctionDeclarationNode *>(stmtNode.release()));

    result = evaluateFunctionDeclaration(funcDec, env);

    stmtNode = std::move(funcDec);

    return result;
  }

  case NodeType::ReturnStmt: {
    if (!env->HasContext(EnvironmentContext::Function)) {
      throw std::runtime_error("illegal return statement");
    }

    auto returnStmt = std::unique_ptr<ReturnStmtNode>(
        static_cast<ReturnStmtNode *>(stmtNode.release()));

    result = std::make_shared<NullValue>();

    if (returnStmt->GetValue() != nullptr) {
      result = evaluateExpr(returnStmt->GetValue(), env);
    }

    stmtNode = std::move(returnStmt);

    return result;
  }

  default: {
    auto expr = std::unique_ptr<Expr>(static_cast<Expr *>(stmtNode.release()));

    result = evaluateExpr(expr, env);

    stmtNode = std::move(expr);

    return result;
  }
  }
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateExpr(std::unique_ptr<Expr> &exprNode,
                          std::unique_ptr<Environment> &env) {

  std::shared_ptr<RuntimeValue> result;

  switch (exprNode->Kind()) {

  case NodeType::AssignmentExpr: {

    auto assignment = std::unique_ptr<AssignmentExprNode>(
        static_cast<AssignmentExprNode *>(exprNode.release()));

    result = evaluateAssignmentExpr(assignment, env);

    exprNode = std::move(assignment);

    return result;
  }

  case NodeType::Identifier: {

    auto ident = std::unique_ptr<IdentifierNode>(
        static_cast<IdentifierNode *>(exprNode.release()));

    result = evaluateIdentifier(ident, env);

    exprNode = std::move(ident);

    return result;
  }

  case NodeType::BinaryExpr: {

    auto binExpr = std::unique_ptr<BinaryExprNode>(
        static_cast<BinaryExprNode *>(exprNode.release()));

    result = evaluateBinaryExpr(binExpr, env);

    exprNode = std::move(binExpr);

    return result;
  }

  case NodeType::NumericLiteral: {

    auto numeric = std::unique_ptr<NumericLiteralNode>(
        static_cast<NumericLiteralNode *>(exprNode.release()));

    result = std::make_shared<NumberValue>(numeric->GetValue());

    exprNode = std::move(numeric);

    return result;
  }

  case NodeType::StringLiteral: {

    auto str = std::unique_ptr<StringLiteralNode>(
        static_cast<StringLiteralNode *>(exprNode.release()));

    result = std::make_shared<StringValue>(str->GetValue());

    exprNode = std::move(str);

    return result;
  }

  case NodeType::UnaryExpr: {

    auto unaryExpr = std::unique_ptr<UnaryExprNode>(
        static_cast<UnaryExprNode *>(exprNode.release()));

    result = evaluateUnaryExpr(unaryExpr, env);

    exprNode = std::move(unaryExpr);

    return result;
  }

  case NodeType::BinaryAssignmentExpr: {

    auto binAssignExpr = std::unique_ptr<BinaryAssignmentExprNode>(
        static_cast<BinaryAssignmentExprNode *>(exprNode.release()));

    result = evaluateBinaryAssignmentExpr(binAssignExpr, env);

    exprNode = std::move(binAssignExpr);

    return result;
  }

  case NodeType::ObjectLiteral: {

    auto obj = std::unique_ptr<ObjectLiteralNode>(
        static_cast<ObjectLiteralNode *>(exprNode.release()));

    result = evaluateObjectLiteral(obj, env);

    exprNode = std::move(obj);

    return result;
  }

  case NodeType::ArrayExpr: {

    auto arr = std::unique_ptr<ArrayExprNode>(
        static_cast<ArrayExprNode *>(exprNode.release()));

    result = evaluateArrayExpr(arr, env);

    exprNode = std::move(arr);

    return result;
  }

  case NodeType::IndexingExpression: {

    auto indExpr = std::unique_ptr<IndexingExpressionNode>(
        static_cast<IndexingExpressionNode *>(exprNode.release()));

    result = evaluateIndexingExpr(indExpr, env);

    exprNode = std::move(indExpr);

    return result;
  }

  case NodeType::CallExpr: {
    auto callExpr =
        std::unique_ptr<CallExpr>(static_cast<CallExpr *>(exprNode.release()));

    result = evaluateCallExpr(callExpr, env);

    exprNode = std::move(callExpr);

    return result;
  }

  default:
    throw std::runtime_error(
        NodeTypeToString(exprNode->Kind()) +
        " node is currently not supported for interpretation");
  }
}
