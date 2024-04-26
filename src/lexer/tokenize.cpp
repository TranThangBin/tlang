#include "lexer/lexer.h"
#include "lexer/token.h"
#include <cctype>
#include <queue>
#include <stdexcept>
#include <string>

std::queue<Token> Lexer::Tokenize() {
  std::queue<Token> tokens;

  srcLen = src.length();

  i = 0;

  while (i < srcLen) {
    char curChar = src[i];

    if (curChar >= 0 && curChar <= 32) {
      i++;
      continue;
    }

    Token literal = getLiteral();

    if (literal.GetTokenType() != TokenType::Invalid) {
      tokens.push(literal);
      i += literal.GetValue().length();
      continue;
    }

    if (isdigit(curChar) != 0) {
      Token number = getNumber();
      tokens.push(number);
      i += number.GetValue().length();
      continue;
    }

    if (isalpha(curChar) != 0 || curChar == '_') {
      Token ident = getIdent();
      tokens.push(ident);
      i += ident.GetValue().length();
      continue;
    }

    throw std::runtime_error("Unexpected symbol " + std::string(1, curChar));
  }

  tokens.push(Token("EOF", TokenType::Eof));

  return tokens;
}
