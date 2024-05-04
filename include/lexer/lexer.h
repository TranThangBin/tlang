#ifndef LEXER_H
#define LEXER_H
#define LEXER_VERSION "v0.0.9"

#include "datastructure/queue.h"
#include "token.h"
#include <map>
#include <string>

struct Lexer {
private:
  std::string src;
  int srcLen;
  int pos;
  std::map<std::string, Token> literal;
  std::map<std::string, Token> reserve;

  Token getLiteral();
  Token getNumber();
  Token getIdent();
  Token getString();

public:
  Lexer(std::string);

  Queue<Token> &Tokenize();

  void SetSrc(std::string src) { this->src = src; }
};

#endif // !LEXER_H
