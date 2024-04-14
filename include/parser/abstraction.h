#ifndef ABSTRACTION_H
#define ABSTRACTION_H

#include <string>
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
  virtual NodeType Kind() = 0;
  virtual std::string Yaml(int) = 0;
};

class Expr : public Stmt {};

#endif // !ABSTRACTION_H
