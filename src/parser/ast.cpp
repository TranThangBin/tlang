#include "parser/ast.h"
#include "lexer/token.h"
#include <stdexcept>
#include <string>

BinaryOperator TokenTypeToBinaryOperator(TokenType tokenType) {
  switch (tokenType) {
  case TokenType::Plus:
  case TokenType::AdditionAssignment:
    return BinaryOperator::Addition;

  case TokenType::Minus:
  case TokenType::SubtractionAssignment:
    return BinaryOperator::Subtraction;

  case TokenType::Asterisk:
  case TokenType::MultiplicationAssignment:
    return BinaryOperator::Multiplication;

  case TokenType::FowardSlash:
  case TokenType::DivisionAssignment:
    return BinaryOperator::Division;

  case TokenType::Percent:
  case TokenType::ModulusAssignment:
    return BinaryOperator::Modulo;

  default:
    throw std::runtime_error("Unexpected token type " +
                             TokenTypeToString(tokenType));
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
    throw std::runtime_error("Unexpected token type " +
                             TokenTypeToString(tokenType));
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

  default:
    return "unidentified";
  }
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

  default:
    return "unidentified";
  }
}

std::string UnaryOperatorToString(UnaryOperator unop) {
  switch (unop) {
  case UnaryOperator::Plus:
    return "+";

  case UnaryOperator::Minus:
    return "-";

  case UnaryOperator::Not:
    return "!";

  default:
    return "unidentified";
  }
}
