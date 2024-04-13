#ifndef VALUE_H
#define VALUE_H

#include "runtime-value.h"

struct NullValue : public RuntimeValue {
public:
  DataType DataTypeID() override { return DataType::Null; }
};

struct BooleanValue : public RuntimeValue {
private:
  bool value;

public:
  DataType DataTypeID() override { return DataType::Boolean; }

  BooleanValue(bool value) : value(value) {}

  bool GetValue() { return value; }
};

struct NumberValue : public RuntimeValue {
private:
  float value;

public:
  DataType DataTypeID() override { return DataType::Number; }

  NumberValue(float value) : value(value) {}

  float GetValue() { return value; }
};

#endif // !VALUE_H
