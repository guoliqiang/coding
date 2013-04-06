project(base)
set(LIB_SRC mutex.cc string_util.cc flags.cc)
add_library(libbase ${LIB_SRC})
set_target_properties(libbase
            PROPERTIES
            COMPILE_FLAGS  -fPIC
            )
set_target_properties(libbase PROPERTIES OUTPUT_NAME "base")
