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

  Invalid,
};

struct Token {
private:
  std::string value;
  TokenType tokenType;

public:
  Token() : value(""), tokenType(TokenType::Invalid) {}
  Token(std::string value, TokenType tkType)
      : value(value), tokenType(tkType) {}

  std::string GetValue() { return value; }
  TokenType GetTokenType() { return tokenType; }

  std::string Yaml() {
    std::stringstream oss;
    oss << "Token:" << std::endl
        << "\tValue: " << value << std::endl
        << "\tTokenType: " << (int)tokenType;
    return oss.str();
  }
};

#endif // !TOKEN_H
