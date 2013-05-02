set(SRC ict_tagger_wrapper.cc)
link_directories(../lib)
add_library(ictwrapper SHARED ${SRC})
target_link_libraries(ictwrapper tagger.so)
set_target_properties(ictwrapper
    PROPERTIES
    COMPILE_FLAGS    "-m64 -O3 -g -DOS_LINUX -pthread"
 )

set(SRC_1 ict_tagger_wrapper_unit.cc)
add_executable(ict_tagger_unit ${SRC_1})
target_link_libraries(ict_tagger_unit ictwrapper.so)
SET(CMAKE_EXE_LINKER_FLAGS "-lpthread -lrt")
set_target_properties(ict_tagger_unit
    PROPERTIES
    COMPILE_FLAGS    "-m64 -O3 -g -DOS_LINUX -pthread"
 )
