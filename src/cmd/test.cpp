#include "datastructure/queue.h"
#include "lexer/token.h"
#include <iostream>
#include <string>

using namespace std;

Queue<Token> makeTokenQueue() {
  Queue<Token> tokens = Queue<Token>();
  tokens.Enqueue(Token("Hello", TokenType::Identifier));
  tokens.Enqueue(Token("World", TokenType::Identifier));
  return tokens;
}

int main(int argc, char *argv[]) {

  Queue<Token> strings = makeTokenQueue();

  while (!strings.IsEmpty()) {
    cout << strings.Dequeue().Yaml() << endl;
  }

  return 0;
}
