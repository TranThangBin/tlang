#include "datastructure/queue.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include <iostream>
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

    Queue<Token> tokens = Queue<Token>();

    try {
      tokens = lexer.Tokenize();
    } catch (runtime_error err) {
      cerr << err.what() << endl;
      continue;
    }

    while (!tokens.IsEmpty()) {
      cout << tokens.Dequeue().Yaml() << endl;
    }

    cout << endl;
  }
  return 0;
}
