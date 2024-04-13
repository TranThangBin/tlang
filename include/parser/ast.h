#ifndef AST_H
#define AST_H

#include "abstraction.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

class ProgramNode : public Stmt {
private:
  std::vector<std::unique_ptr<Stmt>> stmts;

public:
  NodeType Kind() override { return NodeType::Program; }
  std::string Yaml(int) override;

  ProgramNode(std::vector<std::unique_ptr<Stmt>> stmts)
      : stmts(std::move(stmts)) {}
};

class VariableDeclarationNode : public Stmt {
private:
  bool mut;
  std::string identifier;
  std::unique_ptr<Expr> value;

public:
  NodeType Kind() override { return NodeType::VarDec; }
  std::string Yaml(int) override;

  VariableDeclarationNode(bool mut, std::string identifier,
                          std::unique_ptr<Expr> value)
      : mut(mut), identifier(identifier), value(std::move(value)) {}
};

class BinaryExprNode : public Expr {
private:
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
  std::string op;

public:
  NodeType Kind() override { return NodeType::BinExpr; }
  std::string Yaml(int) override;

  BinaryExprNode(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right,
                 std::string op)
      : left(std::move(left)), right(std::move(right)), op(op) {}
};

class NumericLiteralNode : public Expr {
private:
  float value;

public:
  NodeType Kind() override { return NodeType::NumericLiteral; }
  std::string Yaml(int) override;

  NumericLiteralNode(float value) : value(value) {}
};

#endif // !AST_H
