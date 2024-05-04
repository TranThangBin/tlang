#include "datastructure/queue.h"
#include "runtime/environment.h"
#include "runtime/runtime-value.h"
#include <iostream>
#include <memory>

using namespace std;

int main(int argc, char *argv[]) {
  unique_ptr<Environment> env = make_unique<Environment>();
  env->DeclareVariable("hello", make_shared<StringValue>("Hello"), true);
  env->LookUpVar("hello")->out();
  env->AssignVariable("hello", make_shared<NumberValue>(10));
  env->LookUpVar("hello")->out();

  return 0;
}
