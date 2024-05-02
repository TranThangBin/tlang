#include "runtime/environment.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

Environment *Environment::resolve(std::string varname) {
  auto it = variables.find(varname);

  if (it != variables.end()) {
    return this;
  }

  if (parent == nullptr) {
    throw std::runtime_error(varname + " doesn't exist in this scope");
  }

  return parent->resolve(varname);
}

std::shared_ptr<RuntimeValue>
Environment::DeclareVariable(std::string varname,
                             std::shared_ptr<RuntimeValue> value, bool mut) {
  auto it = variables.find(varname);

  if (it != variables.end()) {
    throw std::runtime_error(varname +
                             " has already been initialized in this scope");
  }

  variables.insert({varname, value});

  if (mut) {
    mutables.insert(varname);
  }

  return value;
}

std::shared_ptr<RuntimeValue>
Environment::AssignVariable(std::string varname,
                            std::shared_ptr<RuntimeValue> value) {
  Environment *env = resolve(varname);

  if (env->mutables.find(varname) == mutables.end()) {
    throw std::runtime_error(varname + " is immutable");
  }

  env->variables.find(varname)->second = value;
  return value;
}

std::shared_ptr<RuntimeValue> Environment::LookUpVar(std::string varname) {
  Environment *env = resolve(varname);

  return env->variables.find(varname)->second;
}

bool Environment::HasContext(EnvironmentContext ctx) {
  if (context == ctx) {
    return true;
  }

  if (parent == nullptr) {
    return false;
  }

  return parent->HasContext(ctx);
}
