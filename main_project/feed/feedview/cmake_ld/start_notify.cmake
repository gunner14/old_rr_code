INCLUDE_DIRECTORIES(/data/home/yuwei.mu/xce/include)
LINK_DIRECTORIES(/data/home/yuwei.mu/xce/lib)

ADD_COMPONENT("Notify/util" "NotifyUtil" OceSliceTicket OceCxxAdapterTicketAdapter)

ADD_SERVICE("Notify/index" "NotifyIndex" OceSliceNotify OceCxxAdapterIqMsgHandlerAdapter OceSliceUserBase OceSliceTalkLogic OceSliceTalkCache OceSliceTalkCommon OceCxxAdapterNotifyContentAdapter ReplicaCluster IceExt  OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice )
ADD_SERVICE("Notify/content" "NotifyContent" OceSliceNotify ReplicaCluster IceExt  OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice )
ADD_SERVICE("Notify/dispatcher_n" "NotifyDispatcherN" NotifyUtil OceSliceRFeed OceSliceNotify OceCxxAdapterFeedContentAdapter OceCxxAdapterFeedAssistantNAdapter OceCxxAdapterNotifyDispatcherAdapter OceCxxAdapterNotifyIndexAdapter OceCxxAdapterNotifyContentAdapter ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice TalkUtil ctemplate)
ADD_SERVICE("Notify/dispatcher" "NotifyDispatcher" NotifyUtil OceSliceUserBase OceSliceNotify OceCxxAdapterUserPassportAdapter OceCxxAdapterNotifyDispatcherNAdapter ReplicaCluster IceExt  OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice ctemplate)

ADD_SERVICE("Notify/gate" "NotifyGate" NotifyUtil OceSliceWTransit OceSliceNotify OceCxxAdapterFeedNewsAdapter OceCxxAdapterFeedItemAdapter OceCxxAdapterWTransitAdapter OceCxxAdapterAPNSNotifyAdapter OceCxxAdapterNotifyIndexAdapter ReplicaCluster IceExt  OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm OceSliceAPNSNotify OceSliceRFeed OceSliceTalkCommon DbCxxPool UtilCxx UtilSlice fcgi ctemplate)
ADD_SERVICE("Notify/redirection" "Redirection" NotifyUtil OceSliceNotify OceSliceRedirection OceCxxAdapterTransferManagerAdapter OceCxxAdapterTalkProxyAdapter  ReplicaCluster IceExt  OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm  OceSlice DbCxxPool UtilCxx UtilSlice fcgi ctemplate)

ADD_SERVICE("Notify/feed_gate" "FeedGate" NotifyUtil OceSliceNotify OceSliceRFeed OceCxxAdapterFeedContentAdapter OceCxxAdapterFeedAssistantNAdapter OceCxxAdapterFeedContentAdapter ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice TalkUtil fcgi ctemplate)

ADD_SERVICE("Notify/admin" "NotifyAdmin" NotifyUtil OceSliceNotify OceCxxAdapterNotifyGateAdapter OceCxxAdapterNotifyDispatcherNAdapter ReplicaCluster IceExt  OceSliceReplicaCluster OceSliceSubjectObserver OceSliceXceStorm DbCxxPool UtilCxx UtilSlice fcgi ctemplate)

ADD_SERVICE("Notify/notepad" "TalkNotepad" NotifyUtil OceSliceTicket OceCxxAdapterTicketAdapter OceSliceTalkNotepad ReplicaCluster IceExt OceSliceReplicaCluster OceSliceSubjectObserver OceSliceTalkCommon OceSliceXceStorm DbCxxPool UtilCxx UtilSlice fcgi ctemplate)

