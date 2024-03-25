#include "lexer/lexer.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  string src;

  getline(cin, src);

  Lexer lexer = Lexer(src);

  vector<Token> tokens = lexer.Tokenize();

  int tkSize = tokens.size();

  for (int i = 0; i < tkSize; i++) {
    Token token = tokens[i];

    cout << "Token type: " << token.getTkType()
         << " Token value: " << token.getValue() << endl;
  }

  return 0;
}
