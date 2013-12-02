#INCLUDE_DIRECTORIES(/data/home/zhigang.wu/include)
#LINK_DIRECTORIES(/data/home/zhigang.wu/lib)

ADD_COMPONENT("search/NameCheck/neparse" "neparse") 
ADD_COMPONENT("search/NameCheck/simple_segment" "simple_segment" simple_index)
ADD_COMPONENT("search/NameCheck/simple_index" "simple_index" )
ADD_SERVICE("search/NameCheck/src" "NameCheck" neparse simple_segment simple_index  OceSliceReplicaCluster OceSliceSubjectObserver ReplicaCluster IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice )
ADD_SERVICE("search/SearchMatch" "SearchMatch" OceSliceReplicaCluster OceSliceSubjectObserver ReplicaCluster IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice )
ADD_DC_SERVICE("search/FriendInput/src" "FriendInput"  OceSliceReplicaCluster OceSliceSubjectObserver OceSliceFriendInput OceSliceUserCache OceCxxAdapterUserCacheAdapter OceCxxAdapterDistUserCacheReloaderAdapter ReplicaCluster OceCxxAdapterFriendInputAdapter IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice OceProto OceCxxAdapterBuddyByIdReplicaAdapter)

ADD_COMPONENT("search/GeneralInputSuggest/src" "GeneralInputSuggest"  IceExt UtilCxx )
ADD_SERVICE("search/ZujiInputSuggest" "ZujiInputSuggest" GeneralInputSuggest OceSliceGeneralQS OceSliceReplicaCluster OceSliceSubjectObserver ReplicaCluster IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice )


ADD_SERVICE("search/MiniGroupInput" "MiniGroupInput" OceSlicePageInput IceExt  OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_SERVICE("search/SearchFriendCache/src" "SearchFriendCache" OceCxxAdapterBuddyRelationReplicaAdapter OceCxxAdapterBuddyFriendsAdapter OceSliceReplicaCluster ReplicaCluster OceSliceSearchShortestPath OceSliceSearchFriendCache IceExt  OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_SERVICE("search/PageInput" "PageInput" OceSlicePageInput IceExt  OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_SERVICE("search/Suggestion" "Suggestion" OceSliceSuggestion IceExt  OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_DC_SERVICE("search/GeneralSuggestion2/suggestion" "GeneralSuggestion" OceSliceReplicaCluster OceSliceSubjectObserver OceSliceSuggestion OceSliceUserCache GeneralSuggestionUtil OceCxxAdapterUserCacheAdapter OceCxxAdapterDistUserCacheReloaderAdapter OceProto ReplicaCluster OceCxxAdapterBuddyByIdReplicaAdapter IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_DC_SERVICE("search/GeneralSuggestion2/seconddegreefriends" "SecondDegreeFriendsSuggestion" OceSliceReplicaCluster OceSliceSubjectObserver OceSliceSuggestion OceSliceUserCache GeneralSuggestionUtil OceCxxAdapterUserCacheAdapter OceCxxAdapterDistUserCacheReloaderAdapter OceProto ReplicaCluster OceCxxAdapterBuddyByIdReplicaAdapter IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_SERVICE("DoingSearchFacade/src" "DoingSearchFacade" OceCxxAdapterDoingSearchNewContainerAdapter ReplicaCluster  IceExt  OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_SERVICE("search/Card/CardRelationCache" "CardRelationCache" OceCxxAdapterUserContactAdapter OceCxxAdapterUserContactReaderAdapter OceCxxAdapterUserContactWriterAdapter OceCxxAdapterUserCountAdapter OceSliceUserDesc OceSliceCardCache OceSliceUserCount ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx util UtilSlice)
ADD_SERVICE("search/Card/CardInfoCache" "CardInfoCache" OceCxxAdapterUserContactAdapter OceCxxAdapterUserContactReaderAdapter OceCxxAdapterUserContactWriterAdapter OceSliceCardCache ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_SERVICE("search/Card/CardImportCache" "CardImportCache" OceCxxAdapterUserContactAdapter OceCxxAdapterUserContactReaderAdapter OceCxxAdapterUserContactWriterAdapter OceSliceCardCache ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice OceCxxAdapterBuddyByIdReplicaAdapter)
ADD_DC_SERVICE("search/Card/CardSearcher" "CardSearcher" OceCxxAdapterCardRelationCacheAdapter OceCxxAdapterUserCacheAdapter OceCxxAdapterDistUserCacheReloaderAdapter OceCxxAdapterBuddyByNameReplicaAdapter OceCxxAdapterCardInfoCacheAdapter OceSliceCardSearcher OceSliceCardCache ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_DC_SERVICE("search/Card/CardImportSearcher" "CardImportSearcher" OceCxxAdapterCardRelationCacheAdapter OceCxxAdapterUserCacheAdapter OceCxxAdapterDistUserCacheReloaderAdapter OceCxxAdapterBuddyByNameReplicaAdapter OceCxxAdapterCardInfoCacheAdapter OceCxxAdapterCardImportCacheAdapter OceSliceCardSearcher OceSliceCardCache ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_SERVICE("search/FriendOfFriendSearch/FriendOfFriendCache/src" "FriendOfFriendCache" OceSliceSubjectObserver OceSliceFriendOfFriendCache OceCxxAdapterBuddyByIdReplicaAdapter OceSliceBuddyByIdCache IceExt DbCxxPool UtilCxx UtilSlice ReplicaCluster OceSliceReplicaCluster OceSliceXceStorm)
ADD_SERVICE("search/FriendOfFriendSearch/Indexer/src" "FriendOfFriendIndexer"  OceSliceSubjectObserver OceSliceFriendOfFriendIndexer OceCxxAdapterFriendOfFriendCacheAdapter OceSliceFriendOfFriendCache IceExt DbCxxPool OceSliceXceStorm UtilCxx UtilSlice FoFSearchUtil OceSliceReplicaCluster)
ADD_SERVICE("search/FriendOfFriendSearch2/searcher/src" "FriendOfFriendSearcher" FoFAdapter OceCxxAdapterBuddyByIdReplicaAdapter FoFSearchUtil  OceCxxAdapterFriendOfFriendIndexerAdapter OceSliceReplicaCluster OceSliceSubjectObserver OceSliceSearchServer OceSliceSearchModel ReplicaCluster IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_SERVICE("search/SearchCache2/src/PersistentCache" "SearchPersistentCache" OceCxxAdapterSearchCacheAdapter OceCxxAdapterSearchMemCacheAdapter OceCxxAdapterTTManagerAdapter ReplicaCluster SearchUtil IceExt OceSliceSearchCache OceSliceXceStorm OceSliceReplicaCluster OceSliceSubjectObserver DbCxxPool UtilCxx UtilSlice OceSlice tokyotyrant tokyocabinet)
ADD_SERVICE("search/SearchCache2/src/SearchCacheLogic" "SearchLogic" OceCxxAdapterSearchCacheAdapter OceCxxAdapterTTManagerAdapter ReplicaCluster SearchUtil IceExt OceSliceSearchCache OceSliceXceStorm OceSliceReplicaCluster OceSliceSubjectObserver DbCxxPool UtilCxx UtilSlice OceSlice tokyotyrant tokyocabinet)
ADD_SERVICE("search/SearchCache2/src/tt_manager" "TTManager" SearchUtil IceExt OceSliceSearchCache OceCxxAdapterTTManagerAdapter UtilCxx UtilSlice OceSlice DbCxxPool)
ADD_SERVICE("search/SearchCache2/src/MemCache" "SearchMemCache" SearchUtil IceExt OceSliceSearchCache OceSliceXceStorm DbCxxPool UtilCxx UtilSlice OceSlice tokyotyrant tokyocabinet)
ADD_SERVICE_EXEC("search/FriendInput/src/FriendInputTest" "FriendInputTest"  OceSliceReplicaCluster OceSliceSubjectObserver OceSliceFriendInput OceSliceUserCache OceCxxAdapterUserBaseAdapter ReplicaCluster OceCxxAdapterFriendInputAdapter OceCxxAdapterBuddyByIdReplicaAdapter IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
ADD_SERVICE_EXEC("search/FriendOfFriendSearch/library/test" "test_map" FoFSearchUtil OceSliceReplicaCluster OceSliceSubjectObserver ReplicaCluster IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice memcached)
ADD_SERVICE_EXEC("search/FriendOfFriendSearch/FriendOfFriendCache/tools/" "FriendOfFriendCacheTool" UtilSlice OceCxxAdapterFriendOfFriendCacheAdapter OceSlice IceExt UtilCxx)
ADD_SERVICE_EXEC("search/FriendOfFriendSearch/Indexer/tools/" "FriendOfFriendIndexerTool" UtilSlice OceCxxAdapterFriendOfFriendIndexerAdapter OceSlice IceExt UtilCxx FoFSearchUtil memcached)
ADD_SERVICE("search/ShareSearch/ShareIndexLogic/src" "ShareIndexLogic" OceCxxAdapterShareIndexContainerSubAAdapter OceCxxAdapterShareIndexContainerSubBAdapter ReplicaCluster IceExt OceSliceShareSearch OceSliceXceStorm OceSliceReplicaCluster OceSliceSubjectObserver DbCxxPool UtilCxx UtilSlice OceSlice)
#ADD_SERVICE("search/BlogSearch/src/BlogIndexLogic/src" "BlogUpdateLogic"  OceCxxAdapterBlogSubIndexAdapter ReplicaCluster IceExt OceSliceContentSearch OceSliceContentSearchRmi  OceSliceXceStorm OceSliceReplicaCluster OceSliceSubjectObserver DbCxxPool UtilCxx UtilSlice OceSlice)
ADD_SERVICE_EXEC("search/FriendOfFriendSearch2/adapter/test/" "AdapterTest"  FoFSearchUtil FoFAdapter OceSliceReplicaCluster OceSliceSubjectObserver ReplicaCluster IceExt       OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)
#ADD_SERVICE("search/DataCenter/MainService/src" "SearchDataCenter" OceCxxAdapterFlushSearchDataAdapter OceCxxAdapterSubIndexAdapter ReplicaCluster IceExt OceSliceContentSearch OceSliceContentSearchRmi OceSliceHotWord OceSliceXceStorm  OceSliceReplicaCluster OceSliceSubjectObserver DbCxxPool UtilCxx UtilSlice OceSlice)

#ADD_SERVICE("search/DataCenter/FlushDataService/src" "FlushSearchData" ReplicaCluster IceExt OceSliceContentSearch OceSliceFlushSearchData OceSlice OceSliceXceStorm  OceSliceReplicaCluster OceSliceSubjectObserver DbCxxPool UtilCxx UtilSlice OceSlice)

ADD_SERVICE("search/HotStateSubscribe/src" "HotStateSubscribe" ReplicaCluster IceExt OceSliceXceStorm OceSliceContentSearch OceSliceHotStateSubscribe OceSliceReplicaCluster OceSliceSubjectObserver DbCxxPool UtilCxx UtilSlice OceSlice)

ADD_SERVICE_EXEC("search/HotStateSubscribe/test" "HotStateSubscribeTest" OceCxxAdapterHotStateSubscribeAdapter ReplicaCluster IceExt OceSliceXceStorm OceSliceContentSearch OceSliceHotStateSubscribe OceSliceReplicaCluster OceSliceSubjectObserver DbCxxPool UtilCxx UtilSlice OceSlice)

ADD_SERVICE_EXEC("search/FriendOfFriendSearch2/searcher/SearcherTest/src" "FriendOfFriendSearcherTest" OceCxxAdapterFriendOfFriendSearcherAdapter OceSliceReplicaCluster OceSliceSubjectObserver OceSliceSearchServer OceSliceSearchModel ReplicaCluster IceExt OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice)

ADD_SERVICE_EXEC("search/SearchCache2/src/SearchCacheLoader" "SearchCacheLoader" IceExt OceSliceSearchCache OceSliceXceStorm DbCxxPool UtilCxx UtilSlice OceCxxAdapterSearchMemCacheAdapter OceSlice SearchUtil tokyotyrant tokyocabinet)


#TT_LOADER_TEST
ADD_SERVICE_EXEC("search/SearchCache2/src/tools/load_tt_to_memcache_test" "SearchCacheLoadTest" OceSliceXceStorm DbCxxPool IceExt UtilCxx UtilSlice OceSlice SearchUtil tokyotyrant tokyocabinet)

ADD_SERVICE_EXEC("search/SearchCache2/src/tools/load_tt_to_memcache" "SearchCacheLoad" OceSliceXceStorm DbCxxPool UtilCxx UtilSlice OceSlice SearchUtil IceExt tokyotyrant tokyocabinet)

ADD_SERVICE_EXEC("search/SearchCache2/src/tools/tt_manager_tools" "tt_manager_tools" IceExt OceSliceSearchCache OceSliceXceStorm  DbCxxPool UtilCxx UtilSlice OceCxxAdapterTTManagerAdapter OceSlice SearchUtil tokyotyrant tokyocabinet)
ADD_SERVICE_EXEC("search/SearchCache2/src/tools/check_view_count_and_update" "check_view_count_and_update"  OceCxxAdapterSearchCacheAdapter  ReplicaCluster SearchUtil OceSliceSearchCache OceSliceXceStorm OceSliceReplicaCluster OceSliceSubjectObserver DbCxxPool UtilCxx IceExt UtilSlice OceSlice tokyotyrant tokyocabinet )
ADD_SERVICE_EXEC("search/SearchCache2/src/tools/tt_loader" "tt_loader" OceSliceSearchCache OceSliceXceStorm DbCxxPool IceExt UtilCxx UtilSlice OceSlice SearchUtil tokyotyrant tokyocabinet)
ADD_SERVICE_EXEC("search/SearchCache2/src/tools/check_and_update_critical_info" "check_and_update_critical_info" OceCxxAdapterSearchCacheAdapter ReplicaCluster SearchUtil OceSliceSearchCache OceSliceXceStorm OceSliceReplicaCluster OceSliceSubjectObserver DbCxxPool UtilCxx IceExt UtilSlice OceSlice tokyotyrant tokyocabinet)

#ADD_DC_SERVICE("search/SearchCache2/src/DistSearchCacheLogic" "DistSearchCacheLogic" ReplicaCluster OceSliceReplicaCluster OceProto OceSliceSubjectObserver TripodSearch TripodClient TripodInternalClient TripodConfigClient TripodConfigUtil IceExt OceSliceXceStorm DbCxxPool SearchUtil OceSliceSearchCache  UtilCxx UtilSlice OceSlice tokyotyrant tokyocabinet)
ADD_DC_SERVICE("search/SearchCache2/src/DistSearchCacheLogic" "DistSearchCacheLogic" OceCxxAdapterSearchPeopleProxyAdapter ReplicaCluster OceSliceReplicaCluster OceProto OceSliceSubjectObserver TripodSearch TripodClient TripodInternalClient TripodConfigClient TripodConfigUtil IceExt OceSliceXceStorm DbCxxPool SearchUtil OceSliceSearchCache  UtilCxx json_linux-gcc-4.1.2_libmt UtilSlice OceSlice tokyotyrant tokyocabinet Base Redisc Base)
#ADD_COMPONENT("search/PeopleSearchKafkaClient/src" "PeopleSearchKafkaClient")
#ADD_SERVICE("search/SearchCache2/src/SearchPeopleProxy/src" "SearchPeopleProxy" PeopleSearchKafkaClient ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool SearchUtil UtilCxx  UtilSlice OceSlice )
#ADD_SERVICE_EXEC("search/PeopleSearchKafkaClient/unittest" "PeopleSearchKafkaClientTest"  PeopleSearchKafkaClient UtilCxx  boost_test_exec_monitor-gcc41-mt)



ADD_DC_SERVICE("search/SearchCache2/src/DistSearchCacheLoader" "DistSearchCachePreloader" OceProto ReplicaCluster OceSliceSearchCache OceSliceReplicaCluster                     OceSliceSubjectObserver IceExt OceSliceXceStorm DbCxxPool SearchUtil UtilCxx UtilSlice tokyotyrant tokyocabinet)

ADD_DC_SERVICE_EXEC("search/SearchCache2/src/tools/tt_content_checker" "tt_content_checker" OceCxxAdapterSearchLogicAdapter OceCxxAdapterDistSearchLogicAdapter                  OceCxxAdapterDistSearchCacheAdapter OceProto ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver  OceSliceSearchCache OceSliceXceStorm DbCxxPool UtilCxx IceExt        UtilSlice OceSlice SearchUtil tokyotyrant tokyocabinet)


ADD_DC_SERVICE_EXEC("search/SearchCache2/src/tools/SearchCacheTool" "search_cache_tool" OceCxxAdapterSearchCacheAdapter OceCxxAdapterDistSearchLogicAdapter                      OceCxxAdapterDistSearchCacheAdapter OceCxxAdapterSearchLogicAdapter DistSearchLogic OceProto ReplicaCluster IceExt OceSliceSearchCache OceSliceXceStorm OceSliceReplicaCluster   OceSliceSubjectObserver DbCxxPool UtilCxx UtilSlice OceSlice SearchUtil tokyotyrant tokyocabinet)





ADD_SERVICE_EXEC("search/SearchCache2/src/UnitTest/test_search_mem_cache" "TestSearchMemCache" IceExt OceSliceSearchCache OceSliceXceStorm  DbCxxPool UtilCxx UtilSlice OceCxxAdapterSearchMemCacheAdapter OceSlice SearchUtil tokyotyrant tokyocabinet)
ADD_SERVICE_EXEC("search/SearchCache2/src/tools/tt_manager_mem_id_list_file_loader" "tt_manager_mem_id_list_file_loader" IceExt OceSliceSearchCache UtilCxx UtilSlice OceCxxAdapterTTManagerAdapter OceSlice SearchUtil tokyotyrant tokyocabinet)

ADD_COMPONENT("search/SearchRelation/SearchZkClient/src" "SearchZkClient" zookeeper_mt)
ADD_SERVICE_EXEC("search/SearchRelation/SearchZkClient/test" "SearchZkClientTest" SearchZkClient boost_test_exec_monitor-gcc41-mt)

ADD_COMPONENT("search/SearchRelation/SearchRelationCxxAdapter/src" "SearchRelationCxxAdapter" SearchRelationSlice SearchZkClient)
ADD_SERVICE_EXEC("search/SearchRelation/SearchRelationCxxAdapter/test" "SearchRelationCxxAdapterTest" SearchRelationCxxAdapter zookeeper_mt boost_test_exec_monitor-gcc41-mt)



#ADD_SERVICE_EXEC("search/IndexerSrcDataGenerator/src" "IndexerSrcDataGenerator" OceSliceXceStorm OceSliceReplicaCluster ReplicaCluster OceSliceSubjectObserver OceSlice DbCxxPool UtilCxx IceExt    UtilSlice) 
