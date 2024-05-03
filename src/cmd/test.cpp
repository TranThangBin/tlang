#include "runtime/environment.h"
#include <iostream>
#include <memory>
int main(int argc, char *argv[]) {
  std::unique_ptr<Environment> env = std::make_unique<Environment>();
  std::cout<<env->HasContext(EnvironmentContext::Global);

  return 0;
}
