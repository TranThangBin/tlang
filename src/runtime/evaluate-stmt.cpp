#include "runtime/environment.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

std::shared_ptr<RuntimeValue>
Interpreter::evaluateProgram(std::unique_ptr<ProgramNode> program,
                             std::unique_ptr<Environment> &environment) {
  std::shared_ptr<RuntimeValue> lastEvaluated = std::make_shared<NullValue>();

  std::vector<std::unique_ptr<Stmt>> stmts = std::move(program->GetStmts());
  int stmtCount = stmts.size();

  for (int i = 0; i < stmtCount; i++) {
    lastEvaluated = evaluate(std::move(stmts[i]), environment);
  }

  return lastEvaluated;
}

std::shared_ptr<RuntimeValue> Interpreter::evaluateVariableDeclaration(
    std::unique_ptr<VariableDeclarationNode> variableDeclarationNode,
    std::unique_ptr<Environment> &environment) {

  if (variableDeclarationNode->GetValue() == nullptr) {
    if (!variableDeclarationNode->GetMut()) {
      throw std::runtime_error("Immutables must be assigned when initialized");
    }
    return environment->DeclareVariable(
        variableDeclarationNode->GetIdentifier(), std::make_unique<NullValue>(),
        variableDeclarationNode->GetMut());
  }

  return environment->DeclareVariable(
      variableDeclarationNode->GetIdentifier(),
      evaluate(std::move(variableDeclarationNode->GetValue()), environment),
      variableDeclarationNode->GetMut());
}
