#include "lexer/lexer.h"
#include "lexer/token.h"
#include <cctype>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>

std::queue<Token> Lexer::Tokenize() {
  std::queue<Token> tokens;

  int srcLen = src.length();

  for (int i = 0; i < srcLen; i++) {
    char curChar = src[i];
    Token checkToken = checkReserve(std::string(1, curChar));

    if (curChar >= 0 && curChar <= 32) {
      continue;
    } else if (checkToken.GetTokenType() != TokenType::Invalid) {
      tokens.push(checkToken);
    } else if (isdigit(curChar) != 0) {

      std::stringstream number;
      number << curChar;

      while (i < srcLen && isdigit(src[i + 1]) != 0) {
        number << src[++i];
      }

      tokens.push(Token(number.str(), TokenType::Number));

    } else if (isalpha(curChar) != 0 || curChar == '_') {

      std::stringstream ident;
      ident << curChar;

      while (i < srcLen && isalpha(src[i + 1]) != 0 || src[i + 1] == '_' ||
             isdigit(src[i + 1]) != 0) {
        ident << src[++i];
      }

      checkToken = checkReserve(ident.str());

      if (checkToken.GetTokenType() != TokenType::Invalid) {
        tokens.push(checkToken);
        continue;
      }

      tokens.push(Token(ident.str(), TokenType::Identifier));

    } else {
      throw std::runtime_error("Unexpected symbol " + std::string(1, curChar));
    }
  }

  tokens.push(Token("EOF", TokenType::eof));

  return tokens;
}
