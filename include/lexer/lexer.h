#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <vector>

struct Lexer {
private:
  std::string src;

public:
  Lexer() : src("") {}
  Lexer(std::string src) : src(src) {}

  std::vector<Token> Tokenize();
};

#endif // !LEXER_H
