#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "runtime-value.h"
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

struct Environment {
private:
  std::unique_ptr<Environment> parent;
  std::map<std::string, std::shared_ptr<RuntimeValue>> variables;
  std::set<std::string> mutables;

  Environment *resolve(std::string);

public:
  Environment() : parent(nullptr) {
    DeclareVariable("true", std::make_shared<BooleanValue>(true), false);
    DeclareVariable("false", std::make_shared<BooleanValue>(false), false);
    DeclareVariable("null", std::make_shared<NullValue>(), false);
    DeclareVariable("print",
                    std::make_shared<NativeFunctionValue>(
                        "print",
                        [](std::vector<std::shared_ptr<RuntimeValue>> args,
                           std::unique_ptr<Environment> &env)
                            -> std::shared_ptr<RuntimeValue> {
                          int argCount = args.size();

                          for (int i = 0; i < argCount; i++) {
                            args[i]->out();
                          }

                          return std::make_shared<NullValue>();
                        }),
                    false);

    DeclareVariable("println",
                    std::make_shared<NativeFunctionValue>(
                        "println",
                        [](std::vector<std::shared_ptr<RuntimeValue>> args,
                           std::unique_ptr<Environment> &env)
                            -> std::shared_ptr<RuntimeValue> {
                          int argCount = args.size();

                          for (int i = 0; i < argCount; i++) {
                            args[i]->out();
                          }
                          std::cout << std::endl;

                          return std::make_shared<NullValue>();
                        }),
                    false);
  }

  Environment(std::unique_ptr<Environment> parent)
      : parent(std::move(parent)) {}

  std::unique_ptr<Environment> &GetParent() { return parent; }

  std::shared_ptr<RuntimeValue>
  DeclareVariable(std::string, std::shared_ptr<RuntimeValue>, bool);

  std::shared_ptr<RuntimeValue> AssignVariable(std::string,
                                               std::shared_ptr<RuntimeValue>);

  std::shared_ptr<RuntimeValue> LookUpVar(std::string);
};

#endif // !ENVIRONMENT_H
