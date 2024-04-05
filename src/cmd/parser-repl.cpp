#include "lexer/lexer.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <iostream>
#include <memory>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  string src;
  shared_ptr<Lexer> lexer = make_shared<Lexer>();
  Parser parser = Parser(lexer);
  cout << "Parser explorer v0.0.1" << endl;

  while (true) {
    cout << "> ";
    getline(cin, src);

    lexer->SetSrc(src);

    try {
      unique_ptr<ProgramNode> program = parser.ProduceAST();

      program->Display();

    } catch (runtime_error err) {
      cerr << err.what() << endl;
      continue;
    }
  }

  return 0;
}
