#include "lexer/lexer.h"
#include "lexer/token.h"
#include <string>

Token Lexer::checkLiteral(std::string str) {
  if (str == "=") {
    return Token("", TokenType::Equal);
  }

  if (str == "(") {
    return Token("", TokenType::OpenParen);
  }

  if (str == ")") {
    Token("", TokenType::ClosingParen);
  }

  if (str == ";") {
    return Token("", TokenType::SemiColon);
  }

  if (str == "+") {
    return Token("", TokenType::Plus);
  }

  if (str == "-") {
    return Token("", TokenType::Minus);
  }

  if (str == "*") {
    return Token("", TokenType::Asterisk);
  }

  if (str == "/") {
    return Token("", TokenType::FowardSlash);
  }

  if (str == "%") {
    return Token("", TokenType::Percent);
  }

  if (str == "+=") {
    return Token("", TokenType::AdditionAssignment);
  }

  if (str == "-=") {
    return Token("", TokenType::SubtractionAssignment);
  }

  if (str == "*=") {
    return Token("", TokenType::MultiplicationAssignment);
  }

  if (str == "/=") {
    return Token("", TokenType::DivisionAssignment);
  }

  if (str == "%=") {
    return Token("", TokenType::ModulusAssignment);
  }

  if (str == "!") {
    return Token("", TokenType::Exclamation);
  }

  return Token("", TokenType::Invalid);
}

Token Lexer::checkReserve(std::string str) {
  if (str == "var") {
    return Token("", TokenType::Var);
  }

  if (str == "mut") {
    return Token("", TokenType::Mut);
  }

  return Token("", TokenType::Invalid);
}

std::string Lexer::getNumber() {
  int start = i;

  while (i < srcLen && isdigit(src[i + 1]) != 0) {
    i++;
  }

  return src.substr(start, i - start + 1);
}

std::string Lexer::getIdent() {
  int start = i;

  while (i < srcLen && isalpha(src[i + 1]) != 0 || src[i + 1] == '_' ||
         isdigit(src[i + 1]) != 0) {
    i++;
  }

  return src.substr(start, i - start + 1);
}
