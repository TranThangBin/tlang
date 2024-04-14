#include "lexer/token.h"
#include "parser/abstraction.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <memory>

std::shared_ptr<Stmt> Parser::parseVariableDeclaration() {
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
  std::shared_ptr<Expr> value = parseExpr();
  return std::make_shared<VariableDeclarationNode>(mut, ident.GetValue(),
                                                   value);
}
