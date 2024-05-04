#include "datastructure/array-list.h"
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

  ArrayList<std::unique_ptr<Stmt>> stmts = std::move(program->GetStmts());

  int stmtCount = stmts.Count();

  for (int i = 0; i < stmtCount; i++) {
    lastEvaluated = evaluateStmt(stmts.At(i), env);
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

  ArrayList<std::unique_ptr<Stmt>> &stmts = block->GetBody();
  int stmtCount = stmts.Count();

  for (int i = 0; i < stmtCount; i++) {

    lastEvaluated = evaluateStmt(stmts.At(i), env);

    if (lastEvaluated->DataTypeID() == DataType::Return) {
      return lastEvaluated;
    }
  }

  if (env->GetContext() == EnvironmentContext::Function) {
    return std::make_shared<ReturnValue>(std::make_shared<NullValue>());
  }

  return lastEvaluated;
}

std::shared_ptr<RuntimeValue> Interpreter::evaluateFunctionDeclaration(
    std::unique_ptr<FunctionDeclarationNode> &funcDec,
    std::unique_ptr<Environment> &env) {

  return env->DeclareVariable(
      funcDec->GetName(),
      std::make_shared<FunctionValue>(funcDec->GetName(),
                                      std::move(funcDec->GetParams()),
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
    result = evaluateStmt(ifStmt->GetElseBody(), ifScope);
  }

  env = std::move(ifScope->GetParent());

  return result;
}

std::shared_ptr<RuntimeValue>
Interpreter::evaluateForLoop(std::unique_ptr<ForLoopNode> &forLoop,
                             std::unique_ptr<Environment> &env) {

  std::unique_ptr<Stmt> &initializer = forLoop->GetInitializer();
  std::unique_ptr<Expr> &condition = forLoop->GetCondition();
  std::unique_ptr<Expr> &modifier = forLoop->GetModifier();
  bool haveModifier = false;

  std::unique_ptr<Environment> loopScope =
      std::make_unique<Environment>(std::move(env), EnvironmentContext::Loop);

  if (initializer != nullptr) {

    if (initializer->Kind() != NodeType::VariableDeclaration &&
        initializer->Kind() != NodeType::AssignmentExpr) {

      throw std::runtime_error(
          "expected " + NodeTypeToString(NodeType::VariableDeclaration) +
          " or " + NodeTypeToString(NodeType::AssignmentExpr) + " but get " +
          NodeTypeToString(initializer->Kind()));
    }

    evaluateStmt(initializer, loopScope);
  }

  std::shared_ptr<BooleanValue> conditionValue = nullptr;

  if (condition != nullptr) {

    std::shared_ptr<RuntimeValue> value = evaluateExpr(condition, loopScope);

    if (value->DataTypeID() != DataType::Boolean) {
      throw std::runtime_error(
          "expected " + DataTypeToString(DataType::Boolean) + " but get " +
          DataTypeToString(value->DataTypeID()));
    }

    conditionValue = std::static_pointer_cast<BooleanValue>(value);
  } else {

    conditionValue = std::make_shared<BooleanValue>(true);
  }

  if (modifier != nullptr) {

    if (modifier->Kind() != NodeType::AssignmentExpr &&
        modifier->Kind() != NodeType::BinaryAssignmentExpr) {
      throw std::runtime_error(
          "expected " + NodeTypeToString(NodeType::AssignmentExpr) + " or " +
          NodeTypeToString(NodeType::BinaryAssignmentExpr) + " but get " +
          NodeTypeToString(modifier->Kind()));
    }

    haveModifier = true;
  }

  std::shared_ptr<RuntimeValue> lastEvaluated = std::make_shared<NullValue>();

  while (conditionValue->GetValue()) {

    lastEvaluated = evaluateStmt(forLoop->GetBody(), loopScope);

    if (lastEvaluated->DataTypeID() == DataType::Return) {
      break;
    }

    if (haveModifier) {
      evaluateExpr(modifier, loopScope);
    }

    if (condition != nullptr) {

      conditionValue = std::static_pointer_cast<BooleanValue>(
          evaluateExpr(condition, loopScope));
    }
  }

  env = std::move(loopScope->GetParent());

  return lastEvaluated;
}
