#include "parser/ast.h"
#include "runtime/environment.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <utility>
#include <vector>

std::shared_ptr<RuntimeValue>
Interpreter::evaluateProgram(std::unique_ptr<ProgramNode> &program,
                             std::unique_ptr<Environment> &env) {
  std::shared_ptr<RuntimeValue> lastEvaluated = std::make_shared<NullValue>();

  std::vector<std::unique_ptr<Stmt>> stmts = std::move(program->GetStmts());

  int stmtCount = stmts.size();

  for (int i = 0; i < stmtCount; i++) {
    lastEvaluated = evaluateStmt(stmts[i], env);
  }

  return lastEvaluated;
}

std::shared_ptr<RuntimeValue> Interpreter::evaluateVariableDeclaration(
    std::unique_ptr<VariableDeclarationNode> &varDec,
    std::unique_ptr<Environment> &env) {

  if (varDec->GetValue() == nullptr) {
    return env->DeclareVariable(varDec->GetIdentifier(),
                                std::make_shared<NullValue>(),
                                varDec->GetMut());
  }

  return env->DeclareVariable(varDec->GetIdentifier(),
                              evaluateExpr(varDec->GetValue(), env),
                              varDec->GetMut());
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateBlockStmt(std::unique_ptr<BlockStmtNode> &block,
                               std::unique_ptr<Environment> &env) {
  auto blockEnvironment = std::make_unique<Environment>(std::move(env));

  std::shared_ptr<RuntimeValue> lastEvaluated = std::make_shared<NullValue>();

  std::vector<std::unique_ptr<Stmt>> &stmts = block->GetBody();
  int stmtCount = stmts.size();

  for (int i = 0; i < stmtCount; i++) {
    lastEvaluated = evaluateStmt(stmts[i], blockEnvironment);
  }

  env = std::move(blockEnvironment->GetParent());

  return lastEvaluated;
}

std::shared_ptr<RuntimeValue> Interpreter::evaluateFunctionDeclaration(
    std::unique_ptr<FunctionDeclarationNode> &funcDec,
    std::unique_ptr<Environment> &env) {

  return env->DeclareVariable(
      funcDec->GetName(),
      std::make_shared<FunctionValue>(funcDec->GetName(), funcDec->GetParams(),
                                      std::move(funcDec->GetBody()), env),
      false);
}
