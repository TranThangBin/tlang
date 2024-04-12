#ifndef RUNTIME_VALUE_H
#define RUNTIME_VALUE_H

enum DataType {
  Dt_Null,
  Dt_Number,
  Dt_Boolean,
};

struct RuntimeValue {
public:
  virtual DataType DataTypeID() = 0;
};

#endif // !RUNTIME_VALUE_H
