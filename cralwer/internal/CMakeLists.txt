cmake_minimum_required(VERSION 2.8)
set(src_list hello.cc)
link_directories(../lib)
add_executable(hello ${src_list})
target_link_libraries(hello mylib)
set(src_list hello1.cc)
add_executable(hello1 ${src_list})
set_target_properties(hello
    PROPERTIES
    COMPILE_FLAGS          -g
)
