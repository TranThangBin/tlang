#include "lexer/lexer.h"
#include "parser/parser.h"
#include "runtime/environment.h"
#include "runtime/interpreter.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }

  ifstream fileStream(argv[1]);

  if (!fileStream.is_open()) {
    cerr << "There are no file with name " << argv[1] << std::endl;
    return 1;
  }

  string src((istreambuf_iterator<char>(fileStream)),
             istreambuf_iterator<char>());

  shared_ptr<Lexer> lexer = make_shared<Lexer>(src);
  unique_ptr<Parser> parser = make_unique<Parser>(lexer);
  unique_ptr<Environment> env = make_unique<Environment>();
  Interpreter interpreter = Interpreter(std::move(parser), std::move(env));

  try {
    interpreter.Evaluate();
  } catch (runtime_error err) {
    cerr << err.what() << endl;
    fileStream.close();
    return 1;
  }

  fileStream.close();

  return 0;
}
