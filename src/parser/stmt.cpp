#include "lexer/token.h"
#include "parser/abstraction.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <memory>
#include <utility>

std::unique_ptr<Stmt> Parser::parseVarDec() {
  eat();
  bool mut = false;

  if (at().GetTkType() == TokenType::Tk_Mut) {
    eat();
    mut = true;
  }

  Token ident = expect(TokenType::Tk_Identifier);

  if (at().GetTkType() == TokenType::Tk_SemiColon) {
    eat();
    return std::make_unique<VariableDeclarationNode>(mut, ident.GetValue(),
                                                     nullptr);
  }

  expect(TokenType::Tk_Equal);
  std::unique_ptr<Expr> value = parseExpr();
  return std::make_unique<VariableDeclarationNode>(true, ident.GetValue(),
                                                   std::move(value));
}
