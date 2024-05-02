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

  std::vector<std::unique_ptr<Stmt>> body;

  while (at().GetTokenType() != TokenType::ClosingCurly) {
    body.push_back(parseStmt());
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
