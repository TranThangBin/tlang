#include "lexer/lexer.h"
#include "lexer/token.h"
#include <cctype>
#include <queue>
#include <stdexcept>
#include <string>

std::queue<Token> Lexer::Tokenize() {
  std::queue<Token> tokens;

  int srcLen = src.length();

  for (int i = 0; i < srcLen; i++) {
    char curChar = src[i];

    if (curChar >= 0 && curChar <= 32) {
      continue;
    }

    Token checkToken = checkReserve(std::string(1, curChar));

    if (checkToken.GetTokenType() != TokenType::Invalid) {
      tokens.push(checkToken);
      continue;
    }

    if (isdigit(curChar) != 0) {
      int start = i;
      while (i < srcLen && isdigit(src[i + 1]) != 0) {
        i++;
      }
      tokens.push(Token(src.substr(start, i - start + 1), TokenType::Number));
      continue;
    }

    if (isalpha(curChar) != 0 || curChar == '_') {
      int start = i;

      while (i < srcLen && isalpha(src[i + 1]) != 0 || src[i + 1] == '_' ||
             isdigit(src[i + 1]) != 0) {
        i++;
      }

      std::string ident = src.substr(start, i - start + 1);
      checkToken = checkReserve(ident);

      if (checkToken.GetTokenType() != TokenType::Invalid) {
        tokens.push(checkToken);
        continue;
      }

      tokens.push(Token(ident, TokenType::Identifier));
      continue;
    }

    throw std::runtime_error("Unexpected symbol " + std::string(1, curChar));
  }

  tokens.push(Token("EOF", TokenType::eof));

  return tokens;
}
