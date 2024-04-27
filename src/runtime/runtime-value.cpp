#include "runtime/runtime-value.h"
#include <string>

std::string DataTypeToString(DataType dt) {
  switch (dt) {
  case DataType::String:
    return "string";

  case DataType::Object:
    return "object";

  case DataType::Null:
    return "null";

  case DataType::Number:
    return "number";

  case DataType::Boolean:
    return "boolean";
  }

  return "unknown";
}
