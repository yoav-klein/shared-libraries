
cmake_minimum_required(VERSION 3.15)

add_library(Foo STATIC foo.cpp)
target_include_directories(Foo 
PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../MyPrintf    
)