#include "lexer/token.h"
#include "parser/abstraction.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <memory>
#include <utility>
#include <vector>

std::unique_ptr<ProgramNode> Parser::ProduceAST() {
  tokens = lexer->Tokenize();

  std::vector<std::unique_ptr<Stmt>> stmts;
  while (notEOF()) {
    stmts.push_back(std::move(parseStmt()));
  }

  return std::make_unique<ProgramNode>(std::move(stmts));
}

std::unique_ptr<Stmt> Parser::parseStmt() {
  switch (at().getTkType()) {
  case TokenType::Tk_Var:
    return parseVarDec();

  default:
    return parseExpr();
  }
}

std::unique_ptr<Expr> Parser::parseExpr() {
  std::unique_ptr<Expr> expr = parseAdditiveExpr();

  if (at().getTkType() == TokenType::Tk_SemiColon) {
    eat();
  }

  return expr;
}
