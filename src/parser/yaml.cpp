#include "parser/ast.h"
#include <iostream>
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

std::string BlockStmtNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "BlockStmtNode:" << std::endl
     << getIndent(indentLevel + 1) << "Body:" << std::endl;

  int stmtCount = stmts.size();

  for (int i = 0; i < stmtCount; i++) {
    ss << stmts[i]->Yaml(indentLevel + 2) << std::endl;
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

std::string StringLiteralNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "StringLiteralNode:" << std::endl
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

std::string ObjectLiteralNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "ObjectLiteralNode:" << std::endl
     << getIndent(indentLevel + 1) << "Properties:";

  for (auto it = properties.begin(); it != properties.end(); it++) {
    ss << std::endl
       << getIndent(indentLevel + 2) << it->first << ":" << std::endl
       << it->second->Yaml(indentLevel + 3);
  }

  return ss.str();
}

std::string ArrayExprNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "ArrayExprNode:" << std::endl
     << getIndent(indentLevel + 1) << "Values:";

  int valueCount = values.size();

  for (int i = 0; i < valueCount; i++) {
    ss << std::endl
       << getIndent(indentLevel + 2) << i << ":" << std::endl
       << values[i]->Yaml(indentLevel + 3);
  }

  return ss.str();
}

std::string IndexingExpressionNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "IndexingExpressionNode" << std::endl
     << getIndent(indentLevel + 1) << "Accessor:" << std::endl
     << accessor->Yaml(indentLevel + 2) << std::endl
     << getIndent(indentLevel + 1) << "Index:" << std::endl
     << index->Yaml(indentLevel + 2);

  return ss.str();
}

std::string FunctionExprNode::Yaml(int indentLevel) {
  std::stringstream ss;

  ss << getIndent(indentLevel) << "FunctionExprNode" << std::endl
     << getIndent(indentLevel + 1) << "Params:" << std::endl;

  int paramCount = params.size();

  for (int i = 0; i < paramCount; i++) {
    ss << getIndent(indentLevel + 2) << params[i] << std::endl;
  }

  ss << getIndent(indentLevel + 1) << "Block:" << std::endl
     << block->Yaml(indentLevel + 2);

  return ss.str();
}
