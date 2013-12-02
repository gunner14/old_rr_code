#include "TalkFunPhoneStateAdapter.h"


using namespace com::xiaonei::talk::fun::state;
using namespace MyUtil;


void TalkFunPhoneStateAdapter::getProxySeqForSet(int id, const char *fun, vector<TalkFunPhoneStateManagerPrx> &prxs) {
  prxs = getAllProxySeqOneway(id);
  if (prxs.empty()) {
    MCE_WARN(fun << " --> getAllProxySeqOneway empty id:" << id);
  }

}


void TalkFunPhoneStateAdapter::HeartBeatPhone(const MyUtil::IntSeq& uids) {
  if (uids.empty()) return;
  vector<TalkFunPhoneStateManagerPrx> prxs;
  const char *fun = "TalkFunPhoneStateAdapter::HeartBeatPhone";
  getProxySeqForSet(uids.at(0), fun, prxs);

  for (vector<TalkFunPhoneStateManagerPrx>::iterator it = prxs.begin(); it != prxs.end(); ++it) {
    try {
      (*it)->HeartBeatPhone(uids);
    }	catch (Ice::Exception& e) {
      MCE_WARN(fun << "[LoadTask::handle] "<< (*it) << " load failed");
			MCE_WARN(fun <<  " Ice::Exception: " << e.what() << " at " << __FILE__ << ":" << __LINE__ );
    }	catch(...) {
			MCE_WARN(fun << "[LoadTask::handle] "<< (*it) << " load failed, unknown reason" );
		}

  }
}

void TalkFunPhoneStateAdapter::Offline(int uid) {
  vector<TalkFunPhoneStateManagerPrx> prxs;
  const char *fun = "TalkFunPhoneStateAdapter::Offline";
  getProxySeqForSet(uid, fun, prxs);

  for (vector<TalkFunPhoneStateManagerPrx>::iterator it = prxs.begin(); it != prxs.end(); ++it) {
    try {
      (*it)->Offline(uid);
    }	catch (Ice::Exception& e) {
      MCE_WARN(fun << "[LoadTask::handle] "<< (*it) << " load failed");
			MCE_WARN(fun <<  " Ice::Exception: " << e.what() << " at " << __FILE__ << ":" << __LINE__ );
    }	catch(...) {
			MCE_WARN(fun << "[LoadTask::handle] "<< (*it) << " load failed, unknown reason" );
		}

  }

}

void TalkFunPhoneStateAdapter::Online(int uid) {
  vector<TalkFunPhoneStateManagerPrx> prxs;
  const char *fun = "TalkFunPhoneStateAdapter::Online";
  getProxySeqForSet(uid, fun, prxs);

  for (vector<TalkFunPhoneStateManagerPrx>::iterator it = prxs.begin(); it != prxs.end(); ++it) {
    try {
      (*it)->Online(uid);
    }	catch (Ice::Exception& e) {
      MCE_WARN(fun << "[LoadTask::handle] "<< (*it) << " load failed");
			MCE_WARN(fun <<  " Ice::Exception: " << e.what() << " at " << __FILE__ << ":" << __LINE__ );
    }	catch(...) {
			MCE_WARN(fun << "[LoadTask::handle] "<< (*it) << " load failed, unknown reason" );
		}

  }

}

void TalkFunPhoneStateAdapter::getUsersIds(const MyUtil::IntSeq& uids, MyUtil::IntSeq &res) {
  if (uids.empty()) return;

  TalkFunPhoneStateManagerPrx prx = getProxy(uids.at(0));
  res = prx->getUsersIds(uids);
}
