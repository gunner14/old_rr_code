FILE( GLOB CPP1_SOURCE "${XNS_ROOT_DIR}/ad/AdMemcProxy/util/*.cc" )
FILE( GLOB CC1_SOURCE "${XNS_ROOT_DIR}/ad/AdMemcProxy/util/*.cpp" )
LIST(APPEND AdMemcProtoParser_SOURCE ${CC1_SOURCE} ${CPP1_SOURCE})

ADD_LIBRARY(AdMemcProtoParser STATIC ${AdMemcProtoParser_SOURCE})

ADD_SERVICE_EXEC("ad/AdMemcProxy/client" "memcclient" AdMemcProtoParser  ReplicaCluster Ice OceSlice OceSliceAdMemcProxy OceProto protobuf IceExt)

ADD_SERVICE("ad/AdMemcProxy/proxy" "AdMemcProxy" AdMemcProtoParser OceSliceAdMemcProxy ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool IceExt UtilCxx UtilSlice boost_system-gcc41-mt boost_regex boost_thread)


#ADD_SERVICE_EXEC("ad/lrucache/src" "LRUCache"  AdMemcProtoParser Ice IceExt OceSlice UtilCxx UtilSlice breakpad)

IF (BREAKPAD_FOUND)
  MESSAGE(STATUS "google breakpad : ${BREAKPAD_INCLUDE_DIR} ${BREAKPAD_LIBRARY}")
  INCLUDE_DIRECTORIES( ${BREAKPAD_INCLUDE_DIR} )
  TARGET_LINK_LIBRARIES(AdMemcProxy ${BREAKPAD_LIBRARY})
ENDIF (BREAKPAD_FOUND)

