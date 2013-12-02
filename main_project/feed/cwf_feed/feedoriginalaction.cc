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

#include "FeedNewsReplicaAdapter.h"
#include "feed/rfeed_adapter.h"
#include "feed/feedview/expandfeed.h"
#include "feed/ilikeit_adapter.h"
#include "feed/feedview/headurl.h"
#include "OceCxxAdapter/src/VipIconDataReaderAdapter.h"
#include "FeedStatLogAdapter.h"
#include "feed/cwf_feed/feedoriginalaction.h"

namespace xce {
static const std::string kDefaultContentType("text/html; charset=utf-8");

bool FeedOriginalAction::Match(const std::string& url) const {
  // http://www.renren.com/feedoriginal.do
  return boost::starts_with(url, "/feedoriginal.do"); 
}

cwf::HttpStatusCode FeedOriginalAction::Process(cwf::Request * req, cwf::Response * res) {
  int cwf_pid = getpid();
  MCE_INFO("FeedOriginalAction::Process --> pid:" << cwf_pid);
  boost::scoped_ptr<cwf::User> u(Authorize(req));
  if (!u) {
    return cwf::HC_UNAUTHORIZED;
  }

  int render_type = WS_HOME;

  const int friend_id = req->get("frid", -1);
  vector<int> friend_ids;
  if (friend_id > 0) {
    friend_ids.push_back(friend_id);
  }
  
  res->header().set_status_code(cwf::HC_OK, "OK");
  res->header().Add(cwf::HH_CONTENT_TYPE, kDefaultContentType);
  res->OutputHeader();

  feed::FeedDataSeq feed_seq;
  feed::FeedDataResPtr feed_res;

  {
    render_type = WS_HOME|WS_HOT|WS_NEWSFEED|WS_FRIENDORIGINAL;
    try {
      int begin = 0;
      if (friend_id == -1) {
        begin = 3;
      }
      feed_res = feed::FeedNewsReplicaAdapter::instance().GetFriendOriginal(u->id(), friend_ids, begin, 40);
    } catch (Ice::Exception & e) {
      MCE_WARN("FeedOriginalAction::Process --> pid:" << cwf_pid << " uid:" << u->id() << " exception:" << e);
      if (friend_id == -1) {
        res->WriteRaw("###DELAY");
      }
      return cwf::HC_OK;
    }
    if (feed_res) {
      feed_seq = feed_res->data;
    }
  }
  MCE_INFO("FeedOriginalAction::Process --> pid:" << cwf_pid << " uid:" << u->id() << " seq: " << feed_seq.size());
  

  int feed_count = 0; // 实际输出的 Feed条目数
  {
    ptime pt("expand", false, 30);
    std::string stat;
    
    const xce::vipinfo::VipFeedDataSeq emptyVipData;

   std::string exposeTime = lexical_cast<std::string>(time(0)); 
   for (size_t i=0,likeidx = 0; i<feed_seq.size(); ++i) {
      std::string output;

      std::string stat_log;
      bool f = false;      

      f = ExpandFeedData(feed_seq[i].get(), u.get(), 0, (WeiredSource)render_type, 0
          , i, emptyVipData, &output, stat_log, exposeTime);
      if (f) {
        ++ feed_count;
        res->WriteRaw(output);
        stat += stat_log;
        stat += "\n";
      }
    }

    try {
        std::vector<std::string> cate;
        cate.push_back("feed");
        cate.push_back("retrieve");
        feed::FeedStatLogAdapter::instance().AddLog(cate, stat);
    } catch (Ice::Exception& e) {
      MCE_WARN("friendoriginalaction.cc::Process --> exception:" << e);
    }
  }

  return cwf::HC_OK;
}

static void Init2() {
  cwf::FrameWork::RegisterAction(new FeedOriginalAction);

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
