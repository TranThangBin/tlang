#ifndef VALUE_H
#define VALUE_H

#include "runtime-value.h"

struct NullValue : public RuntimeValue {
public:
  DataType DataTypeID() override { return DataType::Dt_Null; }
};

struct BooleanValue : public RuntimeValue {
private:
  bool value;

public:
  DataType DataTypeID() override { return DataType::Dt_Boolean; }

  BooleanValue(bool value) : value(value) {}

  bool GetValue() { return value; }
};

struct NumberValue : public RuntimeValue {
private:
  float value;

public:
  DataType DataTypeID() override { return DataType::Dt_Number; }

  NumberValue(float value) : value(value) {}

  float GetValue() { return value; }
};

#endif // !VALUE_H
