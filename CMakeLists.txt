cmake_minimum_required(VERSION 3.28)
project(compiler)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

# Define a library with common code
add_library(common_lib
        Token/Token.cpp
        Lexical/Lexical.cpp
        SymbolTable/SymbolTable.cpp
)

target_include_directories(common_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Token
        ${CMAKE_CURRENT_SOURCE_DIR}/Lexical
        ${CMAKE_CURRENT_SOURCE_DIR}/SymbolTable
)

# Main application executable
add_executable(compiler
        main.cpp
        # Note: Do not include test files here
)

# Test executable for SymbolTable
add_executable(test_symbol_table
        Tests/SymbolTableTest/SymbolTableTest.cpp
)

# Test executable for Lexical
add_executable(lexical_test
        Tests/LexicalTest/LexicalTest.cpp
)

# Link the common library to the executables
target_link_libraries(compiler common_lib)
target_link_libraries(test_symbol_table common_lib)
target_link_libraries(lexical_test common_lib)

# Copy input.txt (or any other file) to the build directory after build
add_custom_command(
        TARGET compiler POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/code/code.txt      # Path to the source file
        $<TARGET_FILE_DIR:compiler>             # Destination: the directory of the executable
)
add_custom_command(
        TARGET compiler POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/code/output.txt      # Path to the source file
        $<TARGET_FILE_DIR:compiler>             # Destination: the directory of the executable
)


