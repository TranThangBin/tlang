#include "lexer/token.h"
#include <string>

std::string TokenTypeToString(TokenType tokenType) {
  switch (tokenType) {
  case TokenType::Colon:
    return ":";

  case TokenType::Comma:
    return ",";

  case TokenType::OpenSquare:
    return "[";

  case TokenType::ClosingSquare:
    return "]";

  case TokenType::String:
    return "string";

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

  case TokenType::Percent:
    return "%";

  case TokenType::Exclamation:
    return "!";

  case TokenType::Number:
    return "number";

  case TokenType::Identifier:
    return "identifier";

  case TokenType::Var:
    return "var";

  case TokenType::Mut:
    return "mut";

  case TokenType::Assignment:
    return "assignment";

  case TokenType::OpenCurly:
    return "{";

  case TokenType::ClosingCurly:
    return "}";

  case TokenType::Invalid:
    return "invalid";
  }

  return "unknown";
}
