cmake_minimum_required(VERSION 3.10)

project(my_lang)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_library(lexer ./src/lexer/tokenize.cpp ./src/lexer/lexer.cpp
                  ./src/lexer/token.cpp)

add_library(
  parser
  ./src/parser/produce-ast.cpp ./src/parser/utils.cpp
  ./src/parser/parse-stmt.cpp ./src/parser/parse-expr.cpp ./src/parser/yaml.cpp
  ./src/parser/ast.cpp)

add_library(
  runtime
  ./src/runtime/runtime-value.cpp
  ./src/runtime/environment.cpp
  ./src/runtime/interpreter.cpp
  ./src/runtime/evaluate-expr.cpp
  ./src/runtime/evaluate-stmt.cpp
  ./src/runtime/evaluate-binary-operation.cpp
  ./src/runtime/evaluate-unary-operation.cpp)

add_executable(lexer-repl ./src/cmd/lexer-repl.cpp)
target_link_libraries(lexer-repl lexer)

add_executable(parser-repl ./src/cmd/parser-repl.cpp)
target_link_libraries(parser-repl parser lexer)

add_executable(interpreter-repl ./src/cmd/interpreter-repl.cpp)
target_link_libraries(interpreter-repl runtime parser lexer)

add_executable(tlang ./src/cmd/main.cpp)
target_link_libraries(tlang runtime parser lexer)

add_executable(test ./src/cmd/test.cpp)
target_link_libraries(test runtime parser lexer)
