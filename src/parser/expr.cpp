#include "lexer/token.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

std::unique_ptr<Expr> Parser::parseAdditiveExpr() {
  std::unique_ptr<Expr> left = parseMultiplicativeExpr();

  while (at().getValue() == "+" || at().getValue() == "-") {
    std::string op = eat().getValue();

    std::unique_ptr<Expr> right = parseMultiplicativeExpr();

    left =
        std::make_unique<BinaryExprNode>(std::move(left), std::move(right), op);
  }

  return left;
}

std::unique_ptr<Expr> Parser::parseMultiplicativeExpr() {
  std::unique_ptr<Expr> left = parsePrimaryExpr();

  while (at().getValue() == "*" || at().getValue() == "/" ||
         at().getValue() == "%") {
    std::string op = eat().getValue();

    std::unique_ptr<Expr> right = parsePrimaryExpr();

    left =
        std::make_unique<BinaryExprNode>(std::move(left), std::move(right), op);
  }

  return left;
}

std::unique_ptr<Expr> Parser::parsePrimaryExpr() {
  Token tk = at();

  switch (tk.getTkType()) {
  case TokenType::Tk_Number:
    return std::make_unique<NumericLiteralNode>(std::stod(eat().getValue()));

  case TokenType::Tk_OpenParen: {
    std::unique_ptr<Expr> expr = parseExpr();

    expect(TokenType::Tk_ClosingParen);

    return expr;
  }

  default:
    throw std::runtime_error("Unexpected token " + tk.getValue());
  }
}
