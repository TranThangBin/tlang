#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>
#include <sstream>
#include <string>
enum class TokenType {
  Eof,

  Plus,
  Minus,
  Asterisk,
  FowardSlash,
  Percent,
  Exclamation,
  Equal,
  OpenParen,
  ClosingParen,
  SemiColon,
  BinaryAssignment,

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
    std::stringstream ss;
    ss << "Token:" << std::endl
       << "\tValue: " << value << std::endl
       << "\tTokenType: " << (int)tokenType;
    return ss.str();
  }
};

#endif // !TOKEN_H
