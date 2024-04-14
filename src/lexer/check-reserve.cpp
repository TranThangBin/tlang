#include "lexer/lexer.h"
#include "lexer/token.h"
#include <map>
#include <string>

std::map<std::string, Token> reserve = std::map<std::string, Token>{
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
        Token("+", TokenType::BinaryOperator),
    },

    {
        "-",
        Token("-", TokenType::BinaryOperator),
    },

    {
        "*",
        Token("*", TokenType::BinaryOperator),
    },

    {
        "/",
        Token("/", TokenType::BinaryOperator),
    },

    {
        "%",
        Token("%", TokenType::BinaryOperator),
    },

    {
        "var",
        Token("var", TokenType::Var),
    },

    {
        "mut",
        Token("mut", TokenType::Mut),
    },
};

Token checkReserve(std::string str) {
  auto it = reserve.find(str);

  if (it == reserve.end()) {
    return Token();
  }

  return Token(it->second);
}
