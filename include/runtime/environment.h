#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "runtime-value.h"
#include "value.h"
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>

struct Environment {
private:
  std::unique_ptr<Environment> parent;
  std::map<std::string, std::shared_ptr<RuntimeValue>> variables;
  std::set<std::string> mutables;

public:
  Environment() : parent(nullptr) {
    DeclareVariable("true", std::make_shared<BooleanValue>(true), false);
    DeclareVariable("false", std::make_shared<BooleanValue>(false), false);
    DeclareVariable("null", std::make_shared<NullValue>(), false);
  }

  Environment(std::unique_ptr<Environment> parent)
      : parent(std::move(parent)) {}

  Environment *Resolve(std::string);

  std::shared_ptr<RuntimeValue>
  DeclareVariable(std::string, std::shared_ptr<RuntimeValue>, bool);

  std::shared_ptr<RuntimeValue> AssignVariable(std::string,
                                               std::shared_ptr<RuntimeValue>);

  std::shared_ptr<RuntimeValue> LookUpVar(std::string);
};

#endif // !ENVIRONMENT_H
