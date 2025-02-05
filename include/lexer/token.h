#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>
#include <sstream>
#include <string>

enum class TokenType {
  Eof,

  Dot,
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
  OpenCurly,
  ClosingCurly,
  Colon,
  Comma,
  OpenSquare,
  ClosingSquare,
  Assignment,
  Equality,
  InEquality,
  Greater,
  Lower,
  GreaterEqual,
  LowerEqual,
  And,
  Or,

  Number,
  Identifier,
  Var,
  Mut,
  String,
  Fun,
  Return,
  If,
  Else,
  For,
  Break,
  Continue,

  Invalid,
};

std::string TokenTypeToString(TokenType);

struct Token {
private:
  std::string value;
  TokenType tokenType;

public:
  std::string GetValue() { return value; }
  TokenType GetTokenType() { return tokenType; }

  Token(std::string value, TokenType tokenType)
      : value(value), tokenType(tokenType) {}

  std::string Yaml() {
    std::stringstream ss;

    ss << "Token:" << std::endl
       << "    Value: " << value << std::endl
       << "    TokenType: " << TokenTypeToString(tokenType);

    return ss.str();
  }
};

#endif // !TOKEN_H
