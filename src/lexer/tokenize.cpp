#include "lexer/lexer.h"
#include "lexer/token.h"
#include <cctype>
#include <queue>
#include <stdexcept>
#include <string>

std::queue<Token> Lexer::Tokenize() {
  std::queue<Token> tokens;

  srcLen = src.length();

  pos = 0;

  while (pos < srcLen) {
    char curChar = src[pos];

    if (curChar >= 0 && curChar <= 32) {
      pos++;
      continue;
    }

    Token literal = getLiteral();

    if (literal.GetTokenType() != TokenType::Invalid) {
      tokens.push(literal);
      pos += literal.GetValue().length();
      continue;
    }

    if (isdigit(curChar) != 0) {
      Token number = getNumber();
      tokens.push(number);
      pos += number.GetValue().length();
      continue;
    }

    if (curChar == '"') {
      Token string = getString();
      tokens.push(string);
      pos += string.GetValue().length() + 2;
      continue;
    }

    if (isalpha(curChar) != 0 || curChar == '_') {
      Token ident = getIdent();
      tokens.push(ident);
      pos += ident.GetValue().length();
      continue;
    }

    throw std::runtime_error("Unexpected symbol " + std::string(1, curChar));
  }

  tokens.push(Token("EOF", TokenType::Eof));

  return tokens;
}
