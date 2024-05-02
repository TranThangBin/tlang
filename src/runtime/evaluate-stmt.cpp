#include "parser/ast.h"
#include "runtime/environment.h"
#include "runtime/interpreter.h"
#include "runtime/runtime-value.h"
#include <memory>
#include <stdexcept>
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

  std::shared_ptr<RuntimeValue> lastEvaluated = std::make_shared<NullValue>();

  std::vector<std::unique_ptr<Stmt>> &stmts = block->GetBody();
  int stmtCount = stmts.size();

  for (int i = 0; i < stmtCount; i++) {

    lastEvaluated = evaluateStmt(stmts[i], env);

    if (lastEvaluated->DataTypeID() == DataType::Return) {
      return lastEvaluated;
    }
  }

  if (env->HasContext(EnvironmentContext::Function)) {
    return std::make_shared<ReturnValue>(std::make_shared<NullValue>());
  }

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

std::shared_ptr<RuntimeValue>
Interpreter::evaluateIfStmt(std::unique_ptr<IfStmtNode> &ifStmt,
                            std::unique_ptr<Environment> &env) {

  std::shared_ptr<RuntimeValue> condition =
      evaluateExpr(ifStmt->GetCondition(), env);

  if (condition->DataTypeID() != DataType::Boolean) {
    throw std::runtime_error("expected " + DataTypeToString(DataType::Boolean) +
                             " for condition but get " +
                             DataTypeToString(condition->DataTypeID()));
  }

  std::unique_ptr<Environment> ifScope = std::make_unique<Environment>(
      std::move(env), EnvironmentContext::Conditional);

  std::shared_ptr<RuntimeValue> result = std::make_shared<NullValue>();

  if (std::static_pointer_cast<BooleanValue>(condition)->GetValue()) {
    result = evaluateStmt(ifStmt->GetIfBody(), ifScope);
  } else if (ifStmt->GetElseBody() != nullptr) {
    result = evaluateStmt(ifStmt->GetElseBody(), env);
  }

  env = std::move(ifScope->GetParent());

  return result;
}
