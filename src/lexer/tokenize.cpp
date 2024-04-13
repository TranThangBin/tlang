#include "lexer/lexer.h"
#include "lexer/token.h"
#include <cctype>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

std::vector<Token> Lexer::Tokenize() {
  std::vector<Token> tokens = std::vector<Token>();

  int srcLen = src.length();

  for (int i = 0; i < srcLen; i++) {
    char curChar = src[i];
    auto checkLiteral = literal.find(std::string(1, curChar));

    if (curChar >= 0 && curChar <= 32) {
      continue;
    } else if (checkLiteral != literal.end()) {
      tokens.push_back(checkLiteral->second);
    } else if (isdigit(curChar) != 0) {

      std::stringstream number;
      number << curChar;

      while (i < srcLen && isdigit(src[i + 1]) != 0) {
        number << src[++i];
      }

      tokens.push_back(Token(number.str(), TokenType::Number));

    } else if (isalpha(curChar) != 0 || curChar == '_') {

      std::stringstream ident;
      ident << curChar;

      while (i < srcLen && isalpha(src[i + 1]) != 0 || src[i + 1] == '_' ||
             isdigit(src[i + 1]) != 0) {
        ident << src[++i];
      }

      auto checkReserve = reserve.find(ident.str());

      if (checkReserve != reserve.end()) {
        tokens.push_back(checkReserve->second);
        continue;
      }

      tokens.push_back(Token(ident.str(), TokenType::Identifier));

    } else {
      throw std::runtime_error("Unexpected symbol " + std::string(1, curChar));
    }
  }

  tokens.push_back(Token("EOF", TokenType::eof));

  return tokens;
}
