#include "lexer/token.h"
#include <string>

std::string TokenTypeToString(TokenType tokenType) {
  switch (tokenType) {
  case TokenType::Eof:
    return "EOF";

  case TokenType::Plus:
    return "+";

  case TokenType::Minus:
    return "-";

  case TokenType::Asterisk:
    return "*";

  case TokenType::FowardSlash:
    return "/";

  case TokenType::OpenParen:
    return "(";

  case TokenType::ClosingParen:
    return ")";

  case TokenType::Equal:
    return "=";

  case TokenType::SemiColon:
    return ";";

  case TokenType::Number:
    return "number";

  case TokenType::Identifier:
    return "identifier";

  case TokenType::Var:
    return "var";

  case TokenType::Mut:
    return "mut";

  case TokenType::Invalid:
    return "invalid";

  case TokenType::AdditionAssignment:
    return "+=";

  case TokenType::SubtractionAssignment:
    return "-=";

  case TokenType::MultiplicationAssignment:
    return "*=";

  case TokenType::DivisionAssignment:
    return "/=";

  case TokenType::ModulusAssignment:
    return "%=";

  default:
    return "unidentified";
  }
}
