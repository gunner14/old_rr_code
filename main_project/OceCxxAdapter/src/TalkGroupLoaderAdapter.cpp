#include "TalkGroupLoaderAdapter.h"
#include "IceLogger.h"
#include <boost/lexical_cast.hpp>

using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::group::loader;
using namespace MyUtil;


TalkGroupLoaderManagerPrx TalkGroupLoaderAdapter::getManager(long id) {
	return getProxy(id);
}

TalkGroupLoaderManagerPrx TalkGroupLoaderAdapter::getManagerOneway(long id) {
	return getProxyOneway(id);
}



void TalkGroupLoaderAdapter::LoadGroupInfo(const MyUtil::LongSeq &idSeq, GroupInfoDict &infodict){
	infodict = getManager(idSeq.at(0))->LoadGroupInfo(idSeq);	
}
/*
void TalkGroupLoaderAdapter::GetCreatSessions(int creater, MyUtil::LongSeq &idseq){
	idseq = getManager(creater)->GetCreatSessions(creater);	
}
*/
bool TalkGroupLoaderAdapter::SetGroupInfo(const GroupInfoPtr &groupInfo){
	return getManager(groupInfo->id)->SetGroupInfo(groupInfo);
}

bool TalkGroupLoaderAdapter::AddGroupInfo(const GroupInfoPtr &groupInfo){
	return getManager(groupInfo->id)->AddGroupInfo(groupInfo);
}


bool TalkGroupLoaderAdapter::DeleteGroupInfo(const MyUtil::LongSeq& idSeq){
  if (!idSeq.empty()) {
    return getManager(*idSeq.begin())->DeleteGroupInfo(idSeq);

  }
  return true;
}
bool TalkGroupLoaderAdapter::UpdateGroupInfo(long sid, const MyUtil::Str2StrMap &props){
  return getManager(sid)->UpdateGroupInfo(sid, props);

}



void TalkGroupLoaderAdapter::SetGroupInfoAsyn(const GroupInfoPtr &groupInfo){
  getManagerOneway(groupInfo->id)->SetGroupInfoAsyn(groupInfo);
}

void TalkGroupLoaderAdapter::AddGroupInfoAsyn(const GroupInfoPtr &groupInfo){
  getManagerOneway(groupInfo->id)->AddGroupInfoAsyn(groupInfo);
}


void TalkGroupLoaderAdapter::DeleteGroupInfoAsyn(const MyUtil::LongSeq& idSeq){
  if (!idSeq.empty()) {
    getManagerOneway(*idSeq.begin())->DeleteGroupInfo(idSeq);

  }

}

void TalkGroupLoaderAdapter::UpdateGroupInfoAsyn(long sid, const MyUtil::Str2StrMap &props){
  getManagerOneway(sid)->UpdateGroupInfoAsyn(sid, props);


}

