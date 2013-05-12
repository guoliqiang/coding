project(da_trie)
link_directories(../../../base/lib)
set(LIB_SRC da_trie.cc da_trie_string16.cc)
add_library(libda_trie ${LIB_SRC})
target_link_libraries(libda_trie base)
set_target_properties(libda_trie
           PROPERTIES
           COMPILE_FLAGS    "-fPIC -m64 -g -O3 -DOS_LINUX -pthread"
           )
set_target_properties(libda_trie PROPERTIES OUTPUT_NAME "da_trie")
