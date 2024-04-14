#include "lexer/token.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <memory>
#include <vector>

std::shared_ptr<ProgramNode> Parser::ProduceAST() {
  tokens = lexer->Tokenize();

  std::vector<std::shared_ptr<Stmt>> stmts;

  while (notEOF()) {
    stmts.push_back(parseStmt());
  }

  return std::make_shared<ProgramNode>(stmts);
}

std::shared_ptr<Stmt> Parser::parseStmt() {
  switch (at().GetTokenType()) {
  case TokenType::Var:
    return parseVariableDeclaration();

  default:
    return parseExpr();
  }
}

std::shared_ptr<Expr> Parser::parseExpr() {
  std::shared_ptr<Expr> expr = parseAssignmentExpr();

  if (at().GetTokenType() == TokenType::SemiColon) {
    eat();
  }

  return expr;
}
