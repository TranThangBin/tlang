#include "lexer/token.h"
#include "parser/parser.h"
#include <stdexcept>

bool Parser::notEOF() {
  return tokens.GetFront().GetTokenType() != TokenType::Eof;
}

Token Parser::at() { return tokens.GetFront(); }

Token Parser::eat() { return tokens.Dequeue(); }

Token Parser::expect(TokenType tkType) {
  Token tk = eat();

  if (tk.GetTokenType() != tkType) {
    throw std::runtime_error("Expected " + TokenTypeToString(tkType) +
                             " but get " +
                             TokenTypeToString(tk.GetTokenType()));
  }

  return tk;
}
