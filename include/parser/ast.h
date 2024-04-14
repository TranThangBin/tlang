#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>

enum class NodeType {
  Program,
  VariableDeclaration,

  AssignmentExpr,
  BinaryExpr,
  NumericLiteral,
  Identifier,
};

class Stmt {
public:
  virtual std::string Yaml(int) = 0;
  virtual NodeType Kind() = 0;
};

class Expr : public Stmt {};

class ProgramNode : public Stmt {
private:
  std::vector<std::shared_ptr<Stmt>> stmts;

public:
  std::string Yaml(int) override;

  NodeType Kind() override { return NodeType::Program; }
  std::vector<std::shared_ptr<Stmt>> GetStmts() { return stmts; }

  ProgramNode(std::vector<std::shared_ptr<Stmt>> stmts) : stmts(stmts) {}
};

class VariableDeclarationNode : public Stmt {
private:
  bool mut;
  std::string identifier;
  std::shared_ptr<Expr> value;

public:
  std::string Yaml(int) override;
  NodeType Kind() override { return NodeType::VariableDeclaration; }

  bool GetMut() { return mut; }
  std::string GetIdentifier() { return identifier; }
  std::shared_ptr<Expr> GetValue() { return value; }

  VariableDeclarationNode(bool mut, std::string identifier,
                          std::shared_ptr<Expr> value)
      : mut(mut), identifier(identifier), value(value) {}
};

class AssignmentExprNode : public Expr {
private:
  std::shared_ptr<Expr> assignee;
  std::shared_ptr<Expr> value;

public:
  std::string Yaml(int) override;

  NodeType Kind() override { return NodeType::AssignmentExpr; }
  std::shared_ptr<Expr> GetAssignee() { return assignee; }
  std::shared_ptr<Expr> GetValue() { return value; }

  AssignmentExprNode(std::shared_ptr<Expr> assignee,
                     std::shared_ptr<Expr> value)
      : assignee(assignee), value(value) {}
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
  std::shared_ptr<Expr> left;
  std::shared_ptr<Expr> right;
  std::string op;

public:
  std::string Yaml(int) override;

  NodeType Kind() override { return NodeType::BinaryExpr; }
  std::shared_ptr<Expr> GetLeft() { return left; }
  std::shared_ptr<Expr> GetRight() { return right; }
  std::string GetOperator() { return op; }

  BinaryExprNode(std::shared_ptr<Expr> left, std::shared_ptr<Expr> right,
                 std::string op)
      : left(left), right(right), op(op) {}
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

#endif // !AST_H
