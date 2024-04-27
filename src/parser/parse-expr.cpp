#include "lexer/token.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

std::unique_ptr<Expr> Parser::parseAssignmentExpr() {
  std::unique_ptr<Expr> assignee = parseIndexingExpr();

  while (at().GetTokenType() == TokenType::Equal ||
         at().GetTokenType() == TokenType::Assignment) {

    if (at().GetTokenType() == TokenType::Equal) {
      eat();
      std::unique_ptr<Expr> value = parseAssignmentExpr();
      assignee = std::make_unique<AssignmentExprNode>(std::move(assignee),
                                                      std::move(value));
      continue;
    }

    BinaryOperator op = AssignmentToBinaryOperator(at().GetValue());

    std::unique_ptr<Expr> value = parseAssignmentExpr();
    assignee = std::make_unique<BinaryAssignmentExprNode>(std::move(assignee),
                                                          std::move(value), op);
    continue;
  }

  return assignee;
}

std::unique_ptr<Expr> Parser::parseIndexingExpr() {
  std::unique_ptr<Expr> accessor = parseAdditiveExpr();

  while (at().GetTokenType() == TokenType::OpenSquare) {
    eat();
    accessor = std::make_unique<IndexingExpressionNode>(std::move(accessor),
                                                        parseAdditiveExpr());
    expect(TokenType::ClosingSquare);
  }

  return accessor;
}

std::unique_ptr<Expr> Parser::parseAdditiveExpr() {
  std::unique_ptr<Expr> left = parseMultiplicativeExpr();

  while (at().GetTokenType() == TokenType::Plus ||
         at().GetTokenType() == TokenType::Minus) {
    BinaryOperator op = TokenTypeToBinaryOperator(eat().GetTokenType());

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
    BinaryOperator op = AssignmentToBinaryOperator(eat().GetValue());

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

  case TokenType::String:
    return std::make_unique<StringLiteralNode>(eat().GetValue());

  case TokenType::Identifier: {
    return std::make_unique<IdentifierNode>(eat().GetValue());
  }

  case TokenType::OpenParen: {
    eat();
    std::unique_ptr<Expr> expr = parseExpr();
    expect(TokenType::ClosingParen);

    return expr;
  }

  case TokenType::Plus:
  case TokenType::Minus:
  case TokenType::Exclamation: {
    UnaryOperator op = TokenTypeToUnaryOperator(eat().GetTokenType());

    return std::make_unique<UnaryExprNode>(parseExpr(), op);
  }

  case TokenType::OpenCurly: {
    eat();

    auto properties = std::map<std::string, std::unique_ptr<Expr>>();

    while (at().GetTokenType() != TokenType::ClosingCurly) {
      std::string key;

      if (at().GetTokenType() == TokenType::Identifier ||
          at().GetTokenType() == TokenType::String) {
        key = eat().GetValue();
      }

      expect(TokenType::Colon);

      std::unique_ptr<Expr> value = parseExpr();

      properties.insert({key, std::move(value)});

      if (at().GetTokenType() == TokenType::Comma) {
        eat();
      }
    }

    eat();

    return std::make_unique<ObjectLiteralNode>(std::move(properties));
  }

  default:
    throw std::runtime_error("Unexpected token " +
                             TokenTypeToString(tk.GetTokenType()));
  }
}
