#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

enum NodeType {
  // Stmt
  Program,
  VariableDeclaration,

  // Expr
  BinaryExpr,
  NumericLiteral,
};

class Stmt {
private:
  NodeType kind;

public:
  Stmt(NodeType kind) : kind(kind) {}
};

class Expr : Stmt {
public:
  Expr(NodeType kind) : Stmt(kind) {}
};

class ProgramNode : Stmt {
private:
  std::vector<Stmt> stmts;

public:
  ProgramNode(std::vector<Stmt> stmts) : Stmt(Program), stmts(stmts) {}
};

class VariableDeclarationNode : Stmt {
private:
  bool mut;
  std::string identifier;
  std::unique_ptr<Expr> value;

public:
  VariableDeclarationNode(bool mut, std::string identifier,
                          std::unique_ptr<Expr> value)
      : Stmt(VariableDeclaration), mut(mut), identifier(identifier),
        value(std::move(value)) {}
};

class BinaryExprNode : Expr {
private:
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;
  std::string op;

public:
  BinaryExprNode(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right,
                 std::string op)
      : Expr(BinaryExpr), left(std::move(left)), right(std::move(right)),
        op(op) {}
};

class NumericLiteralNode : Expr {
private:
  float value;

public:
  NumericLiteralNode(double value) : Expr(NumericLiteral), value(value) {}
};

#endif // !AST_H
