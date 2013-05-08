project(trie_tree)
link_directories(../../../base/lib)
set(LIB_SRC trie_tree.cc)
add_library(libtrie_tree ${LIB_SRC})
target_link_libraries(libtrie_tree base)
set_target_properties(libtrie_tree
           PROPERTIES
           COMPILE_FLAGS    "-fPIC -m64 -g -O3 -DOS_LINUX -pthread"
           )
set_target_properties(libtrie_tree PROPERTIES OUTPUT_NAME "trie_tree")
