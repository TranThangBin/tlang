#include "lexer/lexer.h"
#include "parser/parser.h"
#include "runtime/environment.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  string src;
  std::shared_ptr<Lexer> lexer = std::make_shared<Lexer>();
  std::shared_ptr<Parser> parser = std::make_shared<Parser>(lexer);
  std::shared_ptr<Environment> environment = std::make_shared<Environment>();
  Interpreter interpreter = Interpreter(parser, environment);

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

    cout << value->str() << endl;
  }

  return 0;
}
