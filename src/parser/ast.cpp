#include "parser/ast.h"
#include "lexer/token.h"
#include <stdexcept>

BinaryOperator TokenTypeToBinaryOperator(TokenType tokenType) {
  switch (tokenType) {
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
