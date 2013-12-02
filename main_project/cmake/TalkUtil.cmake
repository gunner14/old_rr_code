

FILE( GLOB BUILD_SRC_CPP_SOURCE "${XNS_ROOT_DIR}/TalkUtil/src/*.cpp" "${XNS_ROOT_DIR}/TalkUtil/src/expat/*.c")

ADD_LIBRARY( TalkUtil ${BUILD_SRC_CPP_SOURCE})

