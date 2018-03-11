cmake_minimum_required(VERSION 3.9)
project(Compiler8080)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    Lexer/include
)

add_executable(Compiler8080 main.cpp Lexer/src/Scanner.cpp Lexer/include/Scanner.h Lexer/include/token_types.h Lexer/include/Preprocessor.h Lexer/src/Preprocessor.cpp Lexer/include/utility_functions.h Lexer/src/utility_functions.cpp)