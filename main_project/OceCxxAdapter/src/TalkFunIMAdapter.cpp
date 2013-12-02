#include "TalkFunIMAdapter.h"

using namespace MyUtil;
using namespace talk::adapter;

TalkFunIMManagerPrx TalkFunIMAdapter::getManager(int id) {
  return getProxy(id);
}

TalkFunIMManagerPrx TalkFunIMAdapter::getManagerOneway(int id) {
  return getProxyOneway(id);
}

void TalkFunIMAdapter::presenceSubscribe(const string& id, int from, int to, const string& status) {
  getManagerOneway(from)->presenceSubscribe(id, from, to, status);
}

void TalkFunIMAdapter::presenceSubscribeWithPostscript(const string& id, int from, int to, 
  const string& status, const string& postscript) {
  getManagerOneway(from)->presenceSubscribeWithPostscript(id, from, to, status, postscript);
}

void TalkFunIMAdapter::presenceSubscribed(const string& id, int from, int to) {
  getManagerOneway(from)->presenceSubscribed(id, from, to);
}

void TalkFunIMAdapter::presenceUnsubscribed(const string& id, int from, int to) {
  getManagerOneway(from)->presenceUnsubscribed(id, from, to);
}

void TalkFunIMAdapter::webPresence(int userId) {
  getManagerOneway(userId)->webPresence(userId);
}

void TalkFunIMAdapter::presence(const JidPtr& jid, const string& show, const string& ver, const string& subver, 
  const string& ext) {
  getManagerOneway(jid->userId)->presence(jid, show, ver, subver, ext);
}

void TalkFunIMAdapter::presenceWithSecver(const JidPtr& jid, const string& show, const string& ver, const string& subver, 
  const string& ext, const string& secver) {
  getManagerOneway(jid->userId)->presenceWithSecver(jid, show, ver, subver, ext, secver);
}

void TalkFunIMAdapter::offline(const JidPtr& jid) {
  getManagerOneway(jid->userId)->offline(jid);
}

void TalkFunIMAdapter::userStatusChanged(int userId, int userStatus){
  getManagerOneway(userId)->userStatusChanged(userId, userStatus);
}

void TalkFunIMAdapter::OnlineCenterPresence(const JidPtr& jid, com::xiaonei::talk::common::OnlineStat stat, 
    const string& ver, const string& ext) {
  getManagerOneway(jid->userId)->OnlineCenterPresence(jid, stat, ver, ext);
}

