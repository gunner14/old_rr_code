## Copyright 2012-10-10 shiqiang.ding@renren-inc.com

PROJECT(XCS)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## compile options
# release mode 
SET(COMMON_RELEASE_FLAGS "-g -O2 -Wall -fPIC")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${COMMON_RELEASE_FLAGS}")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COMMON_RELEASE_FLAGS}")
# debug mode
SET(COMMON_DEBUG_FLAG "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${COMMON_DEBUG_FLAG}")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COMMON_DEBUG_FLAG}")

## link options
SET(LDFLAGS, "-fprofile-arcs -ftest-coverage")

## Write your own configure here

## Set boost environment
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)

SET(BOOST_ROOT /usr/local/xce)
FIND_PACKAGE(Boost COMPONENTS thread)   # for LogWrap.h
#FIND_PACKAGE( Boost COMPONENTS date_time system regex thread filesystem)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
LINK_LIBRARIES(${Boost_LIBRARIES})  # for LogWrap.h

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src SRC_LIST)
INCLUDE_DIRECTORIES(/data/home/yuwei.mu/xce/include/zookeeper-client
                    ${PROJECT_SOURCE_DIR}/src
                    ${PROJECT_SOURCE_DIR}/../../../   # for LogWrap.h
                    /opt/Ice-3.3/include
                    /usr/local/xce/include  # for log4cplus
                    ${PROJECT_SOURCE_DIR}/../../../UtilCxx/src)
LINK_DIRECTORIES(/usr/local/xce/lib  # for log4cplus
                 /data/home/yuwei.mu/xce/lib
                 /opt/Ice-3.3/lib)

SET(TARGET_NAME "xcs")
ADD_LIBRARY(${TARGET_NAME} STATIC ${SRC_LIST})
TARGET_LINK_LIBRARIES(${TARGET_NAME} libzookeeper_mt.a libcrypto.a
                                     Ice IceUtil log4cplus  # for log4cplus
                                     boost_thread-gcc41-mt)
INSTALL(TARGETS ${TARGET_NAME} ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/lib)

FILE(GLOB HEAD_LIST "${PROJECT_SOURCE_DIR}/src/*.h")
INSTALL(FILES ${HEAD_LIST} DESTINATION ${PROJECT_SOURCE_DIR}/include)
## test
IF(CMAKE_BUILD_TYPE MATCHES "DEBUG")
  INCLUDE(CTest)
  INCLUDE(Dart)
  ENABLE_TESTING()
  ADD_SUBDIRECTORY(tests)
  # test name
  SET(CTEST_PROJECT_NAME "${CMAKE_PROJECT_NAME}")
  MESSAGE("CTEST name: ${CTEST_PROJECT_NAME}")
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "DEBUG")
