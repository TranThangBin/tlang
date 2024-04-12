#include "lexer/token.h"
#include "parser/parser.h"
#include <sstream>
#include <stdexcept>

bool Parser::notEOF() { return tokens[0].GetTkType() != TokenType::Tk_EOF; }

Token Parser::at() { return tokens[0]; }

Token Parser::eat() {
  Token front = tokens.front();
  tokens.erase(tokens.begin());
  return front;
}

Token Parser::expect(TokenType tkType) {
  Token tk = eat();

  if (tk.GetTkType() != tkType) {
    std::stringstream ss;
    ss << "Expected " << tkType << " but get " << tk.GetTkType();
    throw std::runtime_error(ss.str());
  }

  return tk;
}
