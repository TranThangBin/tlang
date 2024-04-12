#include "lexer/lexer.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  string src;
  Lexer lexer = Lexer();

  cout << "Lexer explorer v0.0.1" << endl;

  while (true) {
    cout << "> ";
    getline(cin, src);

    lexer.SetSrc(src);

    vector<Token> tokens;

    try {
      tokens = lexer.Tokenize();
    } catch (runtime_error err) {
      cerr << err.what() << endl;
      continue;
    }

    int tkSize = tokens.size();

    for (int i = 0; i < tkSize; i++) {
      Token token = tokens[i];

      cout << token.Yaml() << endl;
    }
    cout << endl;
  }
  return 0;
}
