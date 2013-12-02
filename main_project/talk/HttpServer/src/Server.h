#ifndef SERVER_H_
#define SERVER_H_

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>
#include <boost/lexical_cast.hpp>
//

#include "Singleton.h"
//#include "Session.h"
#include "TalkCommon.h"
#include "ServiceI.h"

#include "Refactor.h"
#include "WTalkProxyAdapter.h"

namespace talk {
namespace http {

using namespace MyUtil;
using namespace com::xiaonei::talk::common;
using namespace std;
using namespace talk::adapter;


class Session;
typedef IceUtil::Handle<Session> SessionPtr;

class DeliverStat {
 public:
  enum {
    NOTSTAT_THIS,
    RESPONSE_200,
    RESPONSE_211, //"code:211, comet old connection close"

    UNKNOWN_URL_404,   // 404 ERROR[404]:unknowen url
    LOGIN_FIRST_403, // 403 ERROR[403]: please login first
    VERIFY_FALSE_403, // 403 ERROR[403]: verify is false
    USERID_BIND_ERROR_403,    // 403 ERROR[403]: userid bind err
    INVALID_USERID_403,    // 403 invalid user id
    COMET_BROAD_NOT_LOGIN_403,    // 403    do comet broadcast , but not login
    MSG_ISNOT_XML_403, // 403 msg is not a xml
    SESSION_ISNOT_EXIST_403,    // 403 session is not exist
    ERROR_404, // 404 


    // 分类计数，循环使用，注意保证这个值在最后
    STAT_COUNT,
  };
 private:
  volatile long stat_[24][STAT_COUNT];
  
  size_t begin_time_;
  map<string, int> stat_str_;

 public:
  DeliverStat() {
    ClearCount();
    stat_str_["code:211, comet old connection close"] = RESPONSE_211; //"code:211, comet old connection close"
    stat_str_["ERROR[404]:unknowen url"] = UNKNOWN_URL_404;   // 404 ERROR[404]:unknowen url
    stat_str_["ERROR[403]: please login first"] = LOGIN_FIRST_403; // 403 ERROR[403]: please login first
    stat_str_["ERROR[403]: verify is false"] = VERIFY_FALSE_403; // 403 ERROR[403]: verify is false
    stat_str_["ERROR[403]: userid bind err"] = USERID_BIND_ERROR_403;    // 403 ERROR[403]: userid bind err
    stat_str_["invalid user id"] = INVALID_USERID_403;    // 403 invalid user id
    stat_str_["do comet broadcast , but not login"] = COMET_BROAD_NOT_LOGIN_403;    // 403    do comet broadcast , but not login
    stat_str_["msg is not a xml"] = MSG_ISNOT_XML_403; // 403 msg is not a xml
    stat_str_["session is not exist"] = SESSION_ISNOT_EXIST_403;    // 403 session is not exist
    stat_str_[""] = ERROR_404; // 404 

  }

  void deliverstat(const int stcode, const string &body) {
    if (403 == stcode || 404 == stcode || 211 == stcode) {
      map<string, int>::const_iterator it = stat_str_.find(body);
      if (stat_str_.end() != it) {
        UpCount(it->second);
        return;
      }
    } else if (200 == stcode) {
      UpCount(RESPONSE_200);
      return;
    }

    MCE_INFO("DeliverStat.deliverstat-->status_code=" << stcode << " body=" << body);
    UpCount(NOTSTAT_THIS);

  }

  void ClearCount() {
    begin_time_ = time(NULL);
    for (int t = 0; t < 24; t++) {
      for (int i = 0; i < STAT_COUNT; i++) {
        stat_[t][i] = 0;
      }
    }

  }

  void UpCount(int cate) {
    //int idx = (time(NULL) % (24 * 3600)) / 3600;
    time_t tmp = time(NULL);
    int idx = localtime(&tmp)->tm_hour;
    //MCE_INFO("DeliverStat.UpCount-->idx=" << idx << " cate=" << cate << " sep=" << sep);
    if (idx < 24 && cate < STAT_COUNT) {
      stat_[idx][cate]++;
    }

  }

  vector<long> GetStat() {
    vector<long> st;
    st.push_back(begin_time_);
    for (int t = 0; t < 24; t++) {
      for (int i = 0; i < STAT_COUNT; i++) {
        st.push_back(t);
        st.push_back(i);
        st.push_back((long)stat_[t][i]);
   
      }

    }
    return st;
  }
  

};


class Server : public Singleton<Server> {
public:

	void start(const Ice::PropertiesPtr& props);
	void stop();

//	bool bind(Ice::Long connection, const JidPtr& j, const string& ticket);

//	void unbind(Ice::Long session_id, Ice::Long connection) {
//
//		IceUtil::Mutex::Lock lock(_mutex);
//
//		map<Ice::Long,CometClientPtr>::iterator it = _clients.find(session_id);
//		if (it != _clients.end()) {
//			if (it->second->active_connection == connection) {
//				WTalkProxyAdapter::instance().unbind(boost::lexical_cast<string>(it->second->jid->index),it->second->jid);
//				_clients.erase(session_id);
//			}
//		}
//	}
	int size(){
		return _refactor->connectionPoolSize();
	}
	bool isWap(){
		return _isWap;
	}
	void setWapFlag(bool isWap){
		_isWap = isWap;
	}
//	void deliver(const Long2MsgMap & msgs);
//	void popMsg(Ice::Long session_id, MessageSeq& mseq);

	SessionPtr getSession(Ice::Long connId);
	bool indexExist(Ice::Long index);
	int getServerIndex(){
		return _serverIndex;
	}

  void deliverStat(const int status_code, const string &body);
  vector<long> getDeliverStat() {
     return _deliver_stat.GetStat();
  }
  void clearDeliverStat() {
    _deliver_stat.ClearCount();
  }
private:

	::xce::nio::RefactorPtr _refactor;
	IceUtil::Mutex _mutex;
	bool _isWap;
	int _serverIndex;

  DeliverStat _deliver_stat;

//	//key is cookie:sessionid, 以保证当前cometclient是同一个浏览器会话,同时也是 jid的Resource ID
//	map<Ice::Long,CometClientPtr> _clients;
};
typedef IceUtil::Handle<Server> ServerPtr;

class IdleConnManager : public Timer, public Singleton<IdleConnManager> {
public:
	IdleConnManager() : Timer(1*1000){
		_cometConnSwitch = 0;
		_stoppedReadNothing = 0;
		_stoppedVerify = 0;
		_stoppedNotVerify = 0;
		_stoppedReadSomething = 0;
		_stopedNullComet = 0;
		_stopedOtherConnId = 0;
		_stopedValidComet = 0;
	}
	void handle();
	void push(Ice::Long connId){
		IceUtil::Mutex::Lock lock(_mutex);
		_sessions.push_back(make_pair(connId, ::time(NULL)));
	}
	void cometConnSwitch(){
		IceUtil::Mutex::Lock lock(_mutex);
		_cometConnSwitch++;
	}
	//void connStopped(Ice::Long connId, bool isReadSomthing, bool isVerify);
private:
	const static time_t CONN_TIMEOUT = 5;
	list<pair<Ice::Long, time_t> > _sessions;
	int	_cometConnSwitch;
	int _stoppedReadNothing;
	int _stoppedVerify;
	int _stoppedNotVerify;
	int _stoppedReadSomething; //read something, but not verified

	int _stopedNullComet;
	int _stopedOtherConnId;
	int _stopedValidComet;

	IceUtil::Mutex _mutex;
};


}
;
}
;

#endif /*SERVER_H_*/
