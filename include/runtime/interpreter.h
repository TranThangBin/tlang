#ifndef INTERPRETER_H
#define INTERPRETER_H
#define INTERPRETER_VERSION "v0.0.9"

#include "parser/ast.h"
#include "parser/parser.h"
#include "runtime-value.h"
#include "runtime/environment.h"
#include <memory>

struct Interpreter {
private:
  std::unique_ptr<Parser> parser;
  std::unique_ptr<Environment> environment;

  std::shared_ptr<RuntimeValue> evaluateStmt(std::unique_ptr<Stmt> &,
                                             std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue> evaluateExpr(std::unique_ptr<Expr> &,
                                             std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue> evaluateProgram(std::unique_ptr<ProgramNode> &,
                                                std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateVariableDeclaration(std::unique_ptr<VariableDeclarationNode> &,
                              std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateBlockStmt(std::unique_ptr<BlockStmtNode> &,
                    std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateAssignmentExpr(std::unique_ptr<AssignmentExprNode> &,
                         std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateBinaryAssignmentExpr(std::unique_ptr<BinaryAssignmentExprNode> &,
                               std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateIdentifier(std::unique_ptr<IdentifierNode> &,
                     std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateBinaryExpr(std::unique_ptr<BinaryExprNode> &,
                     std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
      evaluateBinaryOperation(std::shared_ptr<RuntimeValue>,
                              std::shared_ptr<RuntimeValue>, BinaryOperator);

  std::shared_ptr<RuntimeValue>
  evaluateUnaryExpr(std::unique_ptr<UnaryExprNode> &,
                    std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
      evaluateUnaryOperation(std::shared_ptr<RuntimeValue>, UnaryOperator);

  std::shared_ptr<RuntimeValue>
  evaluateObjectLiteral(std::unique_ptr<ObjectLiteralNode> &,
                        std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateArrayExpr(std::unique_ptr<ArrayExprNode> &,
                    std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateIndexingExpr(std::unique_ptr<IndexingExpressionNode> &,
                       std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateCallExpr(std::unique_ptr<CallExpr> &, std::unique_ptr<Environment> &);

  std::shared_ptr<RuntimeValue>
  evaluateFunctionDeclaration(std::unique_ptr<FunctionDeclarationNode> &,
                              std::unique_ptr<Environment> &env);

public:
  Interpreter(std::unique_ptr<Parser> parser,
              std::unique_ptr<Environment> environment)
      : parser(std::move(parser)), environment(std::move(environment)) {}

  std::shared_ptr<RuntimeValue> Evaluate();
};

#endif // !INTERPRETER_H
