#include "lexer/lexer.h"
#include "datastructure/hashmap.h"
#include "lexer/token.h"
#include <stdexcept>
#include <string>

Lexer::Lexer(std::string src) : src(src), literal(50), reserve(50) {

  literal.Insert(".", Token(".", TokenType::Dot));
  literal.Insert("&&", Token("&&", TokenType::And));
  literal.Insert("||", Token("||", TokenType::Or));
  literal.Insert(">=", Token(">=", TokenType::GreaterEqual));
  literal.Insert("<=", Token("<=", TokenType::LowerEqual));
  literal.Insert(">", Token(">", TokenType::Greater));
  literal.Insert("<", Token("<", TokenType::Lower));
  literal.Insert("==", Token("==", TokenType::Equality));
  literal.Insert("!=", Token("!=", TokenType::InEquality));
  literal.Insert("[", Token("[", TokenType::OpenSquare));
  literal.Insert("]", Token("]", TokenType::ClosingSquare));
  literal.Insert("!", Token("!", TokenType::Exclamation));
  literal.Insert("%=", Token("%=", TokenType::Assignment));
  literal.Insert("/=", Token("/=", TokenType::Assignment));
  literal.Insert("*=", Token("*=", TokenType::Assignment));
  literal.Insert("-=", Token("-=", TokenType::Assignment));
  literal.Insert("+=", Token("+=", TokenType::Assignment));
  literal.Insert("%", Token("%", TokenType::Percent));
  literal.Insert("/", Token("/", TokenType::FowardSlash));
  literal.Insert("*", Token("*", TokenType::Asterisk));
  literal.Insert("-", Token("-", TokenType::Minus));
  literal.Insert("+", Token("+", TokenType::Plus));
  literal.Insert(";", Token(";", TokenType::SemiColon));
  literal.Insert("{", Token("{", TokenType::OpenCurly));
  literal.Insert("}", Token("}", TokenType::ClosingCurly));
  literal.Insert(")", Token(")", TokenType::ClosingParen));
  literal.Insert("(", Token("(", TokenType::OpenParen));
  literal.Insert("=", Token("=", TokenType::Equal));
  literal.Insert(":", Token(":", TokenType::Colon));
  literal.Insert(",", Token(",", TokenType::Comma));

  // {
  //     {".", Token(".", TokenType::Dot)},
  //     {"&&", Token("&&", TokenType::And)},
  //     {"||", Token("||", TokenType::Or)},
  //     {">=", Token(">=", TokenType::GreaterEqual)},
  //     {"<=", Token("<=", TokenType::LowerEqual)},
  //     {">", Token(">", TokenType::Greater)},
  //     {"<", Token("<", TokenType::Lower)},
  //     {"==", Token("==", TokenType::Equality)},
  //     {"!=", Token("!=", TokenType::InEquality)},
  //     {"[", Token("[", TokenType::OpenSquare)},
  //     {"]", Token("]", TokenType::ClosingSquare)},
  //     {"!", Token("!", TokenType::Exclamation)},
  //     {"%=", Token("%=", TokenType::Assignment)},
  //     {"/=", Token("/=", TokenType::Assignment)},
  //     {"*=", Token("*=", TokenType::Assignment)},
  //     {"-=", Token("-=", TokenType::Assignment)},
  //     {"+=", Token("+=", TokenType::Assignment)},
  //     {"%", Token("%", TokenType::Percent)},
  //     {"/", Token("/", TokenType::FowardSlash)},
  //     {"*", Token("*", TokenType::Asterisk)},
  //     {"-", Token("-", TokenType::Minus)},
  //     {"+", Token("+", TokenType::Plus)},
  //     {";", Token(";", TokenType::SemiColon)},
  //     {"{", Token("{", TokenType::OpenCurly)},
  //     {"}", Token("}", TokenType::ClosingCurly)},
  //     {")", Token(")", TokenType::ClosingParen)},
  //     {"(", Token("(", TokenType::OpenParen)},
  //     {"=", Token("=", TokenType::Equal)},
  //     {":", Token(":", TokenType::Colon)},
  //     {",", Token(",", TokenType::Comma)},
  // };

  reserve.Insert("var", Token("var", TokenType::Var));
  reserve.Insert("mut", Token("mut", TokenType::Mut));
  reserve.Insert("fun", Token("fun", TokenType::Fun));
  reserve.Insert("return", Token("return", TokenType::Return));
  reserve.Insert("if", Token("if", TokenType::If));
  reserve.Insert("else", Token("else", TokenType::Else));
  reserve.Insert("for", Token("for", TokenType::For));
  reserve.Insert("continue", Token("continue", TokenType::Continue));
  reserve.Insert("break", Token("break", TokenType::Break));

  // {
  //     {"var", Token("var", TokenType::Var)},
  //     {"mut", Token("mut", TokenType::Mut)},
  //     {"fun", Token("fun", TokenType::Fun)},
  //     {"return", Token("return", TokenType::Return)},
  //     {"if", Token("if", TokenType::If)},
  //     {"else", Token("else", TokenType::Else)},
  //     {"for", Token("for", TokenType::For)},
  //     {"continue", Token("continue", TokenType::Continue)},
  //     {"break", Token("break", TokenType::Break)},
  // };
}

Token Lexer::getLiteral() {
  Token token = Token("", TokenType::Invalid);

  int maxLiteralLen = 2;
  int literalLen = 1;

  while (pos + literalLen - 1 < srcLen && literalLen <= maxLiteralLen) {
    Pair<Token> *pair = literal.Find(src.substr(pos, literalLen));

    if (pair != nullptr) {
      token = pair->value;
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

  Pair<Token> *pair = reserve.Find(ident);

  if (pair != nullptr) {
    return pair->value;
  }

  return Token(ident, TokenType::Identifier);
}

Token Lexer::getString() {
  int end = pos;

  char endChar = src[pos];

  while (end < srcLen && src[end + 1] != endChar) {
    end++;
  }

  if (end >= srcLen || src[end + 1] != endChar) {
    throw std::runtime_error("Unterminated string literal");
  }

  return Token(src.substr(pos + 1, end - pos), TokenType::String);
}
