#include "runtime/interpreter.h"
#include "parser/ast.h"
#include "runtime/environment.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <stdexcept>
#include <utility>

std::shared_ptr<RuntimeValue> Interpreter::Evaluate() {
  return evaluate(parser->ProduceAST(), environment);
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluate(std::unique_ptr<Stmt> astNode,
                      std::unique_ptr<Environment> &env) {
  switch (astNode->Kind()) {
  case NodeType::Program: {

    auto program = std::unique_ptr<ProgramNode>(
        static_cast<ProgramNode *>(astNode.release()));

    return evaluateProgram(std::move(program), env);
  }

  case NodeType::VariableDeclaration: {

    auto varDec = std::unique_ptr<VariableDeclarationNode>(
        static_cast<VariableDeclarationNode *>(astNode.release()));

    return evaluateVariableDeclaration(std::move(varDec), env);
  }

  case NodeType::BlockStmt: {

    auto block = std::unique_ptr<BlockStmtNode>(
        static_cast<BlockStmtNode *>(astNode.release()));

    return evaluateBlockStmt(std::move(block), env);
  }

  case NodeType::AssignmentExpr: {

    auto assignment = std::unique_ptr<AssignmentExprNode>(
        static_cast<AssignmentExprNode *>(astNode.release()));

    return evaluateAssignmentExpr(std::move(assignment), env);
  }

  case NodeType::Identifier: {

    auto ident = std::unique_ptr<IdentifierNode>(
        static_cast<IdentifierNode *>(astNode.release()));

    return evaluateIdentifier(std::move(ident), env);
  }

  case NodeType::BinaryExpr: {

    auto binExpr = std::unique_ptr<BinaryExprNode>(
        static_cast<BinaryExprNode *>(astNode.release()));

    return evaluateBinaryExpr(std::move(binExpr), env);
  }

  case NodeType::NumericLiteral: {

    auto numeric = std::unique_ptr<NumericLiteralNode>(
        static_cast<NumericLiteralNode *>(astNode.release()));

    return std::make_shared<NumberValue>(numeric->GetValue());
  }

  case NodeType::StringLiteral: {

    auto str = std::unique_ptr<StringLiteralNode>(
        static_cast<StringLiteralNode *>(astNode.release()));

    return std::make_shared<StringValue>(str->GetValue());
  }

  case NodeType::UnaryExpr: {

    auto unaryExpr = std::unique_ptr<UnaryExprNode>(
        static_cast<UnaryExprNode *>(astNode.release()));

    return evaluateUnaryExpr(std::move(unaryExpr), env);
  }

  case NodeType::BinaryAssignmentExpr: {

    auto binAssignExpr = std::unique_ptr<BinaryAssignmentExprNode>(
        static_cast<BinaryAssignmentExprNode *>(astNode.release()));

    return evaluateBinaryAssignmentExpr(std::move(binAssignExpr), env);
  }

  case NodeType::ObjectLiteral: {

    auto obj = std::unique_ptr<ObjectLiteralNode>(
        static_cast<ObjectLiteralNode *>(astNode.release()));

    return evaluateObjectLiteral(std::move(obj), env);
  }

  case NodeType::ArrayExpr: {

    auto arr = std::unique_ptr<ArrayExprNode>(
        static_cast<ArrayExprNode *>(astNode.release()));

    return evaluateArrayExpr(std::move(arr), env);
  }

  case NodeType::IndexingExpression: {

    auto indExpr = std::unique_ptr<IndexingExpressionNode>(
        static_cast<IndexingExpressionNode *>(astNode.release()));

    return evaluateIndexingExpr(std::move(indExpr), env);
  }

  case NodeType::FunctionExpr: {
    auto funcExpr = std::unique_ptr<FunctionExprNode>(
        static_cast<FunctionExprNode *>(astNode.release()));

    return std::make_shared<FunctionValue>(funcExpr->GetParams(),
                                           std::move(funcExpr->GetBlock()));
  }

  default:
    throw std::runtime_error(
        NodeTypeToString(astNode->Kind()) +
        " node is currently not supported for interpretation");
  }
}
