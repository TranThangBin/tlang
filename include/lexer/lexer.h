#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <map>
#include <string>
#include <vector>

struct Lexer {
private:
  std::map<std::string, Token> literal;
  std::map<std::string, Token> reserve;
  std::string src;

public:
  Lexer();
  Lexer(std::string);

  std::vector<Token> Tokenize();
  void SetSrc(std::string src) { this->src = src; }
};

#endif // !LEXER_H
