#ifndef RUNTIME_VALUE_H
#define RUNTIME_VALUE_H

#include <string>
enum class DataType {
  Null,
  Number,
  Boolean,
};

struct RuntimeValue {
public:
  virtual DataType DataTypeID() = 0;
  virtual std::string str() = 0;
};

#endif // !RUNTIME_VALUE_H
