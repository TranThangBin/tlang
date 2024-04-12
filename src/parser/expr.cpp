#include "lexer/token.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

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

  switch (tk.GetTkType()) {
  case TokenType::Tk_Number:
    return std::make_unique<NumericLiteralNode>(std::stod(eat().GetValue()));

  case TokenType::Tk_OpenParen: {
    eat();
    std::unique_ptr<Expr> expr = parseExpr();
    expect(TokenType::Tk_ClosingParen);

    return expr;
  }

  default:
    throw std::runtime_error("Unexpected token " + tk.GetValue());
  }
}
