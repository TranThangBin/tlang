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
  std::shared_ptr<Lexer> lexer = std::make_shared<Lexer>("");
  std::unique_ptr<Parser> parser = std::make_unique<Parser>(lexer);
  std::unique_ptr<Environment> environment = std::make_unique<Environment>();
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

    value->out();
    cout << endl;
  }

  return 0;
}
