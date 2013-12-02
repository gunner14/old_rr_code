#include "feed/cwf_feed/feedmarkaction.h"

#include <stdio.h>
#ifdef WIN32
#define snprintf _snprintf
#endif
#include <boost/algorithm/string/predicate.hpp>       // for starts_with
#include <sstream>
#include "feed/base_feed/common.h"
#include "feed/base_feed/logging.h"
#include "feed/base_feed/ptime.h"
#include "feed/base_feed/globalinit.h"
#include "feed/base_feed/startuplist.h"

#include "feed/cwf_feed/frame.h"
#include "feed/arch_feed/site_xiaonei.h"

#include "IceLogger.h"
#include "feed/rfeed_adapter.h"
#include "feed/feedview/headurl.h"
#include "FeedAssistantAdapter.h"

namespace xce {

static const std::string kDefaultContentType2;

bool FeedMarkAction::Match(const std::string& url) const {
  // http://www.renren.com/feedmark.do
  return boost::starts_with(url, "/feedmark.do");
}

cwf::HttpStatusCode FeedMarkAction::Process(cwf::Request * req, cwf::Response * res) {
  int cwf_pid = getpid();

  boost::scoped_ptr<cwf::User> u(Authorize(req));
  if (!u) {
    return cwf::HC_UNAUTHORIZED;
  }

  MCE_INFO("FeedMarkAction::Process --> pid:" << cwf_pid <<  "uid:" << u->id());
  bool mark = req->get("mark",0);
  long fid = req->getlong("fid",0);
  long source = req->getlong("source",0);
  int stype = req->get("stype",0);
  int actor = req->get("actor",0);
  res->header().set_status_code(cwf::HC_OK, "OK");
  res->header().Add(cwf::HH_CONTENT_TYPE, kDefaultContentType2);
  res->OutputHeader();
  
  MCE_INFO(" fid: " << fid << " source: " << source << " stype: " <<stype <<" actor: " << actor << " mark: " << mark);
  try {
    // 调用接口
    feed::FeedAssistantAdapter::instance().MarkFeed(u->id(), fid, source, stype, actor, mark);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedMarkAction::Process --> pid:" << cwf_pid <<  "uid:" << u->id() << " exception:" << e);
  } catch (std::exception& e) {
    MCE_WARN("FeedMarkAction::Process --> pid:" << cwf_pid <<  "uid:" << u->id() << " exception:" << e.what());
  }
  
  return cwf::HC_OK;
}

static void InitFeedMarkAction() {
  cwf::FrameWork::RegisterAction(new FeedMarkAction);

#ifdef WIN32
  return;
#endif
}

GLOBAL_INIT(CWF_TYPE_ACTION, {
  AddStartup(&InitFeedMarkAction);
});

}
