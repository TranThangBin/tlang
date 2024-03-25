cmake_minimum_required(VERSION 3.10)

project(my_interpreter)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_library(lexer
    ./src/lexer/tokenize.cpp
)

add_executable(main ./src/cmd/main.cpp)
target_link_libraries(main lexer)

add_executable(lexer-repl ./src/cmd/lexer-repl.cpp)
target_link_libraries(lexer-repl lexer)
