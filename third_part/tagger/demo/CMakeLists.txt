set(SRC demo.cc)
link_directories(../lib)
add_executable(demo ${SRC})
target_link_libraries(demo tagger.so)
set_target_properties(demo
    PROPERTIES
    COMPILE_FLAGS    "-m64 -O3 -g -DOS_LINUX -pthread"
 )
