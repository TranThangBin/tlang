#include "lexer/token.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <memory>
#include <utility>
#include <vector>

std::unique_ptr<ProgramNode> Parser::ProduceAST() {
  tokens = std::move(lexer->Tokenize());

  std::vector<std::unique_ptr<Stmt>> stmts;

  while (notEOF()) {
    stmts.push_back(parseStmt());
  }

  return std::make_unique<ProgramNode>(std::move(stmts));
}

std::unique_ptr<Stmt> Parser::parseStmt() {
  switch (at().GetTokenType()) {
  case TokenType::Var:
    return parseVariableDeclaration();

  default:
    return parseExpr();
  }
}

std::unique_ptr<Expr> Parser::parseExpr() {
  std::unique_ptr<Expr> expr = parseAssignmentExpr();

  if (at().GetTokenType() == TokenType::SemiColon) {
    eat();
  }

  return expr;
}
