#include "lexer/lexer.h"
#include "lexer/token.h"
#include <map>
#include <string>

Lexer::Lexer(std::string src) : src(src) {
  literal = {
      {"!", Token("!", TokenType::Exclamation)},
      {"%=", Token("%=", TokenType::Assignment)},
      {"/=", Token("/=", TokenType::Assignment)},
      {"*=", Token("*=", TokenType::Assignment)},
      {"-=", Token("-=", TokenType::Assignment)},
      {"+=", Token("+=", TokenType::Assignment)},
      {"%", Token("%", TokenType::Percent)},
      {"/", Token("/", TokenType::FowardSlash)},
      {"*", Token("*", TokenType::Asterisk)},
      {"-", Token("-", TokenType::Minus)},
      {"+", Token("+", TokenType::Plus)},
      {";", Token(";", TokenType::SemiColon)},
      {"}", Token("}", TokenType::ClosingCurly)},
      {")", Token(")", TokenType::ClosingParen)},
      {"(", Token("(", TokenType::OpenParen)},
      {"=", Token("=", TokenType::Equal)},
  };

  reserve = {
      {"var", Token("var", TokenType::Var)},
      {"mut", Token("mut", TokenType::Mut)},
  };
}

Token Lexer::getLiteral() {
  Token token = Token("", TokenType::Invalid);

  int maxLiteralLen = 2;
  int literalLen = 1;

  while (i + literalLen - 1 < srcLen && literalLen <= maxLiteralLen) {
    auto it = literal.find(src.substr(i, literalLen));

    if (it != literal.end()) {
      token = it->second;
    }

    literalLen++;
  }

  return token;
}

Token Lexer::getNumber() {
  int end = i;

  while (end < srcLen && isdigit(src[end + 1]) != 0) {
    end++;
  }

  return Token(src.substr(i, end - i + 1), TokenType::Number);
}

Token Lexer::getIdent() {
  int end = i;

  while (end < srcLen && isalpha(src[end + 1]) != 0 || src[end + 1] == '_' ||
         isdigit(src[end + 1]) != 0) {
    end++;
  }

  std::string ident = src.substr(i, end - i + 1);
  auto it = reserve.find(ident);

  if (it != reserve.end()) {
    return it->second;
  }

  return Token(ident, TokenType::Identifier);
}
