#include "lexer/lexer.h"
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

int main(int argc, char *argv[]) {
  string src;
  Lexer lexer = Lexer();

  cout << "Lexer explorer v0.0.1" << endl;

  while (true) {
    cout << "> ";
    getline(cin, src);

    lexer.SetSrc(src);

    queue<Token> tokens;

    try {
      tokens = std::move(lexer.Tokenize());
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
