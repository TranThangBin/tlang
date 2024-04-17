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

  while (at().GetTokenType() == TokenType::Equal ||
         at().GetTokenType() == TokenType::BinaryAssignment) {
    Token token = eat();

    std::unique_ptr<Expr> value = parseAssignmentExpr();

    if (token.GetTokenType() == TokenType::BinaryAssignment) {
      assignee = std::make_unique<BinaryAssignmentExprNode>(
          std::move(assignee), std::move(value), token.GetValue());
      continue;
    }

    assignee = std::make_unique<AssignmentExprNode>(std::move(assignee),
                                                    std::move(value));
  }

  return assignee;
}

std::unique_ptr<Expr> Parser::parseAdditiveExpr() {
  std::unique_ptr<Expr> left = parseMultiplicativeExpr();

  while (at().GetTokenType() == TokenType::Plus ||
         at().GetTokenType() == TokenType::Minus) {
    std::string op = eat().GetValue();

    std::unique_ptr<Expr> right = parseMultiplicativeExpr();

    left =
        std::make_unique<BinaryExprNode>(std::move(left), std::move(right), op);
  }

  return left;
}

std::unique_ptr<Expr> Parser::parseMultiplicativeExpr() {
  std::unique_ptr<Expr> left = parsePrimaryExpr();

  while (at().GetTokenType() == TokenType::Asterisk ||
         at().GetTokenType() == TokenType::FowardSlash ||
         at().GetTokenType() == TokenType::Percent) {
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

  case TokenType::Plus:
  case TokenType::Minus:
  case TokenType::Exclamation: {
    std::string op = eat().GetValue();

    return std::make_unique<UnaryExprNode>(parseExpr(), op);
  }

  default:
    std::stringstream ss;
    ss << "Unexpected token " << (int)tk.GetTokenType();
    throw std::runtime_error(ss.str());
  }
}
