#ifndef PARSER_H
#define PARSER_H

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/ast.h"
#include <memory>
#include <queue>

struct Parser {
private:
  std::shared_ptr<Lexer> lexer;
  std::queue<Token> tokens;

  bool notEOF();
  Token at();
  Token eat();
  Token expect(TokenType);

  std::shared_ptr<Stmt> parseStmt();
  std::shared_ptr<Stmt> parseVariableDeclaration();
  std::shared_ptr<Expr> parseExpr();
  std::shared_ptr<Expr> parseAssignmentExpr();
  std::shared_ptr<Expr> parseAdditiveExpr();
  std::shared_ptr<Expr> parseMultiplicativeExpr();
  std::shared_ptr<Expr> parsePrimaryExpr();

public:
  Parser(std::shared_ptr<Lexer> lexer) : lexer(lexer) {}

  std::shared_ptr<ProgramNode> ProduceAST();
};

#endif // !PARSER_H
