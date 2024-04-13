#include "environment/environment.h"
#include "environment/runtime-value.h"
#include <memory>
#include <stdexcept>
#include <string>

Environment *Environment::Resolve(std::string varname) {
  auto it = variables.find(varname);

  if (it != variables.end()) {
    return this;
  }

  if (parent == nullptr) {
    throw std::runtime_error(varname + " doesn't exist in this scope");
  }

  return parent->Resolve(varname);
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
  Environment *env = Resolve(varname);

  if (env->mutables.find(varname) == mutables.end()) {
    throw std::runtime_error(varname + " is immutable");
  }

  env->variables[varname] = value;
  return value;
}

std::shared_ptr<RuntimeValue> Environment::LookUpVar(std::string varname) {
  Environment *env = Resolve(varname);

  auto it = env->variables.find(varname);

  return it->second;
}
