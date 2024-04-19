#include "parser/ast.h"
#include "runtime/environment.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <memory>
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
    return environment->DeclareVariable(
        variableDeclarationNode->GetIdentifier(), std::make_shared<NullValue>(),
        variableDeclarationNode->GetMut());
  }

  return environment->DeclareVariable(
      variableDeclarationNode->GetIdentifier(),
      evaluate(std::move(variableDeclarationNode->GetValue()), environment),
      variableDeclarationNode->GetMut());
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateBlockStmt(std::unique_ptr<BlockStmtNode> blockStmtNode,
                               std::unique_ptr<Environment> &environment) {
  std::unique_ptr<Environment> blockEnvironment =
      std::make_unique<Environment>(std::move(environment));

  std::shared_ptr<RuntimeValue> lastEvaluated = std::make_shared<NullValue>();

  std::vector<std::unique_ptr<Stmt>> stmts =
      std::move(blockStmtNode->GetStmts());
  int stmtCount = stmts.size();

  for (int i = 0; i < stmtCount; i++) {
    lastEvaluated = evaluate(std::move(stmts[i]), blockEnvironment);
  }

  environment = std::move(blockEnvironment->GetParent());

  return lastEvaluated;
}
