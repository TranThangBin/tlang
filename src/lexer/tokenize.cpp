#include "lexer/lexer.h"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <map>

std::vector<Token> Lexer::Tokenize() {
  std::map<std::string, Token> reserve = std::map<std::string, Token>{
      {
          "var",
          Token("var", TokenType::Var),
      },
      {
          "mut",
          Token("mut", TokenType::Mut),
      },
  };

  std::vector<Token> tokens = std::vector<Token>();

  int srcLen = src.length();

  for (int i = 0; i < srcLen; i++) {
    char curChar = src[i];

    if (curChar >= 0 && curChar <= 32) {

      continue;
    } else if (curChar == '=') {

      tokens.push_back(Token(std::string(1, curChar), TokenType::Equal));
    } else if (curChar == '(') {

      tokens.push_back(Token(std::string(1, curChar), TokenType::OpenParen));
    } else if (curChar == ')') {

      tokens.push_back(Token(std::string(1, curChar), TokenType::ClosingParen));
    } else if (curChar == ';') {

      tokens.push_back(Token(std::string(1, curChar), TokenType::SemiColon));
    } else if (curChar == '+' || curChar == '-' || curChar == '*' ||
               curChar == '/' || curChar == '%') {

      tokens.push_back(
          Token(std::string(1, curChar), TokenType::BinaryOperator));
    } else if (isdigit(curChar) != 0) {

      std::string number = std::string(1, curChar);

      while (i < srcLen && isdigit(src[i + 1]) != 0) {
        number += src[++i];
      }

      tokens.push_back(Token(number, TokenType::Number));
    } else if (isalpha(curChar) != 0 || curChar == '_') {

      std::string ident = std::string(1, curChar);

      while (i < srcLen && isalpha(src[i + 1]) != 0 || src[i + 1] == '_' ||
             isdigit(src[i + 1]) != 0) {
        ident += src[++i];
      }

      auto it = reserve.find(ident);

      if (it != reserve.end()) {
        tokens.push_back(it->second);
        continue;
      }

      tokens.push_back(Token(ident, TokenType::Identifier));
    } else {

      std::cerr << "Unexpected token " << curChar << std::endl;
      exit(1);
    }
  }

  tokens.push_back(Token("EOF", TokenType::EOF_));

  return tokens;
}
