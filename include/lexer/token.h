#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>
#include <sstream>
#include <string>
enum class TokenType {
  eof,

  Binop,
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

  std::string GetValue() { return value; }
  TokenType GetTkType() { return tkType; }

  std::string Yaml() {
    std::stringstream oss;
    oss << "Token:" << std::endl
        << "\tValue: " << value << std::endl
        << "\tTokenType: " << (int)tkType;
    return oss.str();
  }
};

#endif // !TOKEN_H
