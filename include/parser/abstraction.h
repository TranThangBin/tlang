#ifndef ABSTRACTION_H
#define ABSTRACTION_H

enum NodeType {
  // Stmt
  Nt_Program,
  Nt_VarDec,

  // Expr
  Nt_BinExpr,
  Nt_NumericLiteral,
  Nt_Identifier,
};

class Stmt {
public:
  virtual NodeType Kind() = 0;
};

class Expr : public Stmt {};

#endif // !ABSTRACTION_H
