#include "datastructure/array-list.h"
#include "lexer/token.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

std::unique_ptr<VariableDeclarationNode> Parser::parseVariableDeclaration() {
  eat();

  bool mut = at().GetTokenType() == TokenType::Mut;

  if (mut) {
    eat();
  }

  std::string ident = expect(TokenType::Identifier).GetValue();

  if (at().GetTokenType() == TokenType::SemiColon) {
    if (!mut) {
      throw std::runtime_error("Immutables must be assigned when initialized");
    }
    return std::make_unique<VariableDeclarationNode>(mut, ident, nullptr);
  }

  expect(TokenType::Equal);
  return std::make_unique<VariableDeclarationNode>(mut, ident, parseExpr());
}

std::unique_ptr<BlockStmtNode> Parser::parseBlockStmt() {
  expect(TokenType::OpenCurly);

  ArrayList<std::unique_ptr<Stmt>> body = ArrayList<std::unique_ptr<Stmt>>(50);

  while (at().GetTokenType() != TokenType::ClosingCurly) {
    body.Push(parseStmt());
  }

  eat();

  return std::make_unique<BlockStmtNode>(std::move(body));
}

std::unique_ptr<FunctionDeclarationNode> Parser::parseFunctionDeclaration() {
  eat();

  std::string name = expect(TokenType::Identifier).GetValue();

  expect(TokenType::OpenParen);

  std::vector<std::string> params;

  while (at().GetTokenType() != TokenType::ClosingParen) {
    params.push_back(expect(TokenType::Identifier).GetValue());

    if (at().GetTokenType() == TokenType::Comma) {
      eat();
    }
  }

  eat();

  std::unique_ptr<BlockStmtNode> body = parseBlockStmt();

  return std::make_unique<FunctionDeclarationNode>(name, params,
                                                   std::move(body));
}

std::unique_ptr<IfStmtNode> Parser::parseIfStmt() {
  eat();

  expect(TokenType::OpenParen);

  std::unique_ptr<Expr> condition = parseExpr();

  expect(TokenType::ClosingParen);

  std::unique_ptr<Stmt> ifBody = parseStmt();

  if (at().GetTokenType() != TokenType::Else) {
    return std::make_unique<IfStmtNode>(std::move(condition), std::move(ifBody),
                                        nullptr);
  }

  eat();

  return std::make_unique<IfStmtNode>(std::move(condition), std::move(ifBody),
                                      parseStmt());
}

std::unique_ptr<ForLoopNode> Parser::parseForLoop() {
  eat();

  expect(TokenType::OpenParen);

  std::unique_ptr<Stmt> initilizer = nullptr;
  std::unique_ptr<Expr> condition = nullptr;
  std::unique_ptr<Expr> modifier = nullptr;

  if (at().GetTokenType() != TokenType::SemiColon) {
    initilizer = parseStmt();
  } else {
    eat();
  }

  if (at().GetTokenType() != TokenType::SemiColon) {
    condition = parseExpr();
  }

  expect(TokenType::SemiColon);

  if (at().GetTokenType() != TokenType::ClosingParen) {
    modifier = parseExpr();
  }

  expect(TokenType::ClosingParen);

  return std::make_unique<ForLoopNode>(std::move(initilizer),
                                       std::move(condition),
                                       std::move(modifier), parseStmt());
}
