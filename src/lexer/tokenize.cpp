#include "lexer/lexer.h"
#include "lexer/token.h"
#include <cctype>
#include <queue>
#include <stdexcept>
#include <string>

std::queue<Token> Lexer::Tokenize() {
  std::queue<Token> tokens;

  srcLen = src.length();

  for (i = 0; i < srcLen; i++) {
    char curChar = src[i];

    if (curChar >= 0 && curChar <= 32) {
      continue;
    }

    Token checkToken = checkLiteral(std::string(1, curChar));

    if (checkToken.GetTokenType() != TokenType::Invalid) {
      int nextCharIndex = 1;

      while (i + nextCharIndex < srcLen) {
        Token token = checkLiteral(src.substr(i, nextCharIndex + 1));

        if (token.GetTokenType() == TokenType::Invalid) {
          break;
        }

        checkToken = token;
        nextCharIndex++;
      }

      tokens.push(checkToken);
      i += nextCharIndex - 1;
      continue;
    }

    if (isdigit(curChar) != 0) {
      std::string number = getNumber();
      tokens.push(Token(number, TokenType::Number));
      continue;
    }

    if (isalpha(curChar) != 0 || curChar == '_') {
      std::string ident = getIdent();

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

  tokens.push(Token("", TokenType::Eof));

  return tokens;
}
