#ifndef PARSER_H
#define PARSER_H

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/abstraction.h"
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

  std::unique_ptr<Stmt> parseStmt();
  std::unique_ptr<Stmt> parseVarDec();
  std::unique_ptr<Expr> parseExpr();
  std::unique_ptr<Expr> parseAdditiveExpr();
  std::unique_ptr<Expr> parseMultiplicativeExpr();
  std::unique_ptr<Expr> parsePrimaryExpr();

public:
  Parser(std::shared_ptr<Lexer> lexer) : lexer(lexer) {}

  std::unique_ptr<ProgramNode> ProduceAST();
};

#endif // !PARSER_H
