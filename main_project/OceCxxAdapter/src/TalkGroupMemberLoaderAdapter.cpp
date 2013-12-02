#include "TalkGroupMemberLoaderAdapter.h"
#include "IceLogger.h"
#include <boost/lexical_cast.hpp>

using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::group::memberloader;
using namespace MyUtil;


TalkGroupMemberLoaderManagerPrx TalkGroupMemberLoaderAdapter::getManager(long id) {
	return getProxy(id);
}

TalkGroupMemberLoaderManagerPrx TalkGroupMemberLoaderAdapter::getManagerOneway(long id) {
	return getProxyOneway(id);
}

void TalkGroupMemberLoaderAdapter::LoadGroupMember(const MyUtil::LongSeq &idSeq, GroupMemberDict &ans) {
	ans = getManager(idSeq.at(0))->LoadGroupMember(idSeq);	
}

bool TalkGroupMemberLoaderAdapter::AddNewGroup(const GroupMemberDict& memberdict){
	  return getManager(memberdict.begin()->first)->AddNewGroup(memberdict);
}


bool TalkGroupMemberLoaderAdapter::DeleteGroup(const MyUtil::LongSeq &idSeq){
  if (!idSeq.empty()) {
    return getManager(idSeq.at(0))->DeleteGroup(idSeq);
  }
  return true;
}

bool TalkGroupMemberLoaderAdapter::AddNewMember(const long sid, const GroupMemberSeq &members) {
	return getManager(sid)->AddNewMember(sid, members);	
}

bool TalkGroupMemberLoaderAdapter::RemoveMember(const long sid, const MyUtil::IntSeq &idSeq){
  return getManager(sid)->RemoveMember(sid, idSeq);
}

bool TalkGroupMemberLoaderAdapter::SetMember(const GroupMemberDict& memberdict) {
  if (!memberdict.empty()) {
    return getManager(memberdict.begin()->first)->SetMember(memberdict);
  }
  return true;

}
///
void TalkGroupMemberLoaderAdapter::AddNewGroupAsyn(const GroupMemberDict& memberdict) {
  getManagerOneway(memberdict.begin()->first)->AddNewGroupAsyn(memberdict);
}

void TalkGroupMemberLoaderAdapter::DeleteGroupAsyn(const MyUtil::LongSeq &idSeq) {
  if (!idSeq.empty()) {
    getManagerOneway(idSeq.at(0))->DeleteGroupAsyn(idSeq);
  }
}

void TalkGroupMemberLoaderAdapter::AddNewMemberAsyn(const long sid, const GroupMemberSeq &members) {
  getManagerOneway(sid)->AddNewMemberAsyn(sid, members);	
}

void TalkGroupMemberLoaderAdapter::RemoveMemberAsyn(const long sid, const MyUtil::IntSeq &idSeq) {
  getManagerOneway(sid)->RemoveMemberAsyn(sid, idSeq);
}

void TalkGroupMemberLoaderAdapter::SetMemberAsyn(const GroupMemberDict& memberdict) {
  if (!memberdict.empty()) {
    getManagerOneway(memberdict.begin()->first)->SetMemberAsyn(memberdict);
  }

}

