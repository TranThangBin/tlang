#include "lexer/token.h"
#include "parser/abstraction.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <memory>
#include <utility>

std::unique_ptr<Stmt> Parser::parseVarDec() {
  eat();
  bool mut = false;

  if (at().GetTokenType() == TokenType::Mut) {
    eat();
    mut = true;
  }

  Token ident = expect(TokenType::Identifier);

  if (at().GetTokenType() == TokenType::SemiColon) {
    eat();
    return std::make_unique<VariableDeclarationNode>(mut, ident.GetValue(),
                                                     nullptr);
  }

  expect(TokenType::Equal);
  std::unique_ptr<Expr> value = parseExpr();
  return std::make_unique<VariableDeclarationNode>(mut, ident.GetValue(),
                                                   std::move(value));
}
