cmake_minimum_required(VERSION 3.10)

project(my_lang)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_library(lexer ./src/lexer/tokenize.cpp ./src/lexer/check-reserve.cpp)

add_library(
  parser ./src/parser/produce-ast.cpp ./src/parser/utils.cpp
         ./src/parser/stmt.cpp ./src/parser/expr.cpp ./src/parser/yaml.cpp)

add_library(
  runtime
  ./src/runtime/environment.cpp ./src/runtime/interpreter.cpp
  ./src/runtime/evaluate-expr.cpp ./src/runtime/evaluate-stmt.cpp
  ./src/runtime/evaluate-operation.cpp)

add_executable(lexer-repl ./src/cmd/lexer-repl.cpp)
target_link_libraries(lexer-repl lexer)

add_executable(parser-repl ./src/cmd/parser-repl.cpp)
target_link_libraries(parser-repl parser lexer)

add_executable(interpreter-repl ./src/cmd/interpreter-repl.cpp)
target_link_libraries(interpreter-repl runtime parser lexer)
