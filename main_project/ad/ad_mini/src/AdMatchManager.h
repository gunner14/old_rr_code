/*
 * AdMatchManger.h
 *
 *  Created on: 2012-4-26
 *      Author: ping.zhang
 */

#ifndef ADMATCHMANGER_H_
#define ADMATCHMANGER_H_

#include <ctemplate/template.h>
#include "Notify/util/fcgi_request.h"
#include "AdEngine.h"
#include "AdMini.h"
namespace xce {
namespace ad {
using namespace ctemplate;
using namespace MyUtil;
using namespace std;
using namespace xce::notify;

extern std::string db_instance;

class AdMatchManagerI: public AdMini, public Singleton<AdMatchManagerI> {
public:
  AdMatchManagerI();
  void GetAdsByRefer(string &refer, xce::ad::AdZone2Creatives &ads);
  void Init();
  void InitOk(){
    init_ok_ = true;
  }
  private:
  bool init_ok_;
};

class AdGetRequest : public Request {
public:
  AdGetRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
};

class AdClickRequest : public Request {
public:
  AdClickRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
};

class AdGateSRequest : public Request {
public:
  AdGateSRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
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

}
}

#endif /* ADMATCHMANGER_H_ */
