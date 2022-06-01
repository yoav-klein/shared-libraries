
cmake_minimum_required(VERSION 3.15)

add_library(Bar STATIC bar.cpp)

target_include_directories(Bar 
PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../MyPrintf    
)
