#include "lexer/token.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

std::shared_ptr<Expr> Parser::parseAssignmentExpr() {
  std::shared_ptr<Expr> assignee = parseAdditiveExpr();

  while (at().GetTokenType() == TokenType::Equal) {
    eat();

    std::shared_ptr<Expr> value = parseAssignmentExpr();

    assignee = std::make_shared<AssignmentExprNode>(assignee, value);
  }

  return assignee;
}

std::shared_ptr<Expr> Parser::parseAdditiveExpr() {
  std::shared_ptr<Expr> left = parseMultiplicativeExpr();

  while (at().GetValue() == "+" || at().GetValue() == "-") {
    std::string op = eat().GetValue();

    std::shared_ptr<Expr> right = parseMultiplicativeExpr();

    left = std::make_shared<BinaryExprNode>(left, right, op);
  }

  return left;
}

std::shared_ptr<Expr> Parser::parseMultiplicativeExpr() {
  std::shared_ptr<Expr> left = parsePrimaryExpr();

  while (at().GetValue() == "*" || at().GetValue() == "/" ||
         at().GetValue() == "%") {
    std::string op = eat().GetValue();

    std::shared_ptr<Expr> right = parsePrimaryExpr();

    left = std::make_shared<BinaryExprNode>(left, right, op);
  }

  return left;
}

std::shared_ptr<Expr> Parser::parsePrimaryExpr() {
  Token tk = at();

  switch (tk.GetTokenType()) {
  case TokenType::Number:
    return std::make_shared<NumericLiteralNode>(std::stof(eat().GetValue()));

  case TokenType::Identifier:
    return std::make_shared<IdentifierNode>(eat().GetValue());

  case TokenType::OpenParen: {
    eat();
    std::shared_ptr<Expr> expr = parseExpr();
    expect(TokenType::ClosingParen);

    return expr;
  }

  default:
    std::stringstream ss;
    ss << "Unexpected token " << (int)tk.GetTokenType();
    throw std::runtime_error(ss.str());
  }
}
