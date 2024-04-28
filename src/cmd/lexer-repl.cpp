#include "lexer/lexer.h"
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  string src;
  Lexer lexer = Lexer("");

  cout << "Lexer explorer " << LEXER_VERSION << endl;

  while (true) {
    cout << "> ";
    getline(cin, src);

    lexer.SetSrc(src);

    queue<Token> tokens;

    try {
      tokens = lexer.Tokenize();
    } catch (runtime_error err) {
      cerr << err.what() << endl;
      continue;
    }

    int tkSize = tokens.size();

    while (tokens.size() != 0) {
      cout << tokens.front().Yaml() << endl;
      tokens.pop();
    }

    cout << endl;
  }
  return 0;
}
