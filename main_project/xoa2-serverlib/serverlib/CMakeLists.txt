## Copyright 2012-11-27 zhe.long@renren-inc.com
#项目的名称
PROJECT(SERVERLIB)

#使用的cmake版本不能低于2.8
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## compile options
# release mode
# 选项 -Wall 开启编译器几乎所有常用的警告
# -O2 表示编译的时候优化的等级
# -g 生成调试信息。GNU 调试器可利用该信息。
# -fPIC 使.so文件的代码段变为真正意义上的共享
#如果不加-fPIC,则加载.so文件的代码段时,代码段引用的数据对象需要重定位,
#重定位会修改代码段的内容,这就造成每个使用这个.so文件代码段的进程在内核里都会生成这个.so文件代码段的copy.每个copy都不一样,取决于
#这个.so文件代码段和数据段内存映射的位置.
SET(COMMON_RELEASE_FLAGS "-g -O2 -Wall -fPIC")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${COMMON_RELEASE_FLAGS}")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COMMON_RELEASE_FLAGS}")
# debug mode
#SET(COMMON_DEBUG_FLAG "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
SET(COMMON_DEBUG_FLAG "-g -O0 -Wall -w")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${COMMON_DEBUG_FLAG}")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COMMON_DEBUG_FLAG}")

## link options
#SET(LDFLAGS, "-fprofile-arcs -ftest-coverage")

## Write your own configure here

## Set boost environment
SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREAD OFF)

SET(BOOST_INCLUDE ${PROJECT_SOURCE_DIR}/../include/boost)
SET(BOOST_LIB ${PROJECT_SOURCE_DIR}/../lib/boost)
INCLUDE_DIRECTORIES(${BOOST_INCLUDE})
LINK_DIRECTORIES(${BOOST_LIB})
#Finds and loads settings from an external project
#FIND_PACKAGE(Boost COMPONENTS thread)   # for LogWrap.h
#FIND_PACKAGE(Boost COMPONENTS date_time system regex thread filesystem)

#Add the given directories to those searched by the compiler for include files.
#Specify the paths in which the linker should search for libraries
#LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
#Specify a list of libraries to be linked into any following targets
#LINK_LIBRARIES(${Boost_LIBRARIES})  # for LogWrap.h


SET(THRIFT_INCLUDE ${PROJECT_SOURCE_DIR}/../include ${PROJECT_SOURCE_DIR}/../include/thrift)
SET(THRIFT_LIB ${PROJECT_SOURCE_DIR}/../lib/thrift)
INCLUDE_DIRECTORIES(${THRIFT_INCLUDE})
LINK_DIRECTORIES(${THRIFT_LIB})

SET(LIBEVENT_INCLUDE ${PROJECT_SOURCE_DIR}/../include/libevent)
SET(LIBEVENT_LIB ${PROJECT_SOURCE_DIR}/../lib/libevent)
INCLUDE_DIRECTORIES(${LIBEVENT_INCLUDE})
LINK_DIRECTORIES(${LIBEVENT_LIB})

SET(EXPAT_INCLUDE ${PROJECT_SOURCE_DIR}/../include/expat)
SET(EXPAT_LIB ${PROJECT_SOURCE_DIR}/../lib/expat)
INCLUDE_DIRECTORIES(${EXPAT_INCLUDE})
LINK_DIRECTORIES(${EXPAT_LIB})


SET(SERVERLIB_ROOT ${PROJECT_SOURCE_DIR})
INCLUDE(./thrift-idl/start_xoa2.cmake )

SET(ZK_INCLUDE ${PROJECT_SOURCE_DIR}/../include/zookeeper-client)
SET(ZK_LIB ${PROJECT_SOURCE_DIR}/../lib/zookeeper)
INCLUDE_DIRECTORIES(${ZK_INCLUDE})
LINK_DIRECTORIES(${ZK_LIB})

SET(XCS_INCLUDE ${PROJECT_SOURCE_DIR}/../include/xcs)
SET(XCS_LIB ${PROJECT_SOURCE_DIR}/../lib/xcs)
INCLUDE_DIRECTORIES(${XCS_INCLUDE})
LINK_DIRECTORIES(${XCS_LIB})


AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src SRC_LIST)
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/src
                    ${PROJECT_SOURCE_DIR}/../include/log4plus  # for log4cplus
                    ${PROJECT_SOURCE_DIR}/../include/UtilCxx/src)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../lib/util)

SET(TARGET_LIB_NAME "serverlib")
#Adds a library target called <name> to be built from the source files listed in the command invocation
ADD_LIBRARY(${TARGET_LIB_NAME} STATIC ${SRC_LIST})
#Specify libraries or flags to use when linking a given target
TARGET_LINK_LIBRARIES(${TARGET_LIB_NAME} libzookeeper_mt.a libcrypto.a libthrift.a
                                     Ice IceUtil log4cplus  # for log4cplus
                                     boost_thread-gcc41-mt)
#使用make install命令的时候执行
INSTALL(TARGETS ${TARGET_LIB_NAME} ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/lib)


#
#SET(TARGET_NAME "server")
#INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/thrift-gen/gen-cpp)
#LINK_DIRECTORIES(${PROJECT_SOURCE_DIR})
#ADD_EXECUTABLE(${TARGET_NAME} ${PROJECT_SOURCE_DIR}/test/twitter_service.cc)
#TARGET_LINK_LIBRARIES(${TARGET_NAME} ${TARGET_LIB_NAME} libOceThrift_twitter.a
#libOceThrift_renren.a libthriftnb.a libevent.a libxcs.a libexpat.a)

#GLOB will generate a list of all files that match the globbing expressions and store it into the variable
FILE(GLOB HEAD_LIST "${PROJECT_SOURCE_DIR}/src/*.h")
INSTALL(FILES ${HEAD_LIST} DESTINATION ${PROJECT_SOURCE_DIR}/include)


