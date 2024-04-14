#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser/abstraction.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "runtime/environment.h"
#include "runtime/runtime-value.h"
#include "runtime/value.h"
#include <memory>
#include <string>

struct Interpreter {
private:
  std::shared_ptr<Parser> parser;
  std::shared_ptr<Environment> environment;

  std::shared_ptr<RuntimeValue> evaluate(std::shared_ptr<Stmt>,
                                         std::shared_ptr<Environment>);

  std::shared_ptr<RuntimeValue> evaluateProgram(std::shared_ptr<ProgramNode>,
                                                std::shared_ptr<Environment>);
  std::shared_ptr<RuntimeValue>
      evaluateVariableDeclaration(std::shared_ptr<VariableDeclarationNode>,
                                  std::shared_ptr<Environment>);
  std::shared_ptr<RuntimeValue>
      evaluateAssignmentExpr(std::shared_ptr<AssignmentExprNode>,
                             std::shared_ptr<Environment>);
  std::shared_ptr<RuntimeValue>
      evaluateIdentifier(std::shared_ptr<IdentifierNode>,
                         std::shared_ptr<Environment>);
  std::shared_ptr<RuntimeValue>
      evaluateBinaryExpr(std::shared_ptr<BinaryExprNode>,
                         std::shared_ptr<Environment>);
  std::shared_ptr<RuntimeValue>
      evaluateNumberOperation(std::shared_ptr<NumberValue>,
                              std::shared_ptr<NumberValue>, std::string);

public:
  Interpreter(std::shared_ptr<Parser> parser,
              std::shared_ptr<Environment> environment)
      : parser(parser), environment(environment) {}
  std::shared_ptr<RuntimeValue> Evaluate();
};

#endif // !INTERPRETER_H
