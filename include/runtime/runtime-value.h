#ifndef VALUE_H
#define VALUE_H

#include <string>

enum class DataType {
  Null,
  Number,
  Boolean,
};

std::string DataTypeToString(DataType);

struct RuntimeValue {
public:
  virtual DataType DataTypeID() = 0;
  virtual std::string str() = 0;
};

struct NullValue : public RuntimeValue {
public:
  DataType DataTypeID() override { return DataType::Null; }
  std::string str() override { return "null"; }
};

struct BooleanValue : public RuntimeValue {
private:
  bool value;

public:
  DataType DataTypeID() override { return DataType::Boolean; }

  BooleanValue(bool value) : value(value) {}

  bool GetValue() { return value; }
  std::string str() override { return value ? "true" : "false"; }
};

struct NumberValue : public RuntimeValue {
private:
  float value;

public:
  DataType DataTypeID() override { return DataType::Number; }

  NumberValue(float value) : value(value) {}

  float GetValue() { return value; }
  std::string str() override { return std::to_string(value); }
};

#endif // !VALUE_H
