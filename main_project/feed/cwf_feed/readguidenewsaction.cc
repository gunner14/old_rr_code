#include <stdio.h>
#ifdef WIN32
#define snprintf _snprintf
#endif
#include <boost/algorithm/string/predicate.hpp>       // for starts_with
// #include <boost/smart_ptr/scoped_ptr.hpp>

#include "feed/base_feed/common.h"
//#include "feed/base_feed/logging.h"
#include "feed/base_feed/ptime.h"
#include "feed/base_feed/globalinit.h"
#include "feed/base_feed/startuplist.h"

#include "feed/cwf_feed/frame.h"
#include "feed/arch_feed/site_xiaonei.h"

#include "LogWrapper.h"
#include "unistd.h"

#include "feed/rfeed_adapter.h"
#include "feed/feedview/expandfeed.h"
#include "feed/ilikeit_adapter.h"
#include "feed/feedview/headurl.h"
#include "OceCxxAdapter/src/VipIconDataReaderAdapter.h"
#include "FeedStatLogAdapter.h"
#include "feed/cwf_feed/readguidenewsaction.h"
#include "FeedGuideReplicaAdapter.h"

namespace xce {
static const std::string kDefaultContentType("text/html; charset=utf-8");

bool ReadGuideNewsAction::Match(const std::string& url) const {
  //响应删除按钮和全部已读按钮
  // http://guide.renren.com/readguidenews.do
  return boost::starts_with(url, "/readguidenews.do"); 
}

cwf::HttpStatusCode ReadGuideNewsAction::Process(cwf::Request * req, cwf::Response * res) {
  int cwf_pid = getpid();
  MCE_INFO("ReadGuideNewsAction::Process --> pid:" << cwf_pid);
  boost::scoped_ptr<cwf::User> u(Authorize(req));
  if (!u) {
    return cwf::HC_UNAUTHORIZED;
  }
  const string type = req->get("t");
  const long fid = req->getlong("i", 0); 
  
  res->header().set_status_code(cwf::HC_OK, "OK");
  res->header().Add(cwf::HH_CONTENT_TYPE, kDefaultContentType);
  res->OutputHeader();

  {
    try {
      MCE_INFO("ReadGuideNewsAction::Process --> pid:" << cwf_pid << " uid:" << u->id() << " type: " << type << " feedid: " << fid);
      if (type == "s") {
        feed::FeedGuideReplicaAdapter::instance().setReadById(u->id(), fid);
      } else if (type == "a") {
        feed::FeedGuideReplicaAdapter::instance().setReadAll(u->id());
      }
    } catch (Ice::Exception & e) {
      MCE_WARN("ReadGuideNewsAction::Process --> pid:" << cwf_pid << " uid:" << u->id() << " exception:" << e);
    } catch (std::exception & e) {
      MCE_WARN("ReadGuideNewsAction::Process --> pid:" << cwf_pid << " uid:" << u->id() << " exception:" << e.what());
    }
  }

  return cwf::HC_OK;
}

static void Init2() {
  cwf::FrameWork::RegisterAction(new ReadGuideNewsAction);

#ifdef WIN32
  return;
#endif

  try {
    xce::Adapter::feed::ResetCached<feed::FeedItemManagerPrx>("FIM@FeedItem", 20);
  } catch(std::exception & e) {
    MCE_WARN("feednews prx reset error " << e.what());
  }
  try {
    xce::Adapter::minifeed::ResetCached<feed::FeedItemManagerPrx>("FIM@FeedMini", 20);
  } catch(std::exception & e) {
    MCE_WARN("minifeed prx reset error " << e.what());
  }

  HeadUrl::FromPart("");
}

GLOBAL_INIT(CWF_TYPE_ACTION, {
  AddStartup(&Init2);
});

}
