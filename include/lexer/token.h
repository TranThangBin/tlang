#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>
#include <sstream>
#include <string>
enum TokenType {
  Tk_EOF,

  Tk_Binop,
  Tk_Equal,
  Tk_OpenParen,
  Tk_ClosingParen,
  Tk_SemiColon,

  Tk_Number,
  Tk_Identifier,
  Tk_Var,
  Tk_Mut,
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
        << "\tValue " << value << std::endl
        << "\tTokenType " << tkType;
    return oss.str();
  }
};

#endif // !TOKEN_H
