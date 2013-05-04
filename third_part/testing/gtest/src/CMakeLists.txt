project(gtest)
set(LIB_SRC benchmark.cc
            gtest.cc
            gtest-printers.cc
            gtest-death-test.cc
            gtest-filepath.cc
            gtest-port.cc
            gtest-test-part.cc
            gtest-typed-test.cc
            gtest_main.cc
            benchmark_main.cc
            ../../gmock/src/gmock-cardinalities.cc
            ../../gmock/src/gmock.cc
            ../../gmock/src/gmock-cardinalities.cc
            ../../gmock/src/gmock-matchers.cc
            ../../gmock/src/gmock-spec-builders.cc
            )
add_library(libgtest ${LIB_SRC})
set_target_properties(libgtest PROPERTIES OUTPUT_NAME "gtest")
