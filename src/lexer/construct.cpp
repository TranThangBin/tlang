#include "lexer/lexer.h"
#include "lexer/token.h"
#include <map>
#include <string>

std::map<std::string, Token> getLiteral() {
  return std::map<std::string, Token>{
      {
          "=",
          Token("=", TokenType::Equal),
      },
      {
          "(",
          Token("(", TokenType::OpenParen),
      },
      {
          ")",
          Token(")", TokenType::ClosingParen),
      },
      {
          ";",
          Token(";", TokenType::SemiColon),
      },
      {
          "+",
          Token("+", TokenType::Binop),
      },
      {
          "-",
          Token("-", TokenType::Binop),
      },
      {
          "*",
          Token("*", TokenType::Binop),
      },
      {
          "/",
          Token("/", TokenType::Binop),
      },
      {
          "%",
          Token("%", TokenType::Binop),
      },
  };
}

std::map<std::string, Token> getReserve() {
  return std::map<std::string, Token>{
      {
          "var",
          Token("var", TokenType::Var),
      },
      {
          "mut",
          Token("mut", TokenType::Mut),
      },
  };
}

Lexer::Lexer() : src(), literal(getLiteral()), reserve(getReserve()) {}

Lexer::Lexer(std::string src)
    : src(src), literal(getLiteral()), reserve(getReserve()) {}
