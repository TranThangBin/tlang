#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser/ast.h"
#include "parser/parser.h"
#include "runtime-value.h"
#include "runtime/environment.h"
#include <memory>
#include <string>
#include <utility>

struct Interpreter {
private:
  std::unique_ptr<Parser> parser;
  std::unique_ptr<Environment> environment;

  std::shared_ptr<RuntimeValue> evaluate(std::unique_ptr<Stmt>,
                                         std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue> evaluateProgram(std::unique_ptr<ProgramNode>,
                                                std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateVariableDeclaration(std::unique_ptr<VariableDeclarationNode>,
                              std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateAssignmentExpr(std::unique_ptr<AssignmentExprNode>,
                         std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateIdentifier(std::unique_ptr<IdentifierNode>,
                     std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateBinaryExpr(std::unique_ptr<BinaryExprNode>,
                     std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
      evaluateNumberOperation(std::shared_ptr<NumberValue>,
                              std::shared_ptr<NumberValue>, std::string);

public:
  Interpreter(std::unique_ptr<Parser> parser,
              std::unique_ptr<Environment> environment)
      : parser(std::move(parser)), environment(std::move(environment)) {}

  std::shared_ptr<RuntimeValue> Evaluate();
};

#endif // !INTERPRETER_H
