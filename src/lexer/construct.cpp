#include "lexer/lexer.h"
#include "lexer/token.h"
#include <map>
#include <string>

std::map<std::string, Token> getLiteral() {
  return std::map<std::string, Token>{
      {
          "=",
          Token("=", TokenType::Tk_Equal),
      },
      {
          "(",
          Token("(", TokenType::Tk_OpenParen),
      },
      {
          ")",
          Token(")", TokenType::Tk_ClosingParen),
      },
      {
          ";",
          Token(";", TokenType::Tk_SemiColon),
      },
      {
          "+",
          Token("+", TokenType::Tk_Binop),
      },
      {
          "-",
          Token("-", TokenType::Tk_Binop),
      },
      {
          "*",
          Token("*", TokenType::Tk_Binop),
      },
      {
          "/",
          Token("/", TokenType::Tk_Binop),
      },
      {
          "%",
          Token("%", TokenType::Tk_Binop),
      },
  };
}

std::map<std::string, Token> getReserve() {
  return std::map<std::string, Token>{
      {
          "var",
          Token("var", TokenType::Tk_Var),
      },
      {
          "mut",
          Token("mut", TokenType::Tk_Mut),
      },
  };
}

Lexer::Lexer() : src(), literal(getLiteral()), reserve(getReserve()) {}

Lexer::Lexer(std::string src)
    : src(src), literal(getLiteral()), reserve(getReserve()) {}
