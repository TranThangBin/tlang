#include "parser/abstraction.h"
#include "parser/ast.h"
#include "runtime/environment.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include "runtime/value.h"
#include <memory>
#include <stdexcept>
#include <vector>

std::shared_ptr<RuntimeValue>
Interpreter::evaluateProgram(std::shared_ptr<ProgramNode> program,
                             std::shared_ptr<Environment> environment) {
  std::shared_ptr<RuntimeValue> lastEvaluated = std::make_shared<NullValue>();

  std::vector<std::shared_ptr<Stmt>> stmts = program->GetStmts();
  int stmtCount = stmts.size();

  for (int i = 0; i < stmtCount; i++) {
    lastEvaluated = evaluate(stmts[i], environment);
  }

  return lastEvaluated;
}

std::shared_ptr<RuntimeValue> Interpreter::evaluateVariableDeclaration(
    std::shared_ptr<VariableDeclarationNode> variableDeclarationNode,
    std::shared_ptr<Environment> environment) {

  if (variableDeclarationNode->GetValue() == nullptr) {
    if (!variableDeclarationNode->GetMut()) {
      throw std::runtime_error("Immutables must be assigned when initialized");
    }
    return environment->DeclareVariable(
        variableDeclarationNode->GetIdentifier(), std::make_shared<NullValue>(),
        variableDeclarationNode->GetMut());
  }

  return environment->DeclareVariable(
      variableDeclarationNode->GetIdentifier(),
      evaluate(variableDeclarationNode->GetValue(), environment),
      variableDeclarationNode->GetMut());
}
