#include "lexer.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  Lexer lexer = Lexer("Hello");

  vector<Token> tokens = lexer.Tokenize();

  int tkSize = tokens.size();

  for (int i = 0; i < tkSize; i++) {
    Token token = tokens[i];

    cout << "Token type: " << token.tkType << "Token value: " << token.value
         << endl;
  }

  return 0;
}
