project(base)
set(LIB_SRC mutex.cc)
add_library(libbase ${LIB_SRC})
set_target_properties(libbase PROPERTIES OUTPUT_NAME "base")
