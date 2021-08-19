
cmake_minimum_required(VERSION 3.15)

project(using_shared_library)

add_executable(main main.c)

target_link_directories(main
	PUBLIC libs)
	
target_link_libraries(main libfoo.so)
