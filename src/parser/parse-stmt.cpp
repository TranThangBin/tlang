#include "lexer/token.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <memory>
#include <stdexcept>
#include <utility>

std::unique_ptr<Stmt> Parser::parseVariableDeclaration() {
  eat();
  bool mut = false;

  if (at().GetTokenType() == TokenType::Mut) {
    eat();
    mut = true;
  }

  Token ident = expect(TokenType::Identifier);

  if (at().GetTokenType() == TokenType::SemiColon) {
    if (!mut) {
      throw std::runtime_error("Immutables must be assigned when initialized");
    }
    return std::make_unique<VariableDeclarationNode>(mut, ident.GetValue(),
                                                     nullptr);
  }

  expect(TokenType::Equal);
  std::unique_ptr<Expr> value = parseExpr();
  return std::make_unique<VariableDeclarationNode>(mut, ident.GetValue(),
                                                   std::move(value));
}

std::unique_ptr<Stmt> Parser::parseBlockStmt() {
  eat();

  std::vector<std::unique_ptr<Stmt>> stmts;

  while (at().GetTokenType() != TokenType::ClosingCurly) {
    stmts.push_back(parseStmt());
  }

  eat();

  return std::make_unique<BlockStmtNode>(std::move(stmts));
}
