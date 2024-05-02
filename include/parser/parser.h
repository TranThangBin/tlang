#ifndef PARSER_H
#define PARSER_H
#define PARSER_VERSION "v0.0.9"

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

  std::unique_ptr<Stmt> parseStmt();
  std::unique_ptr<VariableDeclarationNode> parseVariableDeclaration();
  std::unique_ptr<BlockStmtNode> parseBlockStmt();
  std::unique_ptr<FunctionDeclarationNode> parseFunctionDeclaration();
  std::unique_ptr<IfStmtNode> parseIfStmt();
  std::unique_ptr<Expr> parseExpr();
  std::unique_ptr<Expr> parseAssignmentExpr();
  std::unique_ptr<Expr> parseOrExpr();
  std::unique_ptr<Expr> parseAndExpr();
  std::unique_ptr<Expr> parseComparisonExpr();
  std::unique_ptr<Expr> parseAdditiveExpr();
  std::unique_ptr<Expr> parseMultiplicativeExpr();
  std::unique_ptr<Expr> parseIndexingOrCallExpr();
  std::unique_ptr<Expr> parsePrimaryExpr();

public:
  Parser(std::shared_ptr<Lexer> lexer) : lexer(lexer) {}

  std::unique_ptr<ProgramNode> ProduceAST();
};

#endif // !PARSER_H
