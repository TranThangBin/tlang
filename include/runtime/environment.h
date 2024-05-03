#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "parser/ast.h"
#include "runtime-value.h"
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

enum class EnvironmentContext {
  Global,
  Block,
  Conditional,
  Function,
  Loop,
};

struct Environment {
private:
  std::unique_ptr<Environment> parent;
  std::map<std::string, std::shared_ptr<RuntimeValue>> variables;
  std::set<std::string> mutables;
  EnvironmentContext context;

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

  Environment(std::unique_ptr<Environment> parent, EnvironmentContext context)
      : parent(std::move(parent)), context(context) {}

  std::unique_ptr<Environment> &GetParent() { return parent; }
  EnvironmentContext GetContext() { return context; }

  std::shared_ptr<RuntimeValue>
  DeclareVariable(std::string, std::shared_ptr<RuntimeValue>, bool);

  bool HasContext(EnvironmentContext);

  std::shared_ptr<RuntimeValue> AssignVariable(std::string,
                                               std::shared_ptr<RuntimeValue>);

  std::shared_ptr<RuntimeValue> LookUpVar(std::string);
};

class FunctionValue : public RuntimeValue {
private:
  std::string name;
  std::vector<std::string> params;
  std::unique_ptr<BlockStmtNode> body;
  std::unique_ptr<Environment> &declaredEnv;

public:
  DataType DataTypeID() override { return DataType::Function; }

  std::vector<std::string> GetParams() { return params; }
  std::unique_ptr<BlockStmtNode> &GetBody() { return body; }
  std::unique_ptr<Environment> &GetDeclaredEnv() { return declaredEnv; }
  void SetDeclaredEnv(std::unique_ptr<Environment> env) {
    declaredEnv = std::move(env);
  }

  FunctionValue(std::string name, std::vector<std::string> params,
                std::unique_ptr<BlockStmtNode> body,
                std::unique_ptr<Environment> &declaredEnv)
      : name(name), params(params), body(std::move(body)),
        declaredEnv(declaredEnv) {}

  std::string str() override { return "[FunctionValue: " + name + "]"; }
  void out() override { std::cout << str(); }
};

#endif // !ENVIRONMENT_H
