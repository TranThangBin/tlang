#include "lexer/token.h"
#include "parser/parser.h"
#include <sstream>
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
    std::stringstream ss;
    ss << "Expected " << (int)tkType << " but get " << (int)tk.GetTokenType();
    throw std::runtime_error(ss.str());
  }

  return tk;
}
