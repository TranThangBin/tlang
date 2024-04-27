#ifndef AST_H
#define AST_H

#include "lexer/token.h"
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

enum class NodeType {
  Program,
  VariableDeclaration,
  BlockStmt,

  AssignmentExpr,
  BinaryAssignmentExpr,
  BinaryExpr,
  NumericLiteral,
  Identifier,
  UnaryExpr,
  ObjectLiteral,
};

enum class BinaryOperator {
  Addition,
  Subtraction,
  Multiplication,
  Division,
  Modulo,
  Invalid,
};

enum class UnaryOperator {
  Plus,
  Minus,
  Not,
  Invalid,
};

BinaryOperator AssignmentToBinaryOperator(std::string);
BinaryOperator TokenTypeToBinaryOperator(TokenType);
UnaryOperator TokenTypeToUnaryOperator(TokenType);

std::string NodeTypeToString(NodeType);
std::string BinaryOperatorToString(BinaryOperator);
std::string UnaryOperatorToString(UnaryOperator);

class Stmt {
public:
  virtual std::string Yaml(int) = 0;
  virtual NodeType Kind() = 0;
};

class Expr : public Stmt {};

class ProgramNode : public Stmt {
private:
  std::vector<std::unique_ptr<Stmt>> stmts;

public:
  std::string Yaml(int) override;

  NodeType Kind() override { return NodeType::Program; }

  std::vector<std::unique_ptr<Stmt>> &GetStmts() { return stmts; }

  ProgramNode(std::vector<std::unique_ptr<Stmt>> stmts)
      : stmts(std::move(stmts)) {}
};

class VariableDeclarationNode : public Stmt {
private:
  bool mut;
  std::string identifier;
  std::unique_ptr<Expr> value;

public:
  std::string Yaml(int) override;

  NodeType Kind() override { return NodeType::VariableDeclaration; }

  bool GetMut() { return mut; }

  std::string GetIdentifier() { return identifier; }

  std::unique_ptr<Expr> &GetValue() { return value; }

  VariableDeclarationNode(bool mut, std::string identifier,
                          std::unique_ptr<Expr> value)
      : mut(mut), identifier(identifier), value(std::move(value)) {}
};

class BlockStmtNode : public Stmt {
private:
  std::vector<std::unique_ptr<Stmt>> stmts;

public:
  std::string Yaml(int) override;

  NodeType Kind() override { return NodeType::BlockStmt; }

  std::vector<std::unique_ptr<Stmt>> &GetStmts() { return stmts; }

  BlockStmtNode(std::vector<std::unique_ptr<Stmt>> stmts)
      : stmts(std::move(stmts)) {}
};

class AssignmentExprNode : public Expr {
private:
  std::unique_ptr<Expr> assignee;
  std::unique_ptr<Expr> value;

public:
  std::string Yaml(int) override;

  NodeType Kind() override { return NodeType::AssignmentExpr; }

  std::unique_ptr<Expr> &GetAssignee() { return assignee; }

  std::unique_ptr<Expr> &GetValue() { return value; }

  AssignmentExprNode(std::unique_ptr<Expr> assignee,
                     std::unique_ptr<Expr> value)
      : assignee(std::move(assignee)), value(std::move(value)) {}
};

class BinaryAssignmentExprNode : public Expr {
private:
  std::unique_ptr<Expr> assignee;
  std::unique_ptr<Expr> value;
  BinaryOperator op;

public:
  std::string Yaml(int) override;

  NodeType Kind() override { return NodeType::BinaryAssignmentExpr; }

  std::unique_ptr<Expr> &GetAssignee() { return assignee; }

  std::unique_ptr<Expr> &GetValue() { return value; }

  BinaryOperator GetOperator() { return op; }

  BinaryAssignmentExprNode(std::unique_ptr<Expr> assignee,
                           std::unique_ptr<Expr> value, BinaryOperator op)
      : assignee(std::move(assignee)), value(std::move(value)), op(op) {}
};

class IdentifierNode : public Expr {
private:
  std::string symbol;

public:
  std::string Yaml(int) override;

  NodeType Kind() override { return NodeType::Identifier; }

  std::string GetSymbol() { return symbol; }

  IdentifierNode(std::string symbol) : symbol(symbol) {}
};

class BinaryExprNode : public Expr {
private:
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
  BinaryOperator op;

public:
  std::string Yaml(int) override;

  NodeType Kind() override { return NodeType::BinaryExpr; }

  std::unique_ptr<Expr> &GetLeft() { return left; }

  std::unique_ptr<Expr> &GetRight() { return right; }

  BinaryOperator GetOperator() { return op; }

  BinaryExprNode(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right,
                 BinaryOperator op)
      : left(std::move(left)), right(std::move(right)), op(op) {}
};

class NumericLiteralNode : public Expr {
private:
  float value;

public:
  std::string Yaml(int) override;

  NodeType Kind() override { return NodeType::NumericLiteral; }

  float GetValue() { return value; }

  NumericLiteralNode(float value) : value(value) {}
};

class UnaryExprNode : public Expr {
private:
  std::unique_ptr<Expr> value;
  UnaryOperator op;

public:
  std::string Yaml(int) override;

  NodeType Kind() override { return NodeType::UnaryExpr; }

  std::unique_ptr<Expr> &GetValue() { return value; }

  UnaryOperator GetOperator() { return op; }

  UnaryExprNode(std::unique_ptr<Expr> value, UnaryOperator op)
      : value(std::move(value)), op(op) {}
};

class ObjectLiteralNode : public Expr {
private:
  std::map<std::string, std::unique_ptr<Expr>> properties;

public:
  std::string Yaml(int) override;

  NodeType Kind() override { return NodeType::ObjectLiteral; }

  std::map<std::string, std::unique_ptr<Expr>> &GetProperties() {
    return properties;
  }

  ObjectLiteralNode(std::map<std::string, std::unique_ptr<Expr>> properties)
      : properties(std::move(properties)) {}
};

#endif // !AST_H
