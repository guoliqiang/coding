set(SRC ac_unittest.cc)
link_directories(../lib)
link_directories(../../../base/lib)
add_executable(ac_unittest ${SRC})
target_link_libraries(ac_unittest ac_automaton base)
SET(CMAKE_EXE_LINKER_FLAGS "-lpthread -lrt")
set_target_properties(ac_unittest
    PROPERTIES
    COMPILE_FLAGS    "-fPIC -m64 -g -O3 -DOS_LINUX -pthread"
)
