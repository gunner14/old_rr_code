# Gtest generate path
SET(GTEST_DIR $ENV{GTEST_DIR})
SET(GTEST_INCLUDE_DIRECTORIES ${GTEST_DIR}/include/)
INCLUDE_DIRECTORIES(${GTEST_DIR} ${GTEST_INCLUDE_DIRECTORIES})

ADD_LIBRARY(gtest ${GTEST_DIR}/src/gtest-all.cc)
ADD_LIBRARY(gtest-main ${GTEST_DIR}/src/gtest_main.cc)

FIND_PACKAGE(GTest)

## Unit test start here
INCLUDE_DIRECTORIES("../")

SET(TestArgs null)
ADD_EXECUTABLE(hello_world_u hello_world_u.cc)
TARGET_LINK_LIBRARIES(hello_world_u hello_world gtest pthread)

GTEST_ADD_TESTS(hello_world_u "${TestArgs}" hello_world_u.cc)

SET(CTEST_MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --quiet --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=100 --verbose --demangle=yes")
