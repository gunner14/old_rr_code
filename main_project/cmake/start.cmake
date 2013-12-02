
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

GET_FILENAME_COMPONENT( XNS_BUILD_DIR "${${PROJECT_NAME}_SOURCE_DIR}" ABSOLUTE )
GET_FILENAME_COMPONENT( XNS_ROOT_DIR "${XNS_BUILD_DIR}/../" ABSOLUTE )
GET_FILENAME_COMPONENT( XNS_CMAKE_DIR "${XNS_ROOT_DIR}/cmake/" ABSOLUTE )

MESSAGE( STATUS "DEBUG: ${${PROJECT_NAME}_SOURCE_DIR}" )
MESSAGE( STATUS "DEBUG: ${XNS_BUILD_DIR}" )
MESSAGE( STATUS "DEBUG: ${XNS_CMAKE_DIR}" )
MESSAGE( STATUS "DEBUG: ${XNS_ROOT_DIR}" )

ADD_CUSTOM_TARGET( deepclean
	COMMAND rm -rf "${XNS_BUILD_DIR}/tmp"
	VERBATIM)

INCLUDE( ${XNS_CMAKE_DIR}/bootstrap.cmake )
#
# Basic Utils
# 
INCLUDE( ${XNS_CMAKE_DIR}/UtilSlice.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/UtilCxx.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/Asio.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/IceExt.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/DbCxxPool.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/OceSlice.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/OceCommon.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/OceCxxAdapter.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/DistCacheCxxAdapter.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/DistCacheSlice.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/DistCacheProto.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/OceProto.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/TalkUtil.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/ReplicaCluster.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/SearchUtil.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/FoFSearchUtil.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/FoFAdapter.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/Base.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/Mongoc.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/Redisc.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/SocialGraphUtil.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/SocialGraphProto.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/DistSearchLogic.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/TripodSearch.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/GeneralSuggestionUtil.cmake )

#INCLUDE( ${XNS_CMAKE_DIR}/Cwf.cmake )
#INCLUDE( ${XNS_CMAKE_DIR}/cwfaction.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/BaseFeed.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/Dbpool.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/Fcgi.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/TripodConfigUtil.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/TripodConfigClient.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/TripodInternalClient.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/TripodClient.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/MessagePipe.cmake )
INCLUDE( ${XNS_CMAKE_DIR}/TripodCore.cmake )


ADD_DEPENDENCIES( TripodConfigUtil OceSlice )
ADD_DEPENDENCIES( TripodConfigClient TripodConfigUtil )
ADD_DEPENDENCIES( TripodInternalClient TripodConfigClient OceCxxAdapterXceLoggerAdapter IceExt)
ADD_DEPENDENCIES( TripodClient TripodInternalClient )
ADD_DEPENDENCIES( IceExt OceSliceXceStorm )
ADD_DEPENDENCIES( OceCommonRelationCommon OceSliceRelation )
ADD_DEPENDENCIES( OceCommonUserCommon OceSliceUser )
ADD_DEPENDENCIES( OceCommonUserConfigCommon OceSliceUserConfig )
ADD_DEPENDENCIES( OceCxxAdapterBatchUpdaterUserCountAdapter OceSliceBatchUpdater )
ADD_DEPENDENCIES( OceCxxAdapterBatchUpdaterAdapter OceSliceBatchUpdater )
ADD_DEPENDENCIES( OceCxxAdapterTalkRightAdapter OceSliceTalkRight )
ADD_DEPENDENCIES( OceCxxAdapterBuddyGroupAdapter OceSliceBuddyGroup )
ADD_DEPENDENCIES( OceCxxAdapterPresenceAdapter OceSlicePresence )
ADD_DEPENDENCIES( OceCxxAdapterOnlineCenterAdapter OceSliceOnlineBuddy )
ADD_DEPENDENCIES( OceCxxAdapterWindowsLiveImporterAdapter OceSliceWindowsLiveImporter )
ADD_DEPENDENCIES( OceCxxAdapterUserCacheAdapter OceCxxAdapterDistUserCacheReloaderAdapter OceSliceUserCache OceSliceUserBase OceProto)
ADD_DEPENDENCIES( OceCxxAdapterUserCountAdapter OceSliceUserCount  OceSliceReplicaCluster OceSliceSubjectObserver)
ADD_DEPENDENCIES( OceCxxAdapterUserAdsCacheAdapter OceSliceUserAdsCache )
ADD_DEPENDENCIES( OceCxxAdapterAntispamRelationAdapter OceSliceAntispamRelation )
ADD_DEPENDENCIES( OceCxxAdapterFriendFinderAdapter OceSliceFriendFinder )
ADD_DEPENDENCIES( OceCxxAdapterActiveTrackFacadeAdapter OceSliceActiveTrack )
ADD_DEPENDENCIES( OceCxxAdapterStatusCacheAdapter OceSliceStatusCache )
ADD_DEPENDENCIES( OceCxxAdapterOnlineStateFilterAdapter OceSliceOnlineStateFilter )
ADD_DEPENDENCIES( OceCxxAdapterPhotoStateFilterAdapter OceSlicePhotoStateFilter )
ADD_DEPENDENCIES( OceCxxAdapterUserStateFilterAdapter OceSliceUserStateFilter )
ADD_DEPENDENCIES( OceCxxAdapterHighSchoolFilterAdapter OceSliceHighSchoolFilter )
ADD_DEPENDENCIES( OceCxxAdapterInviteRewardAdapter OceSliceInviteReward )
ADD_DEPENDENCIES( OceCxxAdapterBuddyByIdReplicaAdapter OceSliceBuddyByIdCache )
ADD_DEPENDENCIES( OceCxxAdapterBuddyFriendsAdapter OceSliceBuddyFriendsCache )
ADD_DEPENDENCIES( OceCxxAdapterBuddyByAddTimeReplicaAdapter OceSliceBuddyByAddTimeCache )
ADD_DEPENDENCIES( OceCxxAdapterBuddyByOnlineTimeReplicaAdapter OceSliceBuddyByOnlineTimeCache )
ADD_DEPENDENCIES( OceCxxAdapterBuddyByNameReplicaAdapter OceSliceBuddyByNameCache )
ADD_DEPENDENCIES( OceCxxAdapterDistBuddyApplyCacheReloaderAdapter OceSliceBuddyApplyCache )
ADD_DEPENDENCIES( OceCxxAdapterDistBuddyByIdCacheReloaderAdapter OceSliceBuddyByIdCache )
ADD_DEPENDENCIES( OceCxxAdapterFriendCountCacheReplicaAdapter OceSliceFriendCountCache )
ADD_DEPENDENCIES( OceCxxAdapterIPRecordReplicaAdapter OceSliceIPRecord )
ADD_DEPENDENCIES( OceCxxAdapterFriendFinderReplicaAdapter OceSliceFriendFinder )
ADD_DEPENDENCIES( OceCxxAdapterFriendFinderByIPReplicaAdapter OceSliceFriendFinderByIP )
ADD_DEPENDENCIES( OceCxxAdapterFriendFinderByMSNReplicaAdapter OceSliceFriendFinderByMSN )
ADD_DEPENDENCIES( OceCxxAdapterFriendFinderByCommonReplicaAdapter OceSliceFriendFinderByCommon )
ADD_DEPENDENCIES( OceCxxAdapterFriendFinderInfoCacheReplicaAdapter OceSliceFriendFinderInfoCache )
ADD_DEPENDENCIES( OceCxxAdapterDailyAccessCacheAdapter OceSliceDailyAccessCache )
ADD_DEPENDENCIES( OceCxxAdapterUserScoreActiveTrackAdapter OceSliceUserScoreActive )
ADD_DEPENDENCIES( OceCxxAdapterUserScoreEventAdapter OceSliceUserScoreEvent )
ADD_DEPENDENCIES( OceCxxAdapterScoreLoginRecordAdapter OceSliceScoreLoginRecord )
ADD_DEPENDENCIES( OceCxxAdapterSocialGraphRcdMessageServiceAdapter OceSliceSocialGraphRcdMessageService )
INCLUDE( ${XNS_CMAKE_DIR}/ScoreCache.cmake )
ADD_DEPENDENCIES( OceCxxAdapterScoreCacheAdapter OceSliceScoreCache )
INCLUDE( ${XNS_CMAKE_DIR}/OfferFriends.cmake )
ADD_DEPENDENCIES( OceCxxAdapterOfferFriendsAdapter OceSliceOfferFriends )
ADD_DEPENDENCIES( OceCxxAdapterFriendFinderNewComerWorkerAdapter OceSliceSocialGraphWorker OceSliceSocialGraphMessage )
ADD_DEPENDENCIES( OceCxxAdapterCardRelationCacheAdapter OceSliceCardCache )
ADD_DEPENDENCIES( OceCxxAdapterFootprintFacadeAdapter OceSliceFootprintUnite )
ADD_DEPENDENCIES( OceCxxAdapterLoveFootprintByIntervieweeAdapter OceSliceFootprintUnite )
ADD_DEPENDENCIES( OceCxxAdapterLoveFootprintByInterviewerAdapter OceSliceFootprintUnite )
ADD_DEPENDENCIES( OceCxxAdapterFootprintViewAdapter OceSliceFootprintView )
ADD_DEPENDENCIES( OceCxxAdapterBuddyXoaLogicAdapter OceSliceBuddyXoaLogic )
ADD_DEPENDENCIES( OceCxxAdapterUserContactWRAdapter OceCxxAdapterUserContactReaderAdapter OceCxxAdapterUserContactWriterAdapter)
ADD_DEPENDENCIES( OceCxxAdapterFeedItemCacheAdapter OceSliceUserFeeds)
ADD_DEPENDENCIES( OceCxxAdapterFeedItemCacheByUseridReplicaAdapter OceSliceUserFeeds)
ADD_DEPENDENCIES( OceCxxAdapterFeedItemCacheByAppidReplicaAdapter OceSliceUserFeeds)
ADD_DEPENDENCIES( OceCxxAdapterFeedItemCacheByAppidAdapter OceSliceUserFeeds)
ADD_DEPENDENCIES( OceCxxAdapterFeedItemWrapperAdapter OceSliceUserFeeds)
ADD_DEPENDENCIES( OceCxxAdapterFeedItemFacadeReplicaAdapter OceSliceUserFeeds)
ADD_DEPENDENCIES( OceCxxAdapterFeedContentCompressDemoAdapter OceSliceFeedContentCompressDemo)
ADD_DEPENDENCIES( OceCxxAdapterFeedNewsAdapter OceSliceRFeed.ice)
ADD_DEPENDENCIES( OceCxxAdapterFeedItemBlockCacheReplicaAdapter OceSliceFeedItemBlockCache OceCxxAdapterFeedItemCacheByUseridReplicaAdapter OceCxxAdapterBuddyByOnlineTimeReplicaAdapter)
ADD_DEPENDENCIES( OceCxxAdapterSocialWikiByUseridReplicaAdapter OceSliceFeedSocialWiki.ice)
ADD_DEPENDENCIES( OceCxxAdapterSocialWikiHotByWikiidReplicaAdapter OceSliceFeedSocialWiki.ice)
ADD_DEPENDENCIES( OceCxxAdapterSocialWikiNewsByWikiidReplicaAdapter OceSliceFeedSocialWiki.ice)
#ADD_DEPENDENCIES( OceCxxAdapterTimeLineFeedReplicaAdapter OceSliceTimeLineFeed.ice)
#ADD_DEPENDENCIES( OceCxxAdapterSocialGraphBuddyByIdAdapter OceSliceSocialGraphBuddyById)

ADD_DEPENDENCIES( UtilCxx UtilSlice )
ADD_DEPENDENCIES( UtilCxx OceSlice )
ADD_DEPENDENCIES( IceExt UtilCxx )
ADD_DEPENDENCIES( DbCxxPool UtilCxx )
ADD_DEPENDENCIES( OceSlice UtilSlice )
ADD_DEPENDENCIES( OceCommon OceSlice )
ADD_DEPENDENCIES( OceCxxAdapter OceCommon )
ADD_DEPENDENCIES( TalkUtil OceSlice )
ADD_DEPENDENCIES( FoFSearchUtil OceSlice UtilCxx )
#ADD_DEPENDENCIES( FoFAdapter IceExt UtilCxx UtilSlice OceSlice )

ADD_DEPENDENCIES( DistCacheCxxAdapter DistCacheSlice DistCacheProto )
ADD_DEPENDENCIES( TripodCore OceProto OceSlicetripod2 )
LINK_LIBRARIES(UtilCxx UtilSlice)

MACRO ( ADD_OCESERVICE directoryname )
	AUX_SOURCE_DIRECTORY("${XNS_ROOT_DIR}/${directoryname}/src" BUILD_${directoryname}_SRC_CPP_SOURCE )
	ADD_LIBRARY( ${directoryname} SHARED EXCLUDE_FROM_ALL ${BUILD_${directoryname}_SRC_CPP_SOURCE})
	TARGET_LINK_LIBRARIES( ${directoryname} Asio OceCxxAdapter OceCommon TalkUtil IceExt DbCxxPool OceSlice UtilCxx UtilSlice)
ENDMACRO ( ADD_OCESERVICE )

MACRO ( ADD_OCESERVICE_DIFFNAME directoryname libraryname )
	AUX_SOURCE_DIRECTORY("${XNS_ROOT_DIR}/${directoryname}/src" BUILD_${directoryname}_SRC_CPP_SOURCE )
	ADD_LIBRARY( ${libraryname} SHARED EXCLUDE_FROM_ALL ${BUILD_${directoryname}_SRC_CPP_SOURCE})
	TARGET_LINK_LIBRARIES( ${libraryname} Asio OceCxxAdapter OceCommon TalkUtil IceExt DbCxxPool OceSlice UtilCxx UtilSlice)
ENDMACRO ( ADD_OCESERVICE_DIFFNAME )

MACRO ( ADD_OCESERVICE_ALL directoryname )
	AUX_SOURCE_DIRECTORY("${XNS_ROOT_DIR}/${directoryname}/src" BUILD_${directoryname}_SRC_CPP_SOURCE )
	ADD_LIBRARY( ${directoryname} SHARED ${BUILD_${directoryname}_SRC_CPP_SOURCE})
	TARGET_LINK_LIBRARIES( ${directoryname} Asio OceCxxAdapter OceCommon TalkUtil IceExt DbCxxPool OceSlice UtilCxx UtilSlice)
ENDMACRO ( ADD_OCESERVICE_ALL )

MACRO ( ADD_OCESERVICE_ALL_DIFFNAME directoryname libraryname )
	AUX_SOURCE_DIRECTORY("${XNS_ROOT_DIR}/${directoryname}/src" BUILD_${directoryname}_SRC_CPP_SOURCE )
	ADD_LIBRARY( ${libraryname} SHARED ${BUILD_${directoryname}_SRC_CPP_SOURCE})
	TARGET_LINK_LIBRARIES( ${libraryname} Asio OceCxxAdapter OceCommon TalkUtil IceExt DbCxxPool OceSlice UtilCxx UtilSlice)
ENDMACRO ( ADD_OCESERVICE_ALL_DIFFNAME )

MACRO ( ADD_OCESERVICE_EXEC directoryname libraryname )
	AUX_SOURCE_DIRECTORY("${XNS_ROOT_DIR}/${directoryname}/src" BUILD_${directoryname}_SRC_CPP_SOURCE )
	ADD_EXECUTABLE( ${libraryname} EXCLUDE_FROM_ALL ${BUILD_${directoryname}_SRC_CPP_SOURCE})
	TARGET_LINK_LIBRARIES( ${libraryname} Asio OceCxxAdapter OceCommon TalkUtil IceExt DbCxxPool OceSlice UtilCxx UtilSlice)
ENDMACRO ( ADD_OCESERVICE_EXEC )

MACRO ( ADD_COMPONENT directoryname libraryname )
	AUX_SOURCE_DIRECTORY("${XNS_ROOT_DIR}/${directoryname}" BUILD_${libraryname}_SRC_CPP_SOURCE )
	ADD_LIBRARY( ${libraryname} STATIC EXCLUDE_FROM_ALL ${BUILD_${libraryname}_SRC_CPP_SOURCE})
	INCLUDE_DIRECTORIES(${XNS_ROOT_DIR}/${directoryname})
	TARGET_LINK_LIBRARIES( ${libraryname} ${ARGN} )
ENDMACRO ( ADD_COMPONENT )

INCLUDE( ${XNS_CMAKE_DIR}/addtional_macro.cmake )

MACRO ( ADD_SERVICE directoryname libraryname )
	AUX_SOURCE_DIRECTORY("${XNS_ROOT_DIR}/${directoryname}" BUILD_${libraryname}_SRC_CPP_SOURCE )
	ADD_LIBRARY( ${libraryname} SHARED EXCLUDE_FROM_ALL ${BUILD_${libraryname}_SRC_CPP_SOURCE})
	TARGET_LINK_LIBRARIES( ${libraryname} ${ARGN} )
ENDMACRO ( ADD_SERVICE )

MACRO ( ADD_SERVICE_ALL directoryname libraryname )
	AUX_SOURCE_DIRECTORY("${XNS_ROOT_DIR}/${directoryname}" BUILD_${libraryname}_SRC_CPP_SOURCE )
	ADD_LIBRARY( ${libraryname} SHARED ${BUILD_${libraryname}_SRC_CPP_SOURCE})
	TARGET_LINK_LIBRARIES( ${libraryname} ${ARGN} )
ENDMACRO ( ADD_SERVICE_ALL )

MACRO ( ADD_SERVICE_EXEC directoryname libraryname )
	MESSAGE(STATUS "DEBUG: ${ARGN}")
	AUX_SOURCE_DIRECTORY("${XNS_ROOT_DIR}/${directoryname}" BUILD_${directoryname}_SRC_CPP_SOURCE )
	ADD_EXECUTABLE( ${libraryname} EXCLUDE_FROM_ALL ${BUILD_${directoryname}_SRC_CPP_SOURCE})
	TARGET_LINK_LIBRARIES( ${libraryname} ${ARGN} )
ENDMACRO ( ADD_SERVICE_EXEC )

MACRO ( ADD_SERVICE_PATH directoryname libraryname)
  INCLUDE_DIRECTORIES(BEFORE "${XNS_ROOT_DIR}/trunk/third-party/thrift")
  AUX_SOURCE_DIRECTORY("${XNS_ROOT_DIR}/${directoryname}" BUILD_${libraryname}_SRC_CPP_SOURCE )
  ADD_LIBRARY( ${libraryname} SHARED EXCLUDE_FROM_ALL ${BUILD_${libraryname}_SRC_CPP_SOURCE})
  TARGET_LINK_LIBRARIES( ${libraryname} ${ARGN})
ENDMACRO ( ADD_SERVICE_PATH )


MACRO ( ADD_DC_SERVICE directoryname libraryname )
	AUX_SOURCE_DIRECTORY("${XNS_ROOT_DIR}/${directoryname}" BUILD_${libraryname}_SRC_CPP_SOURCE )
	ADD_LIBRARY( ${libraryname} SHARED EXCLUDE_FROM_ALL ${BUILD_${libraryname}_SRC_CPP_SOURCE})
	TARGET_LINK_LIBRARIES( ${libraryname} ${ARGN} rdc-client.a libmemcached.so libprotobuf.a libzookeeper_mt.a libactivemq-cpp.a libapr-1.a libaprutil-1.so)
ENDMACRO ( ADD_DC_SERVICE )

MACRO ( ADD_DC_SERVICE_EXEC directoryname libraryname )
	MESSAGE(STATUS "DEBUG: ${ARGN}")
	AUX_SOURCE_DIRECTORY("${XNS_ROOT_DIR}/${directoryname}" BUILD_${directoryname}_SRC_CPP_SOURCE )
	ADD_EXECUTABLE( ${libraryname} EXCLUDE_FROM_ALL ${BUILD_${directoryname}_SRC_CPP_SOURCE})
	TARGET_LINK_LIBRARIES( ${libraryname} ${ARGN} rdc-client.a libmemcached.so libprotobuf.a libzookeeper_mt.a libactivemq-cpp.a libapr-1.a libaprutil-1.so)
ENDMACRO ( ADD_DC_SERVICE_EXEC )

#
# Services
#
INCLUDE( "../cmake/start_user.cmake" )
INCLUDE( "../cmake/start_buddy.cmake" )
INCLUDE( "../cmake/start_app.cmake" )
INCLUDE( "../cmake/start_feed.cmake" )
INCLUDE( "../cmake/start_talk.cmake" )
INCLUDE( "../cmake/start_search.cmake" )
INCLUDE( "../cmake/start_socialgraph.cmake" )
INCLUDE( "../cmake/start_notify.cmake" )
INCLUDE( "../cmake/start_memc_proxy.cmake" )
INCLUDE( "../cmake/start_ad.cmake" )
INCLUDE( "../cmake/start_score.cmake" )
INCLUDE( "../cmake/feed_memc_proxy.cmake" )
INCLUDE( "../cmake/start_training.cmake" )
INCLUDE( "../cmake/start_xoa2.cmake" )
INCLUDE( "../cmake/ad_memc_proxy.cmake")
INCLUDE( "../cmake/start_tripod.cmake")

#
# I will split the following lines.
#

INCLUDE("../cmake/BuddyCore.cmake")
INCLUDE("../cmake/BuddyRelationCache.cmake")
INCLUDE("../cmake/BuddyRelationTT.cmake")
INCLUDE("../cmake/BuddySixDegrees.cmake")
#INCLUDE("../cmake/SearchRelationSlice.cmake")

ADD_OCESERVICE("SvcDescriptor")
ADD_OCESERVICE("ClassApplyCounter")
ADD_OCESERVICE("Account")
ADD_OCESERVICE("Online")

#
# do not use above
# use belows
#

ADD_SERVICE("Transfer/src" "Transfer" ReplicaCluster  OceSliceReplicaCluster Asio OceCommon TalkUtil IceExt DbCxxPool OceSlice UtilCxx UtilSlice ) 

#ADD_SERVICE("BuddyCoreLogic/src" "BuddyCoreLogic"
ADD_DC_SERVICE("BuddyCoreLogic/src" "BuddyCoreLogic"
    OceCxxAdapterBuddyRelationTripodAdapter 
    TripodClient TripodInternalClient TripodConfigClient TripodConfigUtil Base Redisc OceSliceTripodCacheManager OceSliceTripodProducerManager OceSliceTripodData OceSliceTripodException
    OceCxxAdapterBuddyFriendsAdapter OceSliceBuddyFriendsCache
    ReplicaCluster  OceSliceReplicaCluster OceSliceSubjectObserver
    OceCxxAdapterFriendFinderWorkerAdapter	OceSliceSocialGraphWorker
    OceCxxAdapterFriendFinderNewComerWorkerAdapter OceSliceSocialGraphWorker OceSliceSocialGraphMessage
    OceCxxAdapterFriendDistAdapter OceSliceFriendDist
    OceCxxAdapterBatchUpdaterAdapter OceSliceBatchUpdater
    OceCxxAdapterTalkRightAdapter OceSliceTalkRight
    OceCxxAdapterFeedDispatcherAdapter OceSliceRFeed
    OceCxxAdapterBuddyGroupAdapter OceSliceBuddyGroup
    OceCxxAdapterBuddyApplyCacheAdapter OceSliceBuddyApplyCache
    OceCxxAdapterOfferFriendsAdapter OceSliceOfferFriends
    OceCxxAdapterFriendFinderReplicaAdapter OceSliceFriendFinder
    OceCxxAdapterFriendFinderByMSNReplicaAdapter OceSliceFriendFinderByMSN
    OceCxxAdapterRecommendFriendAdapter OceSliceRecommendFriend
    OceCxxAdapterUserCountAdapter OceSliceUserCount
    OceCxxAdapterOnlineCenterAdapter OceSliceOnlineCenter
    OceCxxAdapterPresenceMsgHandlerAdapter OceCxxAdapterIqMsgHandlerAdapter OceSliceTalkLogic
    OceCxxAdapterBuddyByIdReplicaAdapter OceSliceBuddyByIdCache
    OceCxxAdapterBuddyByNameReplicaAdapter OceSliceBuddyByNameCache
    OceCxxAdapterBuddyByOnlineTimeReplicaAdapter OceSliceBuddyByOnlineTimeCache
    OceCxxAdapterBuddyByAddTimeReplicaAdapter OceSliceBuddyByAddTimeCache
    OceCxxAdapterFriendCountCacheReplicaAdapter OceSliceFriendCountCache
    OceCxxAdapterBuddyRelationReplicaAdapter OceCxxAdapterBuddyByIdReplicaAdapter
    OceCxxAdapterSocialGraphLogicAdapter OceSliceSocialGraphLogic
    OceCxxAdapterAntispamRelationAdapter OceSliceAntispamRelation	
    OceCxxAdapterFriendOfFriendIndexerAdapter OceSliceFriendOfFriendIndexer
    OceCxxAdapterBuddyByApplicantAdapter OceSliceBuddyByApplicantCache
    OceCxxAdapterBuddyRelationReplicaAdapter OceSliceBuddyRelationCache OceSliceBuddyCommon
    OceCxxAdapterCardRelationCacheAdapter OceSliceCardCache
    OceCxxAdapterDistBuddyApplyCacheReloaderAdapter 
    OceCxxAdapterDistBuddyByIdCacheReloaderAdapter 
    OceCxxAdapterBuddyXoaLogicAdapter OceSliceBuddyXoaLogic
   	OceCxxAdapterSocialGraphRcdMessageServiceAdapter OceSliceSocialGraphRcdMessageService
    IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)

ADD_SERVICE("NotifyUpdateBackupLogic/src" "NotifyUpdateBackupLogic" IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_SERVICE("UpdateBackupLogic/src" "UpdateBackupLogic" OceCxxAdapterUserLogicAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_SERVICE_EXEC("CxxAdapterTest/src" "CxxAdapterTest" OceCxxAdapterBuddyApplyCacheAdapter OceSliceBuddyApplyCache IceExt OceSliceXceStorm DbCxxPool UtilCxx UtilSlice IceGrid )
#ADD_DC_SERVICE("OnlineEventMediator/src" "OnlineEventMediator" OceSliceOnlineEventMediator OceCxxAdapterDistUserCacheReloaderAdapter OceCxxAdapterUserCacheAdapter OceCxxAdapterDistUserCacheReloaderAdapter OceCxxAdapterBuddyByOnlineTimeReplicaAdapter OceCxxAdapterOnlineStateFilterAdapter ReplicaCluster OceSliceUserCache OceSliceBuddyByOnlineTimeCache OceSliceOnlineStateFilter OceSliceUserBase OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool UtilCxx UtilSlice)
ADD_SERVICE("APNSNotify/src" "APNSNotify"  ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice ssl)
ADD_SERVICE("KeySearch/src" "KeySearch" OceSliceKeySearch IceExt DbCxxPool OceSliceReplicaCluster OceSliceSubjectObserver ReplicaCluster  OceSliceXceStorm   UtilCxx UtilSlice) 

ADD_SERVICE("BuddyApplyCacheN/src" "BuddyApplyCacheN" 
		OceCxxAdapterBuddyApplyCacheAdapter OceSliceBuddyApplyCache
		IceExt OceSliceChecker  OceCxxAdapterBuddyCoreAdapter OceCxxAdapterPresenceMsgHandlerAdapter
		OceCxxAdapterIqMsgHandlerAdapter  OceCxxAdapterBuddyCoreAdapter
		OceCxxAdapterMonitorAdapter OceSliceMonitor
    ReplicaCluster OceSliceBuddyCore OceSlicePresence  OceSliceTalkCommon
		OceSliceTalkCache OceSliceTalkLogic OceSliceReplicaCluster
		OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice
		OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm
		DbCxxPool OceCxxAdapterUserCountAdapter OceSliceUserCount UtilCxx UtilSlice)

ADD_SERVICE("BuddyApplyCacheN/preloader" "BuddyApplyCacheNPreloader" 
		OceCxxAdapterBuddyApplyCacheAdapter OceSliceBuddyApplyCache
		IceExt OceSliceChecker  OceCxxAdapterBuddyCoreAdapter OceCxxAdapterPresenceMsgHandlerAdapter
		OceCxxAdapterIqMsgHandlerAdapter  OceCxxAdapterBuddyCoreAdapter
    ReplicaCluster OceSliceBuddyCore OceSlicePresence  OceSliceTalkCommon
		OceSliceTalkCache OceSliceTalkLogic OceSliceReplicaCluster
		OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice
		OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm
		DbCxxPool OceCxxAdapterUserCountAdapter OceSliceUserCount UtilCxx UtilSlice)

ADD_DC_SERVICE("BuddyApplyCacheN/dist" "DistBuddyApplyCacheReloader" OceProto ReplicaCluster ReplicaCluster OceSlice OceSliceUserCache OceSliceBuddyApplyCache OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool TalkUtil UtilCxx UtilSlice )

#ADD_SERVICE("GeneralDbCache/src" "GeneralDbCache" OceSliceGeneralDbCache OceCxxAdapterPresenceAdapter OceSlicePresence OceSliceTalkCommon IceExt OceSliceXceStorm DbCxxPool OceSliceUserCache TalkUtil UtilCxx UtilSlice mysqlpp)
#ADD_SERVICE("GeneralDbCache/replica" "GeneralDbCacheN" ReplicaCluster OceSliceGeneralDbCache OceCxxAdapterPresenceAdapter OceSlicePresence OceSliceTalkCommon IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSliceUserCache TalkUtil UtilCxx UtilSlice mysqlpp)

ADD_SERVICE("CompareInfo/src" "CompareInfo" OceCxxAdapterCompareInfoReloaderAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver OceSliceCompareInfo  OceSliceUserBase IceExt OceSliceXceStorm DbCxxPool UtilCxx UtilSlice)

ADD_SERVICE("CompareInfo/reloader" "CompareInfoReloader"  OceCxxAdapterCompareInfoAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool OceSliceCompareInfo OceSliceUserBase  UtilCxx UtilSlice)

ADD_SERVICE("CompareInfo/preloader" "CompareInfoPreloader" OceCxxAdapterPresenceAdapter ReplicaCluster  OceSliceReplicaCluster OceSliceSubjectObserver OceSlicePresence OceSliceCompareInfo OceSliceTalkCommon IceExt OceSliceXceStorm DbCxxPool TalkUtil UtilCxx UtilSlice)


ADD_SERVICE("WindowsLiveImporter/src" "WindowsLiveImporter" IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceCxxAdapterImporterMSNWorkerAdapter OceSliceImporterMSNWorker OceCxxAdapterWindowsLiveImporterAdapter OceSliceWindowsLiveImporter OceSliceWindowsLiveSchema Asio UtilCxx UtilSlice curl ldap idn ssl)

ADD_SERVICE("XceStorm/src" "XceStorm" OceCxxAdapterMonitorAdapter OceSliceMonitor IceExt ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice mysqlpp)


ADD_SERVICE("DbDescriptor/src" "DbDescriptor" OceCxxAdapterMonitorAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver OceSliceMonitor IceExt OceSliceXceStorm DbCxxPool UtilCxx UtilSlice )
ADD_SERVICE_EXEC("DbDescriptor/dump" "dumpDbObservers" IceExt UtilCxx )

ADD_SERVICE("XceAlert/src" "XceAlert" OceCxxAdapterMonitorAdapter ReplicaCluster IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice )
ADD_SERVICE("Monitor/alerter" "MonitorAlerter" OceCxxAdapterMonitorAdapter OceSliceMonitor ReplicaCluster OceSliceMonitor OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool UtilCxx UtilSlice util curl ldap idn ssl)
ADD_SERVICE("Monitor/analyzer" "MonitorAnalyzer" OceCxxAdapterMonitorAdapter ReplicaCluster OceSliceMonitor OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool UtilCxx UtilSlice util)
ADD_SERVICE_EXEC("Monitor/client/mashine" "MonitorMashineClient" OceCxxAdapterMonitorAdapter OceSliceMonitor ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx util UtilSlice)
ADD_SERVICE_EXEC("Monitor/tools/sendemail" "SendEmail" OceCxxAdapterMonitorAdapter OceSliceMonitor ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx util UtilSlice)
ADD_SERVICE_EXEC("Monitor/tools/stopheartbeat" "StopHeartBeat" OceCxxAdapterMonitorAdapter OceSliceMonitor ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx util UtilSlice)
ADD_SERVICE_EXEC("Monitor/tools/reloadalerter" "ReloadAlerter" OceCxxAdapterMonitorAdapter OceSliceMonitor ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx util UtilSlice)
ADD_SERVICE_EXEC("Monitor/tools/reloadanalyzer" "ReloadAnalyzer" OceCxxAdapterMonitorAdapter OceSliceMonitor ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx util UtilSlice)
ADD_SERVICE_EXEC("Monitor/tools/alertclient" "AlertClient" OceCxxAdapterMonitorAdapter OceSliceMonitor ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx util UtilSlice)
ADD_SERVICE_EXEC("Monitor/tools/taskmanager" "TaskManagerMonitor" OceCxxAdapterMonitorAdapter OceSliceMonitor ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx util UtilSlice)
ADD_SERVICE("Monitor/heartbeater" "MonitorHeartBeater" OceCxxAdapterMonitorAdapter ReplicaCluster OceSliceMonitor OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool UtilCxx UtilSlice util)
ADD_SERVICE("Monitor/dispatcher" "MonitorDispatcher" OceCxxAdapterMonitorAdapter ReplicaCluster OceSliceMonitor OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool UtilCxx UtilSlice util)

ADD_SERVICE_EXEC("XceAlert/test" "sendMessage" OceCxxAdapterXceAlertAdapter OceSliceXceAlert ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)

ADD_DC_SERVICE("DistCacheMonitor/monitor/" "DistCacheMonitor" OceProto OceCxxAdapterXceAlertAdapter OceCxxAdapterDistCacheMonitorAdapter OceCxxAdapterDistCacheSwitcherAdapter ReplicaCluster OceSliceXceAlert OceSliceDistCacheMonitor IceExt OceSliceXceStorm DbCxxPool TalkUtil UtilCxx OceSliceReplicaCluster OceSliceSubjectObserver UtilSlice )
ADD_DC_SERVICE("DistCacheMonitor/switcher/" "DistCacheSwitcher" OceProto ReplicaCluster OceSliceXceAlert OceSliceDistCacheMonitor IceExt OceSliceXceStorm DbCxxPool TalkUtil UtilCxx OceSliceReplicaCluster OceSliceSubjectObserver UtilSlice )
ADD_SERVICE("GeneralDbService/src" "GeneralDbService" ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool OceSlice  OceSliceGeneralDbService UtilCxx UtilSlice)
ADD_SERVICE_EXEC("CallGuarantee/main" "CallGuaranteeTest" IceExt OceSliceXceStorm DbCxxPool UtilCxx UtilSlice)
ADD_SERVICE_EXEC("tools/HAStatusClient" "hacli" IceExt ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice)
ADD_SERVICE_EXEC("tools/TaskManagerClient" "tmcli" IceExt ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice)

ADD_SERVICE("ReplicaCluster/ClusterController/src" "ClusterController" OceCxxAdapterMonitorAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)

ADD_SERVICE_EXEC("Checker/server" "Checker" OceCxxAdapterXceAlertAdapter OceCxxAdapterBuddyByIdReplicaAdapter OceSliceBuddyByIdCache OceCxxAdapterBuddyApplyCacheAdapter OceSliceBuddyApplyCache ReplicaCluster OceSliceChecker IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice )

ADD_SERVICE("BuddyGroup/src" "BuddyGroup"  IceExt OceCxxAdapterPresenceMsgHandlerAdapter OceCxxAdapterBuddyCoreAdapter OceCxxAdapterIqMsgHandlerAdapter OceCxxAdapterMonitorAdapter ReplicaCluster OceSliceBuddyGroup OceSliceBuddyCore OceSliceMonitor OceSlicePresence  OceSliceTalkCommon OceSliceTalkCache OceSliceTalkLogic OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice)

ADD_SERVICE("Ticket" "Ticket" OceCxxAdapterMonitorAdapter OceSliceTicket ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)

ADD_SERVICE("Login/logic" "LoginLogic" OceCxxAdapterMonitorAdapter OceCxxAdapterIdCacheReaderAdapter OceSliceIdCache OceCxxAdapterUserStateAdapter OceCxxAdapterUserPassportAdapter OceSliceUserBase OceCxxAdapterLoginCacheAdapter OceCxxAdapterXceLoggerAdapter OceSliceTicket OceSliceLogin ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_SERVICE("Login/cache" "LoginCache" OceCxxAdapterMonitorAdapter OceSliceLogin ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)

#ADD_SERVICE("Bulletin/admin" "BulletinAdmin" OceCxxAdapterBulletinAdapter OceSliceBulletin ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)

#ADD_SERVICE("Quest/logic" "QuestLogic" OceCxxAdapterQuestCacheAdapter OceSliceQuest ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)

#ADD_SERVICE("Quest/cache" "QuestCache" OceSliceQuest ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx util UtilSlice)

ADD_SERVICE("XceMonitor/src" "XceMonitor" OceSliceXceMonitor OceCxxAdapterMonitorAdapter IceExt ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver OceSliceMonitor OceSliceXceStorm DbCxxPool UtilCxx util UtilSlice)

ADD_SERVICE_EXEC("XceMonitor/unsubscribe" "Unsubscribe" OceSliceXceMonitor ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx util UtilSlice)

#ADD_SERVICE_EXEC("GetLocalIP/src" "GetLocalIP" IceExt UtilCxx UtilSlice )
#ADD_SERVICE("BatchUpdater/src" "BatchUpdaterN" OceCxxAdapterBatchUpdaterUserCountReplicaAdapter OceSliceBatchUpdater ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice)
INCLUDE( "../cmake/DbDescriptor.cmake" )
ADD_SERVICE("EvaCollect/src" "EvaCollect" OceSliceXceLogger IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm UtilCxx UtilSlice)
INCLUDE_DIRECTORIES( /usr/local/xce-dev/include/log4cxx )
LINK_DIRECTORIES( /usr/local/xce-dev/lib )
ADD_SERVICE("logging/XceLogger/src" "XceLogger" IceExt OceSliceXceStorm DbCxxPool OceSliceXceLogger ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver UtilCxx UtilSlice )

ADD_SERVICE("KeepUse" "KeepUse" OceSliceXceStorm IceExt OceSlice DbCxxPool OceSliceKeepUse UtilCxx UtilSlice IceGrid Ice)


ADD_DC_SERVICE_EXEC("GeneralCheck/src" "GeneralCheck" OceCxxAdapterUserNetworkOldAdapter OceCxxAdapterUserNetworkWriterAdapter OceCxxAdapterUserNetworkReaderAdapter OceCxxAdapterUserNetworkAdapter OceCxxAdapterUserBaseAdapter OceCxxAdapterUserBaseReaderAdapter OceCxxAdapterUserBornAdapter OceCxxAdapterUserDoingAdapter OceCxxAdapterUserLoginCountAdapter OceCxxAdapterUserNameAdapter OceCxxAdapterUserPassportAdapter OceCxxAdapterUserRightAdapter OceCxxAdapterUserStageAdapter OceCxxAdapterUserStateAdapter OceCxxAdapterUserUrlAdapter OceCxxAdapterUserContactAdapter OceCxxAdapterUserContactReaderAdapter OceCxxAdapterUserContactWriterAdapter OceCxxAdapterUserTimeAdapter OceCxxAdapterUserTimeReaderAdapter OceCxxAdapterUserTimeWriterAdapter OceCxxAdapterUserDescAdapter OceCxxAdapterUserDescReaderAdapter OceCxxAdapterUserBornAdapter OceSliceUserBase OceCxxAdapterUserCacheAdapter OceCxxAdapterDistUserCacheReloaderAdapter OceCxxAdapterUserConfigAdapter OceCxxAdapterUserConfigReaderAdapter OceCxxAdapterUserAdsCacheAdapter OceCxxAdapterUserAdsCacheLoaderAdapter OceCxxAdapterDistUserApiCacheReloaderAdapter OceCxxAdapterUserApiCacheAdapter OceCxxAdapterBuddyByIdReplicaAdapter OceCxxAdapterBuddyByNameReplicaAdapter OceCxxAdapterBuddyByAddTimeReplicaAdapter OceCxxAdapterBuddyByOnlineTimeReplicaAdapter OceCxxAdapterStatusCacheAdapter OceSliceBuddyByNameCache OceSliceBuddyByIdCache OceSliceBuddyByOnlineTimeCache OceSliceBuddyByAddTimeCache OceSliceUserApiCache OceSliceUserAdsCache OceSliceUserNetwork OceSliceStatusCache OceSliceUserCache ReplicaCluster IceExt OceSliceReplicaCluster OceSliceUserConfig OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice OceProto )
#----------------------------------------------------------

#ADD_DC_SERVICE_EXEC("message_pipe/src/cpp" "MessagePipe")

ADD_DC_SERVICE_EXEC("tripod-new/src/cpp/admin" "TripodAdmin" IceExt DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_DC_SERVICE("tripod-new/src/cpp/producermanager" "TripodProducerManagerCC" OceSliceTripodProducerManager OceCxxAdapterMonitorAdapter OceSliceTripodData MiniGroupCommon ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt DbCxxPool OceSlice UtilCxx UtilSlice TripodSearch OceProto TripodClient TripodInternalClient TripodConfigClient TripodConfigUtil OceSliceTripodCacheManager OceSliceTripodData OceSliceTripodException OceSliceXceLogger)
ADD_DC_SERVICE_EXEC("search/tripodsearchcachetest/load" "testTripodSearchLoad" OceSliceTripodProducerManager OceCxxAdapterMonitorAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver TripodSearch TripodClient TripodInternalClient TripodConfigClient TripodConfigUtil OceSliceTripodCacheManager OceSliceTripodData OceSliceTripodException OceSliceXceLogger DbCxxPool OceSlice UtilCxx UtilSlice  OceProto IceExt)
ADD_DC_SERVICE_EXEC("search/tripodsearchcachetest/get" "testTripodSearchGet" OceSliceTripodProducerManager OceCxxAdapterMonitorAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver TripodSearch TripodClient TripodInternalClient TripodConfigClient TripodConfigUtil OceSliceTripodCacheManager OceSliceTripodData OceSliceTripodException OceSliceXceLogger DbCxxPool OceSlice UtilCxx UtilSlice  OceProto IceExt)
ADD_DC_SERVICE_EXEC("tripod-new/src/cpp/configutil" "testTripodConfigUtil" TripodConfigUtil) 
ADD_DC_SERVICE_EXEC("tripod-new/src/cpp/configclient" "testTripodConfigClient" TripodConfigClient TripodConfigUtil OceSliceTripodData Base Redisc OceSlice) 
ADD_DC_SERVICE_EXEC("tripod-new/src/cpp/internalclient" "testTripodInternalClient" TripodInternalClient TripodConfigClient TripodConfigUtil Base Redisc OceSlice IceExt DbCxxPool UtilCxx UtilSlice) 
ADD_DC_SERVICE_EXEC("tripod-new/src/cpp/client" "testTripodClient" TripodClient TripodInternalClient TripodConfigClient TripodConfigUtil Base Redisc OceSlice IceExt DbCxxPool UtilCxx UtilSlice) 
ADD_DC_SERVICE_EXEC("tripod-new/src/cpp/warmer" "TripodWarmer" TripodClient TripodInternalClient TripodConfigClient TripodConfigUtil Base Redisc OceSlice IceExt DbCxxPool UtilCxx UtilSlice) 
ADD_DC_SERVICE("tripod-new/src/cpp/cachemanager" "TripodCacheManager" TripodClient TripodInternalClient TripodConfigClient TripodConfigUtil Base Redisc OceSliceTripodCacheManager OceSliceTripodProducerManager OceSliceTripodData OceSliceTripodException OceCxxAdapterMonitorAdapter OceProto OceSliceMonitor ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool mysqlpp UtilCxx UtilSlice breakpad)
ADD_DC_SERVICE("tripod-new/src/cpp/monitor" "TripodMonitor" TripodConfigClient TripodConfigUtil OceSliceTripodCacheManager OceSliceTripodProducerManager OceSliceTripodData OceSliceTripodException OceCxxAdapterMonitorAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver OceSlice IceExt DbCxxPool UtilCxx UtilSlice) 
ADD_DC_SERVICE_EXEC("DataSync/sync" "SyncManager" OceCxxAdapterMonitorAdapter OceSliceMonitor ReplicaCluster OceSliceDataSync OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool UtilCxx UtilSlice OceProto MessagePipe IceExt)
#改用gyp，不再兼容CMake
#ADD_DC_SERVICE_EXEC("DataSync/dispatch" "DispatchManager" TripodInternalClient TripodConfigClient TripodConfigUtil Base Redisc OceSliceTripodCacheManager OceSliceTripodProducerManager OceSliceTripodData OceSliceTripodException OceCxxAdapterMonitorAdapter OceSliceMonitor ReplicaCluster OceSliceDataSync OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool UtilCxx UtilSlice OceProto MessagePipe IceExt)
ADD_DC_SERVICE("DataSync/localproducermanager" "LocalProducerManager" OceSliceTripodException OceCxxAdapterMonitorAdapter OceSliceMonitor ReplicaCluster OceSliceDataSync OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool UtilCxx UtilSlice OceProto MessagePipe IceExt)
ADD_DC_SERVICE_EXEC("DataSync/remoteconsumer" "RemoteConsumer" OceCxxAdapterMonitorAdapter OceSliceMonitor ReplicaCluster OceSliceDataSync OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool UtilCxx UtilSlice OceProto MessagePipe IceExt)
ADD_DC_SERVICE_EXEC("DataSync/test" "testSync" OceCxxAdapterMonitorAdapter OceSliceMonitor ReplicaCluster OceSliceDataSync OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceDataSync OceSliceXceStorm DbCxxPool UtilCxx UtilSlice OceProto MessagePipe IceExt)
