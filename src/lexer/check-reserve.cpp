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
        Token("+", TokenType::Plus),
    },

    {
        "-",
        Token("-", TokenType::Minus),
    },

    {
        "*",
        Token("*", TokenType::Asterisk),
    },

    {
        "/",
        Token("/", TokenType::FowardSlash),
    },

    {
        "%",
        Token("%", TokenType::Percent),
    },

    {
        "+=",
        Token("+=", TokenType::AdditionAssignment),
    },

    {
        "-=",
        Token("-=", TokenType::SubtractionAssignment),
    },

    {
        "*=",
        Token("*=", TokenType::MultiplicationAssignment),
    },

    {
        "/=",
        Token("/=", TokenType::DivisionAssignment),
    },

    {
        "%=",
        Token("%=", TokenType::ModulusAssignment),
    },

    {
        "!",
        Token("!", TokenType::Exclamation),
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
    return Token("", TokenType::Invalid);
  }

  return it->second;
}
