#include "lexer/lexer.h"
#include "lexer/token.h"
#include <map>
#include <stdexcept>
#include <string>

Lexer::Lexer(std::string src) : src(src) {
  literal = {
      {".", Token(".", TokenType::Dot)},
      {"&&", Token("&&", TokenType::And)},
      {"||", Token("||", TokenType::Or)},
      {">=", Token(">=", TokenType::GreaterEqual)},
      {"<=", Token("<=", TokenType::LowerEqual)},
      {">", Token(">", TokenType::Greater)},
      {"<", Token("<", TokenType::Lower)},
      {"==", Token("==", TokenType::Equality)},
      {"!=", Token("!=", TokenType::InEquality)},
      {"[", Token("[", TokenType::OpenSquare)},
      {"]", Token("]", TokenType::ClosingSquare)},
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
      {"{", Token("{", TokenType::OpenCurly)},
      {"}", Token("}", TokenType::ClosingCurly)},
      {")", Token(")", TokenType::ClosingParen)},
      {"(", Token("(", TokenType::OpenParen)},
      {"=", Token("=", TokenType::Equal)},
      {":", Token(":", TokenType::Colon)},
      {",", Token(",", TokenType::Comma)},
  };

  reserve = {
      {"var", Token("var", TokenType::Var)},
      {"mut", Token("mut", TokenType::Mut)},
      {"fun", Token("fun", TokenType::Fun)},
      {"return", Token("return", TokenType::Return)},
  };
}

Token Lexer::getLiteral() {
  Token token = Token("", TokenType::Invalid);

  int maxLiteralLen = 2;
  int literalLen = 1;

  while (pos + literalLen - 1 < srcLen && literalLen <= maxLiteralLen) {
    auto it = literal.find(src.substr(pos, literalLen));

    if (it != literal.end()) {
      token = it->second;
    }

    literalLen++;
  }

  return token;
}

Token Lexer::getNumber() {
  int end = pos;

  while (end < srcLen && isdigit(src[end + 1]) != 0) {
    end++;
  }

  return Token(src.substr(pos, end - pos + 1), TokenType::Number);
}

Token Lexer::getIdent() {
  int end = pos;

  while (end < srcLen && isalpha(src[end + 1]) != 0 || src[end + 1] == '_' ||
         isdigit(src[end + 1]) != 0) {
    end++;
  }

  std::string ident = src.substr(pos, end - pos + 1);
  auto it = reserve.find(ident);

  if (it != reserve.end()) {
    return it->second;
  }

  return Token(ident, TokenType::Identifier);
}

Token Lexer::getString() {
  int end = pos;

  while (end < srcLen && src[end + 1] != '"') {
    end++;
  }

  if (end >= srcLen || src[end + 1] != '"') {
    throw std::runtime_error("Unterminated string literal");
  }

  return Token(src.substr(pos + 1, end - pos), TokenType::String);
}
