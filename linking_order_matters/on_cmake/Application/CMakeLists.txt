


cmake_minimum_required(VERSION 3.15)

add_executable(app app.cpp)

target_link_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/MyPrintf)

target_link_libraries(app
    Bar
    Foo
    libmyprintf.a
)