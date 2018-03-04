cmake_minimum_required(VERSION 3.9)
project(Compiler8080)

set(CMAKE_CXX_STANDARD 11)

add_executable(Compiler8080 main.cpp Lexer/src/Scanner.cpp Lexer/include/Scanner.h Lexer/include/token_types.h resources/input_file.c Lexer/include/Preprocessor.h Lexer/src/Preprocessor.cpp)