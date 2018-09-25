cmake_minimum_required(VERSION 3.9)

project(Compiler8080)

add_subdirectory(tinyxml2)

include_directories(
    Lexer/include
    Parser/include
)

# SET RESOURCE FILES OBJECT
file(GLOB RESOURCE_FILES
    "${CMAKE_SOURCE_DIR}/resources/*.h"
    "${CMAKE_SOURCE_DIR}/resources/*.c"
)
file(COPY ${RESOURCE_FILES} DESTINATION ${CMAKE_BINARY_DIR} )

# SET LEXER FILES OBJECT
file(GLOB LEXER_FILES
    "${CMAKE_SOURCE_DIR}/Lexer/include/*.h"
    "${CMAKE_SOURCE_DIR}/Lexer/src/*.cpp"
)
add_library(Lexer ${LEXER_FILES})

# SET PARSER FILES OBJECT
file(GLOB PARSER_FILES
    "${CMAKE_SOURCE_DIR}/Parser/include/*.h"
    "${CMAKE_SOURCE_DIR}/Parser/src/*.cpp"
)
add_library(Parser ${PARSER_FILES})

# SET TESTING ROUTINE EXECUTABLE
add_executable(TestLexingRoutine tests/testlexingroutine.cpp)
target_link_libraries(TestLexingRoutine Parser Lexer)

add_executable(TestParsingTree tests/testparser.cpp)
target_link_libraries(TestParsingTree tinyxml2 Parser Lexer)
