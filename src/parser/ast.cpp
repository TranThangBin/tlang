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

  case TokenType::Equality:
    return BinaryOperator::Equality;

  case TokenType::InEquality:
    return BinaryOperator::InEquality;

  case TokenType::Greater:
    return BinaryOperator::Greater;

  case TokenType::Lower:
    return BinaryOperator::Lower;

  case TokenType::GreaterEqual:
    return BinaryOperator::GreaterEqual;

  case TokenType::LowerEqual:
    return BinaryOperator::LowerEqual;

  case TokenType::And:
    return BinaryOperator::And;

  case TokenType::Or:
    return BinaryOperator::Or;

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
  case NodeType::ForLoop:
    return "for loop";

  case NodeType::IfStmt:
    return "if statement";

  case NodeType::ReturnStmt:
    return "return statement";

  case NodeType::FunctionDeclaration:
    return "function declaration";

  case NodeType::CallExpr:
    return "call expression";

  case NodeType::ArrayExpr:
    return "array expression";

  case NodeType::StringLiteral:
    return "string literal";

  case NodeType::IndexingExpression:
    return "indexing expression";

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
    return "variable declaration";

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
  case BinaryOperator::And:
    return "&&";

  case BinaryOperator::Or:
    return "||";

  case BinaryOperator::Equality:
    return "==";

  case BinaryOperator::InEquality:
    return "!=";

  case BinaryOperator::Greater:
    return ">";

  case BinaryOperator::Lower:
    return "<";

  case BinaryOperator::GreaterEqual:
    return ">=";

  case BinaryOperator::LowerEqual:
    return "<=";

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
