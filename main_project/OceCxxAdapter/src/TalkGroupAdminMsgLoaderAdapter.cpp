#include "TalkGroupAdminMsgLoaderAdapter.h"
#include "IceLogger.h"
#include <boost/lexical_cast.hpp>

using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::group::admmsgloader;
using namespace MyUtil;


TalkGroupAdminMsgLoaderPrx TalkGroupAdminMsgLoaderAdapter::getManager(int id) {
	return getProxy(id);
}

TalkGroupAdminMsgLoaderPrx TalkGroupAdminMsgLoaderAdapter::getManagerOneway(int id) {
	return getProxyOneway(id);
}

MyUtil::StrSeq TalkGroupAdminMsgLoaderAdapter::getMsg(int uid) {
  return getManager(uid)->getMsg(uid);
}

MyUtil::StrSeq TalkGroupAdminMsgLoaderAdapter::getdelMsg(int uid) {
  return getManager(uid)->getdelMsg(uid);
}


bool TalkGroupAdminMsgLoaderAdapter::saveMsg(int uid, const string &msg) {
  return getManager(uid)->saveMsg(uid, msg);	
}

bool TalkGroupAdminMsgLoaderAdapter::delMsg(int uid) {
  return getManager(uid)->delMsg(uid);
}

void TalkGroupAdminMsgLoaderAdapter::saveMsgAsyn(int uid, const string &msg) {
  getManagerOneway(uid)->saveMsgAsyn(uid, msg);
}

void TalkGroupAdminMsgLoaderAdapter::delMsgAsyn(int uid) {
  getManagerOneway(uid)->delMsgAsyn(uid);
}


