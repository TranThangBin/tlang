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
  std::unique_ptr<Stmt> node = nullptr;

  switch (at().GetTokenType()) {
  case TokenType::Var:
    node = parseVariableDeclaration();
    break;

  case TokenType::OpenCurly:
    return parseBlockStmt();

  case TokenType::Fun:
    return parseFunctionDeclaration();

  case TokenType::If:
    return parseIfStmt();

  case TokenType::Return:
    eat();

    if (at().GetTokenType() == TokenType::SemiColon) {
      node = std::make_unique<ReturnStmtNode>(nullptr);
    } else {
      node = std::make_unique<ReturnStmtNode>(parseExpr());
    }

    break;

  default:
    node = parseExpr();
    break;
  }

  expect(TokenType::SemiColon);

  return node;
}

std::unique_ptr<Expr> Parser::parseExpr() { return parseAssignmentExpr(); }
