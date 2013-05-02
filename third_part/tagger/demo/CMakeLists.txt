set(SRC demo.cc)
link_directories(../lib)
add_executable(demo ${SRC})
target_link_libraries(demo tagger.so)
SET(CMAKE_EXE_LINKER_FLAGS "-lpthread -lrt")
set_target_properties(demo
    PROPERTIES
    COMPILE_FLAGS    "-fPIC -m64 -g -O3 -DOS_LINUX -pthread"
)
