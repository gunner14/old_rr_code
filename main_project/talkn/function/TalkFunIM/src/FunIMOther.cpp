#include "FunIMOther.h"
#include "TalkProxyAdapter.h"
#include "TalkCommon.h"
#include "XmppTools.h"

using namespace com::xiaonei::talk::function::im::other;
using namespace com::xiaonei::talk;
using namespace com::xiaonei::talk::util;
using namespace com::xiaonei::talk::adapter;
using namespace com::xiaonei::talk::common;

void FunIMOther::ProcessPingRequest(const Str2StrMap& paras) {
  const string FuncName = "ProcessPingRequest::Handle";
  TimeStatN ts;

  string jid_string, id;
  JidPtr jid;
  ParasGetValue("actor_jid", jid_string, paras);
  ParasGetValue("iq:id", id, paras);
  try {
    jid = stringToJid(jid_string);
    if(!jid)
      return;
  } catch(...) {
    MCE_WARN(FuncName << "stringToJid error");
  }

  string reply = "<iq type=\"result\" to=\"" + jid_string + "\" from=\"talk.renren.com\" id=\"" + id + "\" />";
  try {
    TalkProxyAdapter::instance().express(jid, reply);
  } catch(Ice::Exception& e) {
    MCE_WARN(FuncName << "-->TalkProxyAdapter::express error" << e);
  }

  FunStatManager::instance().Stat(FuncName, ts.getTime(), (ts.getTime() > 50.0));
}
