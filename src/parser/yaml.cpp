#include "parser/ast.h"
#include <ostream>
#include <sstream>
#include <string>

std::string getIndent(int indentLevel) {
  return std::string(indentLevel * 4, ' ');
}

std::string ProgramNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "ProgramNode:" << std::endl
     << getIndent(indentLevel + 1) << "Body:" << std::endl;

  int stmtCount = stmts.size();

  for (int i = 0; i < stmtCount; i++) {
    ss << stmts[i]->Yaml(indentLevel + 2) << std::endl;
  }

  return ss.str();
}

std::string VariableDeclarationNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "VariableDeclarationNode:" << std::endl
     << getIndent(indentLevel + 1) << "Identifier: " << identifier << std::endl
     << getIndent(indentLevel + 1) << "Mutable: " << mut << std::endl
     << getIndent(indentLevel + 1) << "Value: " << std::endl;

  if (value == nullptr) {
    ss << getIndent(indentLevel + 2) << "nullptr";
  } else {
    ss << value->Yaml(indentLevel + 2);
  }

  return ss.str();
}

std::string AssignmentExprNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "AssignmentExprNode:" << std::endl
     << getIndent(indentLevel + 1) << "Assignee:" << std::endl
     << assignee->Yaml(indentLevel + 2) << std::endl
     << getIndent(indentLevel + 1) << "Value:" << std::endl
     << value->Yaml(indentLevel + 2);

  return ss.str();
}

std::string BinaryAssignmentExprNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "BinaryAssignmentExprNode:" << std::endl
     << getIndent(indentLevel + 1) << "Assignee:" << std::endl
     << assignee->Yaml(indentLevel + 2) << std::endl
     << getIndent(indentLevel + 1) << "Value:" << std::endl
     << value->Yaml(indentLevel + 2) << std::endl
     << getIndent(indentLevel + 1)
     << "Operator: " << BinaryOperatorToString(op);

  return ss.str();
}

std::string IdentifierNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "IdentifierNode:" << std::endl
     << getIndent(indentLevel + 1) << "Symbol: " << symbol;

  return ss.str();
}

std::string BinaryExprNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "BinaryExprNode:" << std::endl
     << getIndent(indentLevel + 1) << "Left:" << std::endl
     << left->Yaml(indentLevel + 2) << std::endl
     << getIndent(indentLevel + 1) << "Operator: " << BinaryOperatorToString(op)
     << std::endl
     << getIndent(indentLevel + 1) << "Right:" << std::endl
     << right->Yaml(indentLevel + 2);

  return ss.str();
}

std::string NumericLiteralNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "NumericLiteralNode:" << std::endl
     << getIndent(indentLevel + 1) << "Value: " << value;

  return ss.str();
}

std::string UnaryExprNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "UnaryExprNode:" << std::endl
     << getIndent(indentLevel + 1) << "Value:" << std::endl
     << value->Yaml(indentLevel + 2) << std::endl
     << getIndent(indentLevel + 1) << "Operator: " << UnaryOperatorToString(op);

  return ss.str();
}
