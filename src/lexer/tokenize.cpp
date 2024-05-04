#include "datastructure/queue.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include <cctype>
#include <stdexcept>
#include <string>

Queue<Token> &Lexer::Tokenize() {
  Queue<Token> *tokens = new Queue<Token>();

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
      tokens->Enqueue(literal);
      pos += literal.GetValue().length();
      continue;
    }

    if (isdigit(curChar) != 0) {
      Token number = getNumber();
      tokens->Enqueue(number);
      pos += number.GetValue().length();
      continue;
    }

    if (curChar == '"' || curChar == '\'') {
      Token string = getString();
      tokens->Enqueue(string);
      pos += string.GetValue().length() + 2;
      continue;
    }

    if (isalpha(curChar) != 0 || curChar == '_') {
      Token ident = getIdent();
      tokens->Enqueue(ident);
      pos += ident.GetValue().length();
      continue;
    }

    throw std::runtime_error("Unexpected symbol " + std::string(1, curChar));
  }

  tokens->Enqueue(Token("EOF", TokenType::Eof));

  return *tokens;
}
