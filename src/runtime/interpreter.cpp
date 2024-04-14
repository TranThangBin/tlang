#include "runtime/interpreter.h"
#include "parser/ast.h"
#include "runtime/environment.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <sstream>
#include <stdexcept>

std::shared_ptr<RuntimeValue> Interpreter::Evaluate() {
  return evaluate(parser->ProduceAST(), environment);
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluate(std::shared_ptr<Stmt> astNode,
                      std::shared_ptr<Environment> environment) {
  switch (astNode->Kind()) {
  case NodeType::Program: {
    std::shared_ptr<ProgramNode> programNode =
        std::static_pointer_cast<ProgramNode>(astNode);
    return evaluateProgram(programNode, environment);
  }

  case NodeType::VariableDeclaration: {
    std::shared_ptr<VariableDeclarationNode> variableDeclarationNode =
        std::static_pointer_cast<VariableDeclarationNode>(astNode);
    return evaluateVariableDeclaration(variableDeclarationNode, environment);
  }

  case NodeType::AssignmentExpr: {
    std::shared_ptr<AssignmentExprNode> assignmentExprNode =
        std::static_pointer_cast<AssignmentExprNode>(astNode);
    return evaluateAssignmentExpr(assignmentExprNode, environment);
  }

  case NodeType::Identifier: {
    std::shared_ptr<IdentifierNode> identifierNode =
        std::static_pointer_cast<IdentifierNode>(astNode);
    return evaluateIdentifier(identifierNode, environment);
  }

  case NodeType::BinaryExpr: {
    std::shared_ptr<BinaryExprNode> binaryExprNode =
        std::static_pointer_cast<BinaryExprNode>(astNode);
    return evaluateBinaryExpr(binaryExprNode, environment);
  }

  case NodeType::NumericLiteral: {
    std::shared_ptr<NumericLiteralNode> numericLiteralNode =
        std::static_pointer_cast<NumericLiteralNode>(astNode);
    return std::make_shared<NumberValue>(numericLiteralNode->GetValue());
  }

  default:
    std::stringstream ss;
    ss << (int)astNode->Kind()
       << " node is currently not supported for interpretation";
    throw std::runtime_error(ss.str());
  }
}
