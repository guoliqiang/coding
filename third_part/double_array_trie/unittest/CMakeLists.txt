set(SRC da_trie_unittest.cc)
set(SRC_1 da_trie_string16_unittest.cc)
link_directories(../lib)
link_directories(../../testing/gtest/lib)
link_directories(../../../base/lib)
add_executable(da_trie_unittest ${SRC})
add_executable(da_trie_string16_unittest ${SRC_1})
target_link_libraries(da_trie_unittest da_trie base gtest)
target_link_libraries(da_trie_string16_unittest da_trie base gtest)
SET(CMAKE_EXE_LINKER_FLAGS "-lpthread -lrt")
set_target_properties(da_trie_unittest
    PROPERTIES
    COMPILE_FLAGS    "-fPIC -m64 -g -O3 -DOS_LINUX -pthread"
)
set_target_properties(da_trie_string16_unittest
    PROPERTIES
    COMPILE_FLAGS    "-fPIC -m64 -g -O3 -DOS_LINUX -pthread"
)
