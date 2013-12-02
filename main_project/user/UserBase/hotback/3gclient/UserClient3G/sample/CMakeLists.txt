PROJECT(UserClient3GTest)

SET(SRC ./src/main.cpp)
SET(LIB_PATH 
  #ICE的库目录
  /opt/Ice-3.3/lib
  #log4cplus库目录，需要有静态库，版本1.0.4
  ../../../../../../third-party/log4cplus/lib
  #中间层的库目录
  ../lib
  )
SET(CMAKE_CXX_FLAGS "-g -O2")

INCLUDE_DIRECTORIES(../include)
LINK_DIRECTORIES(${LIB_PATH})

#ICE 依赖库
LINK_LIBRARIES(Ice IceGrid)
#中间层客户端依赖库
LINK_LIBRARIES(libUserClient3G.a)
#log4cplus依赖库，由于中间层客户端依赖于此库，此库需要放在后面
LINK_LIBRARIES(liblog4cplus.a)

ADD_EXECUTABLE(UserClient3GTest ${SRC})
