set(SRC CppClient.cc
        ../proto/gen-cpp/Calculator.cpp
        ../proto/gen-cpp/shared_constants.cpp
        ../proto/gen-cpp/SharedService.cpp
        ../proto/gen-cpp/shared_types.cpp
        ../proto/gen-cpp/tutorial_constants.cpp
        ../proto/gen-cpp/tutorial_types.cpp
        )
link_directories(../../lib)
link_directories(../../../libevent/lib)
add_executable(client_unit ${SRC})
target_link_libraries(client_unit thrift thriftnb thriftz event)
INCLUDE_DIRECTORIES(../../include/)
INCLUDE_DIRECTORIES(../../../boost/include/)
set_target_properties(client_unit
             PROPERTIES
             COMPILE_FLAGS  "-DHAVE_NETINET_IN_H"
             )
