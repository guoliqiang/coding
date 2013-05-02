project(tagger)
link_directories(../lib)
link_directories(../../../base/lib)
set(LIB_SRC tagger.cc)
add_library(libtagger SHARED ${LIB_SRC})
#add_library(libtagger ${LIB_SRC})
target_link_libraries(libtagger ICTCLAS50 base)
set_target_properties(libtagger
           PROPERTIES
           COMPILE_FLAGS    "-fPIC -m64 -g -O3 -DOS_LINUX -pthread"
           )
set_target_properties(libtagger PROPERTIES OUTPUT_NAME "tagger")
