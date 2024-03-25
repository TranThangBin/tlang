#ifndef TOKEN_H
#define TOKEN_H

#include <string>
enum TokenType {
  EOF_,

  BinaryOperator,
  Equal,
  OpenParen,
  ClosingParen,
  SemiColon,

  Number,
  Identifier,
  Var,
  Mut,
};

struct Token {
private:
  std::string value;
  TokenType tkType;

public:
  Token(std::string value, TokenType tkType) : value(value), tkType(tkType) {}

  std::string getValue() { return value; }
  TokenType getTkType() { return tkType; }
};

#endif // !TOKEN_H
