#ifndef _AD_AD_GATE_H_
#define _AD_AD_GATE_H_

#include <ctemplate/template.h>
#include <boost/lexical_cast.hpp>

#include "AdGate.h"
#include "AdEngine.h"
#include "Singleton.h"
#include "ServiceI.h"
#include "Notify/util/fcgi_request.h"
#include "../../ping_client/src/PingSender.h"
#include "AdEngineJAdapter.h"

namespace xce {
namespace ad {

using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace ctemplate;
using namespace xce::notify;

class AdGetRequest : public Request {
public:
  enum RenderType {RT_JSON, RT_HTML};
  AdGetRequest(FCGX_Request* r, RenderType render) : Request(r), render_(render) {}
  virtual bool response();
private:
  RenderType render_;
};

class AdClickRequest : public Request {
public:
  AdClickRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
};

class AdTestRequest : public Request {
public:
  AdTestRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
};

class AdNewGetRequest : public Request {
public:
  enum RenderType {RT_JSON, RT_HTML};
  AdNewGetRequest(FCGX_Request* r, RenderType render) : Request(r), render_(render) {}
  virtual bool response();
private:
  RenderType render_;
};

class AdNewClickRequest : public Request {
public:
  AdNewClickRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
};

class AdGateI : public AdGate, public MyUtil::Singleton<AdGateI> {
public:
  AdGateI();
  virtual bool Reload(const Ice::Current& =  Ice::Current());
  virtual bool ReloadCreativeById(Ice::Long id, const Ice::Current& = Ice::Current());

private:
  //std::string _fcgi_socket;
  //void BodySeq2StrSeq(const NotifyBodySeq & n, Ice::Int view, StrSeq & s);
};

}
}

#endif // _AD_AD_GATE_H_

