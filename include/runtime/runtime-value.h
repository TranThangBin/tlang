#ifndef VALUE_H
#define VALUE_H

#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

enum class DataType {
  Null,
  Number,
  Boolean,
  String,
  Array,
  Object,
};

std::string DataTypeToString(DataType);

class RuntimeValue {
public:
  virtual DataType DataTypeID() = 0;
  virtual std::string str() = 0;
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
  double value;

public:
  DataType DataTypeID() override { return DataType::Number; }

  NumberValue(double value) : value(value) {}

  double GetValue() { return value; }
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

class ArrayValue : public IndexAbleValue {
private:
  std::vector<std::shared_ptr<RuntimeValue>> values;

public:
  DataType DataTypeID() override { return DataType::Array; }

  ArrayValue(std::vector<std::shared_ptr<RuntimeValue>> values)
      : values(values) {}

  std::string str() override {
    std::stringstream ss;

    ss << "[ ";

    int valueCount = values.size();

    if (valueCount > 0) {
      int i = 0;

      ss << values[i]->str();

      for (i++; i < valueCount; i++) {
        ss << ", " << values[i]->str();
      }
    }

    ss << " ]";

    return ss.str();
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

    if (indexNumber < 0 || indexNumber >= values.size()) {
      throw std::runtime_error("Index " + indexValue->str() +
                               " is out of range of the array");
    }

    return values[indexNumber];
  }

  std::shared_ptr<RuntimeValue>
  SetIndexedValue(std::shared_ptr<RuntimeValue> index,
                  std::shared_ptr<RuntimeValue> value) override {

    if (index->DataTypeID() != DataType::Number) {
      throw std::runtime_error(DataTypeToString(index->DataTypeID()) +
                               " is not a valid index for array");
    }

    return values[std::static_pointer_cast<NumberValue>(index)->GetValue()] =
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

  std::string str() override {
    std::stringstream ss;

    ss << "{ ";

    auto it = properties.begin();

    if (it != properties.end()) {

      ss << '"' << it->first << "\": " << it->second->str();

      for (it++; it != properties.end(); it++) {
        ss << ", " << '"' << it->first << "\": " << it->second->str();
      }
    }

    ss << " }";

    return ss.str();
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

#endif // !VALUE_H
