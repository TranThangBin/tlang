#include "lexer/token.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

std::unique_ptr<Expr> Parser::parseAssignmentExpr() {
  std::unique_ptr<Expr> assignee = parseAdditiveExpr();

  while (at().GetTokenType() == TokenType::Equal) {
    eat();

    std::unique_ptr<Expr> value = parseAssignmentExpr();

    assignee = std::make_unique<AssignmentExprNode>(std::move(assignee),
                                                    std::move(value));
  }

  return assignee;
}

std::unique_ptr<Expr> Parser::parseAdditiveExpr() {
  std::unique_ptr<Expr> left = parseMultiplicativeExpr();

  while (at().GetValue() == "+" || at().GetValue() == "-") {
    std::string op = eat().GetValue();

    std::unique_ptr<Expr> right = parseMultiplicativeExpr();

    left =
        std::make_unique<BinaryExprNode>(std::move(left), std::move(right), op);
  }

  return left;
}

std::unique_ptr<Expr> Parser::parseMultiplicativeExpr() {
  std::unique_ptr<Expr> left = parsePrimaryExpr();

  while (at().GetValue() == "*" || at().GetValue() == "/" ||
         at().GetValue() == "%") {
    std::string op = eat().GetValue();

    std::unique_ptr<Expr> right = parsePrimaryExpr();

    left =
        std::make_unique<BinaryExprNode>(std::move(left), std::move(right), op);
  }

  return left;
}

std::unique_ptr<Expr> Parser::parsePrimaryExpr() {
  Token tk = at();

  switch (tk.GetTokenType()) {
  case TokenType::Number:
    return std::make_unique<NumericLiteralNode>(std::stof(eat().GetValue()));

  case TokenType::Identifier:
    return std::make_unique<IdentifierNode>(eat().GetValue());

  case TokenType::OpenParen: {
    eat();
    std::unique_ptr<Expr> expr = parseExpr();
    expect(TokenType::ClosingParen);

    return expr;
  }

  case TokenType::BinaryOperator: {
    if (tk.GetValue() == "-") {
      eat();
      Token value = expect(TokenType::Number);
      return std::make_unique<NumericLiteralNode>(-std::stof(value.GetValue()));
    }
  }

  default:
    std::stringstream ss;
    ss << "Unexpected token " << (int)tk.GetTokenType();
    throw std::runtime_error(ss.str());
  }
}
