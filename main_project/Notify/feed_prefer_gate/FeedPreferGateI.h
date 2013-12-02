#ifndef _NOTIFY_CGI_H_
#define _NOTIFY_CGI_H_

#include <ctemplate/template.h>
#include <boost/lexical_cast.hpp>

#include "FeedPreferGate.h"
#include "Notify.h"
#include "RFeed.h"
#include "Singleton.h"
#include "ServiceI.h"
#include "Notify/util/fcgi_request.h"
#include "pugixml.hpp"

namespace xce {
namespace notify {

using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace ctemplate;

const string NOTIFY_FCGI_HOST = "notify.renren.com";
  
//class FeedGetRequest : public Request{
//public:
//  FeedGetRequest(FCGX_Request* r) : Request(r) {}
//
//  virtual bool response();
//};

class FeedPreferGateI : public FeedPreferGate, public MyUtil::Singleton<FeedPreferGateI> {
public:
	FeedPreferGateI();
};



}
}

#endif // _FEED_GATEI_H_

