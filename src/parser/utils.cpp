#include "lexer/token.h"
#include "parser/parser.h"
#include <stdexcept>

bool Parser::notEOF() {
  return tokens.front().GetTokenType() != TokenType::Eof;
}

Token Parser::at() { return tokens.front(); }

Token Parser::eat() {
  Token token = tokens.front();
  tokens.pop();
  return token;
}

Token Parser::expect(TokenType tkType) {
  Token tk = eat();

  if (tk.GetTokenType() != tkType) {
    throw std::runtime_error("Expected " + TokenTypeToString(tkType) +
                             " but get " +
                             TokenTypeToString(tk.GetTokenType()));
  }

  return tk;
}
