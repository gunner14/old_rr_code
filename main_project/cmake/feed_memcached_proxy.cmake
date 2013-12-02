
FILE( GLOB CPP_SOURCE_N "${XNS_ROOT_DIR}/feed/XceFeedMcProxy/client/*.cc" )
FILE( GLOB CC_SOURCE_N "${XNS_ROOT_DIR}/feed/XceFeedMcProxy/client/*.cpp" )
LIST(APPEND FeedMemcClient_SOURCE_N ${CC_SOURCE_N} ${CPP_SOURCE_N})

FILE( GLOB CPP1_SOURCE_N "${XNS_ROOT_DIR}/feed/XceFeedMcProxy/util/*.cc" )
FILE( GLOB CC1_SOURCE_N "${XNS_ROOT_DIR}/feed/XceFeedMcProxy/util/*.cpp" )
LIST(APPEND MemcProtoParser_SOURCE_N ${CC1_SOURCE_N} ${CPP1_SOURCE_N})

MESSAGE(STATUS "files ${FeedMemcClient_SOURCE_N}")

ADD_LIBRARY(MemcProtoParser_N STATIC ${MemcProtoParser_SOURCE_N})

ADD_LIBRARY(FeedMemcachedClient STATIC ${FeedMemcClient_SOURCE_N})


TARGET_LINK_LIBRARIES(FeedMemcachedClient MemcProtoParser_N)
TARGET_LINK_LIBRARIES(FeedMemcachedClient OceSliceRFeed)
TARGET_LINK_LIBRARIES(FeedMemcachedClient OceSliceFeedMemcProxy)
TARGET_LINK_LIBRARIES(FeedMemcachedClient OceCxxAdapterFeedMemcachedProxyAdapter)
TARGET_LINK_LIBRARIES(FeedMemcachedClient OceProto)
TARGET_LINK_LIBRARIES(FeedMemcachedClient protobuf)
TARGET_LINK_LIBRARIES(FeedMemcachedClient memcached)


INCLUDE_DIRECTORIES(${XNS_ROOT_DIR}/third-party/include/zookeeper-client)
LINK_DIRECTORIES(${XNS_ROOT_DIR}/XCS/cpp/xcs-core/lib)

ADD_SERVICE("feed/XceFeedMcProxy/proxy" "FeedMemcachedProxy" xcs zookeeper_mt  MemcProtoParser_N OceSliceRFeed OceSliceFeedMemcProxy ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool IceExt UtilCxx UtilSlice boost_system-gcc41-mt boost_regex boost_thread)


IF (BREAKPAD_FOUND)
  MESSAGE(STATUS "google breakpad : ${BREAKPAD_INCLUDE_DIR} ${BREAKPAD_LIBRARY}")
  INCLUDE_DIRECTORIES( ${BREAKPAD_INCLUDE_DIR} )
  TARGET_LINK_LIBRARIES(FeedMemcProxy ${BREAKPAD_LIBRARY})
ENDIF (BREAKPAD_FOUND)

