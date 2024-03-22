cmake_minimum_required(VERSION 3.10)

project(my_interpreter)

set(cmake_cxx_standard 14)

set(cmake_export_compile_commands on)

include_directories(include)

add_executable(main
    ./src/cmd/main.cpp
    ./src/lexer/tokenize.cpp
)
