#include "lexer/lexer.h"
#include "parser/parser.h"
#include "runtime/environment.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

int main(int argc, char *argv[]) {
  string src;
  shared_ptr<Lexer> lexer = make_shared<Lexer>("");
  unique_ptr<Parser> parser = make_unique<Parser>(lexer);
  unique_ptr<Environment> environment = make_unique<Environment>();
  Interpreter interpreter =
      Interpreter(std::move(parser), std::move(environment));

  cout << "Interpreter explorer " << INTERPRETER_VERSION << endl;

  while (true) {
    cout << "> ";
    getline(cin, src);

    lexer->SetSrc(src);

    shared_ptr<RuntimeValue> value;

    try {
      value = interpreter.Evaluate();
    } catch (runtime_error err) {
      cerr << err.what() << endl;
      continue;
    }

    cout << endl;
    value->out();
    cout << endl;
  }

  return 0;
}
