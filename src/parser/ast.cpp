#include "parser/ast.h"
#include "lexer/token.h"
#include <string>

BinaryOperator AssignmentToBinaryOperator(std::string str) {
  if (str == "+=") {
    return BinaryOperator::Addition;
  }

  if (str == "-=") {
    return BinaryOperator::Subtraction;
  }

  if (str == "*=") {
    return BinaryOperator::Multiplication;
  }

  if (str == "/=") {
    return BinaryOperator::Division;
  }

  if (str == "%=") {
    return BinaryOperator::Modulo;
  }

  return BinaryOperator::Invalid;
}

BinaryOperator TokenTypeToBinaryOperator(TokenType tkType) {
  switch (tkType) {
  case TokenType::Plus:
    return BinaryOperator::Addition;

  case TokenType::Minus:
    return BinaryOperator::Subtraction;

  case TokenType::Asterisk:
    return BinaryOperator::Multiplication;

  case TokenType::FowardSlash:
    return BinaryOperator::Division;

  case TokenType::Percent:
    return BinaryOperator::Modulo;

  default:
    return BinaryOperator::Invalid;
  }
}

UnaryOperator TokenTypeToUnaryOperator(TokenType tokenType) {
  switch (tokenType) {
  case TokenType::Plus:
    return UnaryOperator::Plus;

  case TokenType::Minus:
    return UnaryOperator::Minus;

  case TokenType::Exclamation:
    return UnaryOperator::Not;

  default:
    return UnaryOperator::Invalid;
  }
}

std::string NodeTypeToString(NodeType nodeType) {
  switch (nodeType) {
  case NodeType::Program:
    return "program";

  case NodeType::UnaryExpr:
    return "unary expression";

  case NodeType::BinaryExpr:
    return "binary expression";

  case NodeType::Identifier:
    return "identifier";

  case NodeType::AssignmentExpr:
    return "assignment expression";

  case NodeType::NumericLiteral:
    return "numeric literal";

  case NodeType::VariableDeclaration:
    return "variable declaratin";

  case NodeType::ObjectLiteral:
    return "object literal";

  case NodeType::BlockStmt:
    return "block statement";

  case NodeType::BinaryAssignmentExpr:
    return "binary assignment expression";
  }

  return "unknown";
}

std::string BinaryOperatorToString(BinaryOperator binop) {
  switch (binop) {
  case BinaryOperator::Addition:
    return "+";

  case BinaryOperator::Subtraction:
    return "-";

  case BinaryOperator::Multiplication:
    return "*";

  case BinaryOperator::Division:
    return "/";

  case BinaryOperator::Modulo:
    return "%";

  case BinaryOperator::Invalid:
    return "invalid";
  }

  return "unknown";
}

std::string UnaryOperatorToString(UnaryOperator unop) {
  switch (unop) {
  case UnaryOperator::Plus:
    return "+";

  case UnaryOperator::Minus:
    return "-";

  case UnaryOperator::Not:
    return "!";

  case UnaryOperator::Invalid:
    return "invalid";
  }

  return "unknown";
}
