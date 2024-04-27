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
                      std::unique_ptr<Environment> &environment) {
  switch (astNode->Kind()) {
  case NodeType::Program: {

    std::unique_ptr<ProgramNode> programNode = std::unique_ptr<ProgramNode>(
        static_cast<ProgramNode *>(astNode.release()));

    return evaluateProgram(std::move(programNode), environment);
  }

  case NodeType::VariableDeclaration: {

    std::unique_ptr<VariableDeclarationNode> variableDeclarationNode =
        std::unique_ptr<VariableDeclarationNode>(
            static_cast<VariableDeclarationNode *>(astNode.release()));

    return evaluateVariableDeclaration(std::move(variableDeclarationNode),
                                       environment);
  }

  case NodeType::BlockStmt: {
    std::unique_ptr<BlockStmtNode> blockStmtNode =
        std::unique_ptr<BlockStmtNode>(
            static_cast<BlockStmtNode *>(astNode.release()));

    return evaluateBlockStmt(std::move(blockStmtNode), environment);
  }

  case NodeType::AssignmentExpr: {

    std::unique_ptr<AssignmentExprNode> assignmentExprNode =
        std::unique_ptr<AssignmentExprNode>(
            static_cast<AssignmentExprNode *>(astNode.release()));

    return evaluateAssignmentExpr(std::move(assignmentExprNode), environment);
  }

  case NodeType::Identifier: {

    std::unique_ptr<IdentifierNode> identifierNode =
        std::unique_ptr<IdentifierNode>(
            static_cast<IdentifierNode *>(astNode.release()));

    return evaluateIdentifier(std::move(identifierNode), environment);
  }

  case NodeType::BinaryExpr: {

    std::unique_ptr<BinaryExprNode> binaryExprNode =
        std::unique_ptr<BinaryExprNode>(
            static_cast<BinaryExprNode *>(astNode.release()));

    return evaluateBinaryExpr(std::move(binaryExprNode), environment);
  }

  case NodeType::NumericLiteral: {

    std::unique_ptr<NumericLiteralNode> numericLiteralNode =
        std::unique_ptr<NumericLiteralNode>(
            static_cast<NumericLiteralNode *>(astNode.release()));

    return std::make_shared<NumberValue>(numericLiteralNode->GetValue());
  }

  case NodeType::StringLiteral: {
    std::unique_ptr<StringLiteralNode> stringLiteralNode =
        std::unique_ptr<StringLiteralNode>(
            static_cast<StringLiteralNode *>(astNode.release()));

    return std::make_shared<StringValue>(stringLiteralNode->GetValue());
  }

  case NodeType::UnaryExpr: {
    std::unique_ptr<UnaryExprNode> unaryExprNode =
        std::unique_ptr<UnaryExprNode>(
            static_cast<UnaryExprNode *>(astNode.release()));

    return evaluateUnaryExpr(std::move(unaryExprNode), environment);
  }

  case NodeType::BinaryAssignmentExpr: {
    std::unique_ptr<BinaryAssignmentExprNode> binaryAssignmentExprNode =
        std::unique_ptr<BinaryAssignmentExprNode>(
            static_cast<BinaryAssignmentExprNode *>(astNode.release()));

    return evaluateBinaryAssignmentExpr(std::move(binaryAssignmentExprNode),
                                        environment);
  }

  case NodeType::ObjectLiteral: {
    std::unique_ptr<ObjectLiteralNode> objectLiteralNode =
        std::unique_ptr<ObjectLiteralNode>(
            static_cast<ObjectLiteralNode *>(astNode.release()));

    return evaluateObjectLiteral(std::move(objectLiteralNode), environment);
  }

  case NodeType::IndexingExpression: {
    std::unique_ptr<IndexingExpressionNode> indexingExprNode =
        std::unique_ptr<IndexingExpressionNode>(
            static_cast<IndexingExpressionNode *>(astNode.release()));

    return evaluateIndexingExpr(std::move(indexingExprNode), environment);
  }

  default:
    throw std::runtime_error(
        NodeTypeToString(astNode->Kind()) +
        " node is currently not supported for interpretation");
  }
}
