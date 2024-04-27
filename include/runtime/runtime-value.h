#ifndef VALUE_H
#define VALUE_H

#include <map>
#include <memory>
#include <sstream>
#include <string>

enum class DataType {
  Null,
  Number,
  Boolean,
  String,
  Object,
};

std::string DataTypeToString(DataType);

class RuntimeValue {
public:
  virtual DataType DataTypeID() = 0;
  virtual std::string str() = 0;
};

class NullValue : public RuntimeValue {
public:
  DataType DataTypeID() override { return DataType::Null; }
  std::string str() override { return "null"; }
};

class BooleanValue : public RuntimeValue {
private:
  bool value;

public:
  DataType DataTypeID() override { return DataType::Boolean; }

  BooleanValue(bool value) : value(value) {}

  bool GetValue() { return value; }
  std::string str() override { return value ? "true" : "false"; }
};

class NumberValue : public RuntimeValue {
private:
  float value;

public:
  DataType DataTypeID() override { return DataType::Number; }

  NumberValue(float value) : value(value) {}

  float GetValue() { return value; }
  std::string str() override { return std::to_string(value); }
};

class StringValue : public RuntimeValue {
private:
  std::string value;

public:
  DataType DataTypeID() override { return DataType::String; }

  StringValue(std::string value) : value(value) {}

  std::string GetValue() { return value; }
  std::string str() override { return "\"" + value + "\""; }
};

class ObjectValue : public RuntimeValue {
private:
  std::map<std::string, std::shared_ptr<RuntimeValue>> properties;

public:
  DataType DataTypeID() override { return DataType::Object; }

  ObjectValue(std::map<std::string, std::shared_ptr<RuntimeValue>> properties)
      : properties(properties) {}

  std::string str() override {
    std::stringstream ss;

    ss << "{";

    for (auto it = properties.begin(); it != properties.end(); it++) {
      ss << it->first << ": " << it->second->str() << ", ";
    }

    ss << "}";

    return ss.str();
  }
};

#endif // !VALUE_H
