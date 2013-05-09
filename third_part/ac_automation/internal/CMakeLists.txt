project(ac_automaton)
link_directories(../../../base/lib)
set(LIB_SRC ac_automaton.cc ac_automaton_unit.cc utils.cc)
add_library(libac_automaton ${LIB_SRC})
target_link_libraries(libac_automaton base)
set_target_properties(libac_automaton
           PROPERTIES
           COMPILE_FLAGS    "-fPIC -m64 -g -O3 -DOS_LINUX -pthread"
           )
set_target_properties(libac_automaton PROPERTIES OUTPUT_NAME "ac_automaton")
