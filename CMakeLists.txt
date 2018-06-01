cmake_minimum_required(VERSION 3.9)
project(Compiler8080)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    Lexer/include
)

file(COPY ${CMAKE_SOURCE_DIR}/resources/hello_world.c DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/resources/hello_stuff.h DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/resources/input_file.c DESTINATION ${CMAKE_BINARY_DIR})

#add_executable(Compiler8080 main.cpp Lexer/src/Scanner.cpp Lexer/include/Scanner.h Lexer/include/token_types.h Lexer/include/Preprocessor.h Lexer/src/Preprocessor.cpp Lexer/include/utility_functions.h Lexer/src/utility_functions.cpp)
add_library(Lexer Lexer/src/Scanner.cpp Lexer/include/Scanner.h Lexer/include/token_types.h Lexer/include/Preprocessor.h Lexer/src/Preprocessor.cpp Lexer/include/utility_functions.h Lexer/src/utility_functions.cpp)

add_executable(TestLexingRoutine tests/testlexingroutine.cpp)

target_link_libraries(TestLexingRoutine Lexer)