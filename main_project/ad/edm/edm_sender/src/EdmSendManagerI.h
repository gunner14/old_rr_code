/*
 * EdmSendManagerI.h
 *
 *  Created on: 2012-2-16
 *      Author: sunzz
 */

#ifndef EDMSENDMANAGERI_H_
#define EDMSENDMANAGERI_H_

#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include "TaskManager.h"
#include "Singleton.h"
#include "AdEdmSender.h"
#include <vector>
#include "MatchHandler.h"
#include "Notify/util/fcgi_request.h"
namespace xce {
namespace ad {

using namespace MyUtil;
using namespace std;
using namespace xce::notify;

class AdEdmSenderI: public AdEdmSender, public MyUtil::Singleton<AdEdmSenderI> {
public:
  AdEdmSenderI();
  virtual void Expose(Ice::Int user_id, Ice::Long adgroup_id, Ice::Int index, const Ice::Current& = Ice::Current());
  virtual void SendEdm(Ice::Int user_id, const MyUtil::Str2StrMap& params ,const Ice::Current& = Ice::Current());
  virtual void SendEdmTest(Ice::Int user_id, const MyUtil::Str2StrMap& params, const Ice::Current& = Ice::Current());
  virtual void ClearLoginCacheByUser(Ice::Int user_id, const Ice::Current& = Ice::Current());
	//add for test
	 virtual groups GetCandidateGroups(Ice::Int userid,const Ice::Current& = Ice::Current());

  void Init();
  void InitOk(){
    init_ok_ = true;
  }
private:
  void GetUserInfo(int user_id, UserProfile &profile, const MyUtil::Str2StrMap& params);
  bool init_ok_;
  vector<EdmMatchHandlerPtr> edm_match_handler_list_;
};

class InitTask : virtual public MyUtil::Task{
public:
  InitTask(){}
  virtual void handle();
};

class ReloadTask : virtual public MyUtil::Task{
public:
  ReloadTask(){}
  virtual void handle();
};

class EdmClickRequest : public Request{
  public:
    EdmClickRequest(FCGX_Request* r) : Request(r) {}
    virtual bool response();
    static int ProcessLogStr(string& source, string& edm_log, string& left_click_log, string& left_cpc_charge);
  private:
    void ProcessLeftClickLog(string& log);
    void ProcessLeftChargeLog(string& log);
};

class OtherRequest : public Request{
  public:
    OtherRequest(FCGX_Request* r) : Request(r) {}
    virtual bool response();
};
}
}



#endif /* EDMSENDMANAGERI_H_ */
