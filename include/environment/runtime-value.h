#ifndef RUNTIME_VALUE_H
#define RUNTIME_VALUE_H

enum class DataType {
  Null,
  Number,
  Boolean,
};

struct RuntimeValue {
public:
  virtual DataType DataTypeID() = 0;
};

#endif // !RUNTIME_VALUE_H
