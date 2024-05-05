#ifndef VALUE_H
#define VALUE_H

#include "datastructure/array-list.h"
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

struct Environment;

enum class DataType {
  Null,
  Number,
  Boolean,
  String,
  Array,
  Object,
  Function,
  NativeFunction,
  Return,
};

std::string DataTypeToString(DataType);

class RuntimeValue {
public:
  virtual DataType DataTypeID() = 0;
  virtual std::string str() = 0;
  virtual void out() = 0;
};

class IndexAbleValue : public RuntimeValue {
public:
  virtual std::shared_ptr<RuntimeValue>
      GetIndexedValue(std::shared_ptr<RuntimeValue>) = 0;
  virtual std::shared_ptr<RuntimeValue>
      SetIndexedValue(std::shared_ptr<RuntimeValue>,
                      std::shared_ptr<RuntimeValue>) = 0;
};

class NullValue : public RuntimeValue {
public:
  DataType DataTypeID() override { return DataType::Null; }
  std::string str() override { return "null"; }
  void out() override { std::cout << str(); }
};

class BooleanValue : public RuntimeValue {
private:
  bool value;

public:
  DataType DataTypeID() override { return DataType::Boolean; }

  BooleanValue(bool value) : value(value) {}

  bool GetValue() { return value; }
  std::string str() override { return value ? "true" : "false"; }
  void out() override { std::cout << str(); }
};

class NumberValue : public RuntimeValue {
private:
  double value;

public:
  DataType DataTypeID() override { return DataType::Number; }

  NumberValue(double value) : value(value) {}

  double GetValue() { return value; }
  std::string str() override { return std::to_string(value); }
  void out() override { std::cout << value; }
};

class StringValue : public RuntimeValue {
private:
  std::string value;

public:
  DataType DataTypeID() override { return DataType::String; }

  StringValue(std::string value) : value(value) {}

  std::string GetValue() { return value; }
  std::string str() override { return "\"" + value + "\""; }
  void out() override { std::cout << value; }
};

class ArrayValue : public IndexAbleValue {
private:
  ArrayList<std::shared_ptr<RuntimeValue>> values;

public:
  DataType DataTypeID() override { return DataType::Array; }

  ArrayValue(ArrayList<std::shared_ptr<RuntimeValue>> values)
      : values(std::move(values)) {}

  std::string str() override {
    return "Array[" + std::to_string(values.Count()) + "]";
  }

  void out() override {
    std::cout << "[ ";

    int valueCount = values.Count();

    if (valueCount > 0) {
      int i = 0;

      values.At(i)->out();

      for (i++; i < valueCount; i++) {
        std::cout << ", ";
        values.At(i)->out();
      }
    }

    std::cout << " ]";
  }

  std::shared_ptr<RuntimeValue>
  GetIndexedValue(std::shared_ptr<RuntimeValue> index) override {

    if (index->DataTypeID() != DataType::Number) {
      throw std::runtime_error(DataTypeToString(index->DataTypeID()) +
                               " is not a valid index for array");
    }

    std::shared_ptr<NumberValue> indexValue =
        std::static_pointer_cast<NumberValue>(index);

    int indexNumber = indexValue->GetValue();

    return values.At(indexNumber);
  }

  std::shared_ptr<RuntimeValue>
  SetIndexedValue(std::shared_ptr<RuntimeValue> index,
                  std::shared_ptr<RuntimeValue> value) override {

    if (index->DataTypeID() != DataType::Number) {
      throw std::runtime_error(DataTypeToString(index->DataTypeID()) +
                               " is not a valid index for array");
    }

    return values.At(std::static_pointer_cast<NumberValue>(index)->GetValue()) =
               value;
  }
};

class ObjectValue : public IndexAbleValue {
private:
  std::map<std::string, std::shared_ptr<RuntimeValue>> properties;

public:
  DataType DataTypeID() override { return DataType::Object; }

  ObjectValue(std::map<std::string, std::shared_ptr<RuntimeValue>> properties)
      : properties(properties) {}

  std::string str() override { return "Object"; }

  void out() override {
    std::cout << "{ ";

    auto it = properties.begin();

    if (it != properties.end()) {

      std::cout << '"' << it->first << "\": ";
      it->second->out();

      for (it++; it != properties.end(); it++) {
        std::cout << ", " << '"' << it->first << "\": ";
        it->second->out();
      }
    }

    std::cout << " }";
  }

  std::shared_ptr<RuntimeValue>
  GetIndexedValue(std::shared_ptr<RuntimeValue> key) override {
    if (key->DataTypeID() != DataType::String) {
      throw std::runtime_error(DataTypeToString(key->DataTypeID()) +
                               " is not a valid index for object");
    }

    std::string keyStr = std::static_pointer_cast<StringValue>(key)->GetValue();

    auto it = properties.find(keyStr);

    if (it == properties.end()) {
      throw std::runtime_error("Unable to find key " + keyStr +
                               " in the object");
    }

    return it->second;
  }

  std::shared_ptr<RuntimeValue>
  SetIndexedValue(std::shared_ptr<RuntimeValue> key,
                  std::shared_ptr<RuntimeValue> value) override {
    if (key->DataTypeID() != DataType::String) {
      throw std::runtime_error(DataTypeToString(key->DataTypeID()) +
                               " is not a valid index for object");
    }

    return properties[std::static_pointer_cast<StringValue>(key)->GetValue()] =
               value;
  }
};

class NativeFunctionValue : public RuntimeValue {
private:
  std::string name;
  std::shared_ptr<RuntimeValue> (*call)(
      ArrayList<std::shared_ptr<RuntimeValue>>, std::unique_ptr<Environment> &);

public:
  DataType DataTypeID() override { return DataType::NativeFunction; }

  std::shared_ptr<RuntimeValue>
  Call(ArrayList<std::shared_ptr<RuntimeValue>> args,
       std::unique_ptr<Environment> &env) {
    return call(std::move(args), env);
  }

  NativeFunctionValue(std::string name,
                      std::shared_ptr<RuntimeValue> (*call)(
                          ArrayList<std::shared_ptr<RuntimeValue>>,
                          std::unique_ptr<Environment> &))
      : name(name), call(call) {}

  std::string str() override { return "[NativeFunctionValue: " + name + "]"; }
  void out() override { std::cout << str(); }
};

class ReturnValue : public RuntimeValue {
private:
  std::shared_ptr<RuntimeValue> value;

public:
  DataType DataTypeID() override { return DataType::Return; }

  std::shared_ptr<RuntimeValue> GetValue() { return value; }

  ReturnValue(std::shared_ptr<RuntimeValue> value) : value(value) {}

  std::string str() override { return "[ReturnValue: " + value->str() + "]"; }
  void out() override { std::cout << str(); }
};

#endif // !VALUE_H
