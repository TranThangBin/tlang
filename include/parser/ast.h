#ifndef AST_H
#define AST_H

#include "abstraction.h"
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class ProgramNode : public Stmt {
private:
  std::vector<std::unique_ptr<Stmt>> stmts;

public:
  NodeType Kind() override { return NodeType::Nt_Program; }

  ProgramNode(std::vector<std::unique_ptr<Stmt>> stmts)
      : stmts(std::move(stmts)) {}

  void Display() {
    int stmtsLen = stmts.size();

    for (int i = 0; i < stmtsLen; i++) {
      std::cout << stmts[i]->Kind() << std::endl;
    }
  }
};

class VariableDeclarationNode : public Stmt {
private:
  bool mut;
  std::string identifier;
  std::unique_ptr<Expr> value;

public:
  NodeType Kind() override { return NodeType::Nt_VarDec; }

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
  NodeType Kind() override { return NodeType::Nt_BinExpr; }

  BinaryExprNode(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right,
                 std::string op)
      : left(std::move(left)), right(std::move(right)), op(op) {}
};

class NumericLiteralNode : public Expr {
private:
  float value;

public:
  NodeType Kind() override { return NodeType::Nt_NumericLiteral; }

  NumericLiteralNode(float value) : value(value) {}
};

#endif // !AST_H
