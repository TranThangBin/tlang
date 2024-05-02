#include "lexer/token.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

std::unique_ptr<Expr> Parser::parseAssignmentExpr() {
  std::unique_ptr<Expr> assignee = parseOrExpr();

  while (at().GetTokenType() == TokenType::Equal ||
         at().GetTokenType() == TokenType::Assignment) {

    Token token = eat();
    std::unique_ptr<Expr> value = parseExpr();

    if (token.GetTokenType() == TokenType::Equal) {
      assignee = std::make_unique<AssignmentExprNode>(std::move(assignee),
                                                      std::move(value));
      continue;
    }

    assignee = std::make_unique<BinaryAssignmentExprNode>(
        std::move(assignee), std::move(value),
        AssignmentToBinaryOperator(token.GetValue()));
  }

  return assignee;
}

std::unique_ptr<Expr> Parser::parseOrExpr() {
  std::unique_ptr<Expr> left = parseAndExpr();

  while (at().GetTokenType() == TokenType::Or) {

    BinaryOperator op = TokenTypeToBinaryOperator(eat().GetTokenType());

    std::unique_ptr<Expr> right = parseAndExpr();

    left =
        std::make_unique<BinaryExprNode>(std::move(left), std::move(right), op);
  }

  return left;
}

std::unique_ptr<Expr> Parser::parseAndExpr() {
  std::unique_ptr<Expr> left = parseComparisonExpr();

  while (at().GetTokenType() == TokenType::And) {

    BinaryOperator op = TokenTypeToBinaryOperator(eat().GetTokenType());

    std::unique_ptr<Expr> right = parseComparisonExpr();

    left =
        std::make_unique<BinaryExprNode>(std::move(left), std::move(right), op);
  }

  return left;
}

std::unique_ptr<Expr> Parser::parseComparisonExpr() {
  std::unique_ptr<Expr> left = parseAdditiveExpr();

  while (at().GetTokenType() == TokenType::Equality ||
         at().GetTokenType() == TokenType::InEquality ||
         at().GetTokenType() == TokenType::Greater ||
         at().GetTokenType() == TokenType::Lower ||
         at().GetTokenType() == TokenType::GreaterEqual ||
         at().GetTokenType() == TokenType::LowerEqual) {

    BinaryOperator op = TokenTypeToBinaryOperator(eat().GetTokenType());

    std::unique_ptr<Expr> right = parseAdditiveExpr();

    left =
        std::make_unique<BinaryExprNode>(std::move(left), std::move(right), op);
  }

  return left;
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
  std::unique_ptr<Expr> left = parseIndexingOrCallExpr();

  while (at().GetTokenType() == TokenType::Asterisk ||
         at().GetTokenType() == TokenType::FowardSlash ||
         at().GetTokenType() == TokenType::Percent) {

    BinaryOperator op = TokenTypeToBinaryOperator(eat().GetTokenType());

    std::unique_ptr<Expr> right = parseIndexingOrCallExpr();

    left =
        std::make_unique<BinaryExprNode>(std::move(left), std::move(right), op);
  }

  return left;
}

std::unique_ptr<Expr> Parser::parseIndexingOrCallExpr() {
  std::unique_ptr<Expr> primeExpr = parsePrimaryExpr();

  while (at().GetTokenType() == TokenType::OpenSquare ||
         at().GetTokenType() == TokenType::OpenParen) {

    if (eat().GetTokenType() == TokenType::OpenSquare) {

      primeExpr = std::make_unique<IndexingExpressionNode>(std::move(primeExpr),
                                                           parseExpr());
      expect(TokenType::ClosingSquare);

      continue;
    }

    std::vector<std::unique_ptr<Expr>> args;

    while (at().GetTokenType() != TokenType::ClosingParen) {
      args.push_back(parseExpr());

      if (at().GetTokenType() == TokenType::Comma) {
        eat();
      }
    }

    eat();

    primeExpr =
        std::make_unique<CallExpr>(std::move(args), std::move(primeExpr));
  }

  return primeExpr;
}

std::unique_ptr<Expr> Parser::parsePrimaryExpr() {
  Token tk = at();

  switch (tk.GetTokenType()) {
  case TokenType::Number: {
    std::string number = eat().GetValue();

    if (at().GetTokenType() != TokenType::Dot) {
      return std::make_unique<NumericLiteralNode>(std::stod(number));
    }

    eat();

    return std::make_unique<NumericLiteralNode>(
        std::stod(number + "." + expect(TokenType::Number).GetValue()));
  }

  case TokenType::Dot:
    eat();
    return std::make_unique<NumericLiteralNode>(
        std::stod("0." + expect(TokenType::Number).GetValue()));

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
  case TokenType::Exclamation:
    return std::make_unique<UnaryExprNode>(
        parseExpr(), TokenTypeToUnaryOperator(eat().GetTokenType()));

  case TokenType::OpenCurly: {
    eat();

    std::map<std::string, std::unique_ptr<Expr>> properties;

    while (at().GetTokenType() != TokenType::ClosingCurly) {
      std::string key;

      if (at().GetTokenType() == TokenType::Identifier ||
          at().GetTokenType() == TokenType::String) {
        key = eat().GetValue();
      }

      expect(TokenType::Colon);

      properties.insert({key, parseExpr()});

      if (at().GetTokenType() == TokenType::Comma) {
        eat();
      }
    }

    eat();

    return std::make_unique<ObjectLiteralNode>(std::move(properties));
  }

  case TokenType::OpenSquare: {
    eat();

    std::vector<std::unique_ptr<Expr>> values;

    while (at().GetTokenType() != TokenType::ClosingSquare) {
      values.push_back(parseExpr());

      if (at().GetTokenType() == TokenType::Comma) {
        eat();
      }
    }

    eat();

    return std::make_unique<ArrayExprNode>(std::move(values));
  }

  default:
    throw std::runtime_error("Unexpected token " +
                             TokenTypeToString(tk.GetTokenType()));
  }
}
