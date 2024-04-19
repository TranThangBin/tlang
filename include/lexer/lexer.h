#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <queue>
#include <string>

struct Lexer {
private:
  std::string src;
  int srcLen;
  int i;

  std::string getNumber();
  std::string getIdent();
  Token checkLiteral(std::string);
  Token checkReserve(std::string);

public:
  Lexer(std::string src) : src(src) {}

  std::queue<Token> Tokenize();
  void SetSrc(std::string src) { this->src = src; }
};

#endif // !LEXER_H
