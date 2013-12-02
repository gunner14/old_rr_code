# Gtest generate path
SET(GTEST_DIR $ENV{GTEST_DIR})
SET(GTEST_INCLUDE_DIRECTORIES ${GTEST_DIR}/include/)
INCLUDE_DIRECTORIES(${GTEST_DIR} ${GTEST_INCLUDE_DIRECTORIES})

MESSAGE(${GTEST_DIR})
ADD_LIBRARY(gtest ${GTEST_DIR}/src/gtest-all.cc)
ADD_LIBRARY(gtest-main ${GTEST_DIR}/src/gtest_main.cc)
FIND_PACKAGE(GTest)

## Unit test start here
INCLUDE_DIRECTORIES(../src)
SET(TestArgs null)

AUX_SOURCE_DIRECTORY(. DEMO_LIST)
FOREACH(demo ${DEMO_LIST})
  GET_FILENAME_COMPONENT(demo_target ${demo} NAME_WE)
  ADD_EXECUTABLE(${demo_target} ${demo})
  TARGET_LINK_LIBRARIES(${demo_target} ${TARGET_NAME} gtest gtest-main pthread)

  IF(demo_target MATCHES "xcs_service_factory_demo")
    ADD_CUSTOM_COMMAND(
      TARGET ${demo_target}
      POST_BUILD
      COMMAND cp ${PROJECT_SOURCE_DIR}/tests/client.conf ${PROJECT_BINARY_DIR}/tests
      COMMAND cp ${PROJECT_SOURCE_DIR}/tests/server.conf ${PROJECT_BINARY_DIR}/tests
    )
  ELSE(demo_target MATCHES "xcs_service_factory_demo")
    ADD_CUSTOM_COMMAND(
      TARGET ${demo_target}
      POST_BUILD
      COMMAND cp ${PROJECT_SOURCE_DIR}/tests/${demo_target}.conf ${PROJECT_BINARY_DIR}/tests
    )
  ENDIF(demo_target MATCHES "xcs_service_factory_demo")
  GTEST_ADD_TESTS(${demo_target} "${TestArgs}" ${demo})
ENDFOREACH(demo ${DEMO_LIST})

SET(CTEST_MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --quiet --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=100 --verbose --demangle=yes")
