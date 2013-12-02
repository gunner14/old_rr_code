#include "TalkGroupViewLoaderAdapter.h"
#include "IceLogger.h"

using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::groupview::loader;
using namespace MyUtil;

GroupViewLoaderManagerPrx TalkGroupViewLoaderAdapter::getManager(int id) {
	return getProxy(id);
}

GroupViewLoaderManagerPrx TalkGroupViewLoaderAdapter::getManagerOneway(int id) {
  return getProxyOneway(id);
}

void TalkGroupViewLoaderAdapter::UpdateAsyn(int actor, const TalkGroupViewPtr &groupinfo) {
  getManagerOneway(actor)->UpdateAsyn(actor, groupinfo);

}

void TalkGroupViewLoaderAdapter::DeleteAsyn(int userid, long sessionid, int type) {
  getManagerOneway(userid)->DeleteAsyn(userid, sessionid, type);
}

bool TalkGroupViewLoaderAdapter::Update(int actor, const TalkGroupViewPtr &groupinfo) {
  return getManager(actor)->Update(actor, groupinfo);

}

bool TalkGroupViewLoaderAdapter::UpdateMsgid(int uid, long sessionid, int type, long msgid) {
  return getManager(uid)->UpdateMsgid(uid, sessionid, type, msgid);
}

bool TalkGroupViewLoaderAdapter::UpdateItem(int userid, long sid, const Str2StrMap &props) {
  return getManager(userid)->UpdateItem(userid, sid, props);
}

void TalkGroupViewLoaderAdapter::UpdateItemAsyn(int userid, long sid, const Str2StrMap &props) {
  getManagerOneway(userid)->UpdateItemAsyn(userid, sid, props);
}


bool TalkGroupViewLoaderAdapter::Delete(int userid, long sessionid, int type) {
  return getManager(userid)->Delete(userid, sessionid, type);
}


void TalkGroupViewLoaderAdapter::GetCreatSessions(int creater, MyUtil::LongSeq &idseq){
	idseq = getManager(creater)->GetCreatSessions(creater);	
}


/*
void TalkGroupViewLoaderAdapter::LoadById(int owner, int type, TalkGroupViewSeq &groupviews) {
  groupviews = getManager(owner)->LoadById(owner, type);
}
*/
void TalkGroupViewLoaderAdapter::LoadBySeq(const MyUtil::IntSeq &owners, int type, TalkGroupViewSeqMap &groupviewsdict) {
	if (!owners.empty()) {
    groupviewsdict = getManager(owners.size())->LoadBySeq(owners, type);
	}

}


