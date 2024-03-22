#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum TokenType {
  EOF_,
};

struct Token {
public:
  const std::string value;
  const TokenType tkType;
  Token(std::string value, TokenType tkType) : value(value), tkType(tkType) {}
};

struct Lexer {
private:
  std::string src;

public:
  Lexer(std::string src) : src(src) {}

  std::vector<Token> Tokenize();
};

#endif // !LEXER_H
