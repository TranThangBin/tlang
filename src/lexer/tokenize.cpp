#include "lexer/lexer.h"
#include "lexer/token.h"
#include <cctype>
#include <queue>
#include <stdexcept>
#include <string>

std::queue<Token> Lexer::Tokenize() {
  std::queue<Token> tokens;

  int srcLen = src.length();

  for (i = 0; i < srcLen; i++) {
    char curChar = src[i];

    if (curChar >= 0 && curChar <= 32) {
      continue;
    }

    Token checkToken = checkLiteral(std::string(1, curChar));

    if (checkToken.GetTokenType() != TokenType::Invalid) {
      if (i < srcLen) {
        Token token = checkLiteral(src.substr(i, 2));

        if (token.GetTokenType() != TokenType::Invalid) {
          checkToken = token;
        }
      }

      tokens.push(checkToken);
      i += checkToken.GetValue().length() - 1;
      continue;
    }

    if (isdigit(curChar) != 0) {
      tokens.push(Token(getNumber(), TokenType::Number));
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

  tokens.push(Token("EOF", TokenType::Eof));

  return tokens;
}
