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
#include "feed/cwf_feed/cwf_statLog.h"
#include "FeedStatLogAdapter.h"
#include "feed/cwf_feed/retrieveguidefeedidaction.h"
#include "FeedGuideReplicaAdapter.h"

namespace xce {
static const std::string kDefaultContentType("text/html; charset=utf-8");

bool RetrieveGuideFeedIdAction::Match(const std::string& url) const {
  // 响应扶贫项目新鲜事折叠展开请求
  // http://guide.renren.com/retrieveguidefeedid.do
  return boost::starts_with(url, "/retrieveguidefeedid.do"); 
}

cwf::HttpStatusCode RetrieveGuideFeedIdAction::Process(cwf::Request * req, cwf::Response * res) {
  int cwf_pid = getpid();
  MCE_INFO("RetrieveGuideFeedIdAction::Process --> pid:" << cwf_pid);
  boost::scoped_ptr<cwf::User> u(Authorize(req));
  if (!u) {
    return cwf::HC_UNAUTHORIZED;
  }

  int render_type = WS_HOME;

  const string fid = req->get("id");
  vector<long> fids;
  {
    string friendid = fid + ",";
    long frid = 0;
    for(int i = 0;i < friendid.size();++ i) {
      if(friendid[i] != ',') {
        frid = friendid[i] - '0' + frid * 10;
      } else {
        fids.push_back(frid);
        frid = 0;
      }
    }
  }

  res->header().set_status_code(cwf::HC_OK, "OK");
  res->header().Add(cwf::HH_CONTENT_TYPE, kDefaultContentType);
  res->OutputHeader();

  feed::FeedDataSeq feed_seq;
  feed::FeedDataResPtr feed_res;

  {
    render_type = WS_HOME|WS_HOT|WS_NEWSFEED|WS_GUIDEFEED;
    try {
      MCE_INFO("RetrieveGuideFeedIdAction::Process --> pid:" << cwf_pid << " uid:" << u->id() << " fid: " << fid );
      feed_seq = feed::FeedGuideReplicaAdapter::instance().GetFeedDataByIds(fids);
    } catch (Ice::Exception & e) {
      MCE_WARN("RetrieveGuideFeedIdAction::Process --> pid:" << cwf_pid << " uid:" << u->id() << " exception:" << e);
    }
  }
  MCE_INFO("RetrieveGuideFeedIdAction::Process --> pid:" << cwf_pid << " uid:" << u->id() << " seq: " << feed_seq.size());
  //增加weight标记
  for(int i = 0;i < feed_seq.size(); ++ i) {
    feed_seq[i]->weight = 0x1;
  }
  int feed_count = 0; // 实际输出的 Feed条目数

  // 得到“赞” ilike 信息
  ::xce::ilikeit::LikeInfoSeq likeseq;
  std::vector<char> vbool(0, feed_seq.size()); // 有些类型并不需要 zan
  bool likeinfo_avaible = false;
#if 1//POSIX
  {
    ptime pt("buildzan", false, 30);
    ::xce::ilikeit::GlobalIdSeq globalseq;
    for (size_t i=0; i<feed_seq.size(); ++i) {
      feed::FeedData* feed = feed_seq[i].get();

      if (BuildGlobalId(feed->source, feed->type, &globalseq))
        vbool.push_back(true);
      else
        vbool.push_back(false);
    }
    
    if (!globalseq.empty()) {
      // try catch inside
      likeinfo_avaible = BuildZan(u->id(), globalseq, &likeseq);
    }
  }
#endif
  MCE_INFO("RetrieveGuideFeedIdAction::Process --> pid:" << cwf_pid << " fid:" << fid << " uid:" << u->id() << " expand start.");

  {
    ptime pt("expand", false, 30);
    std::string stat;
    
    MyUtil::IntSeq uids, vipicontypes;
    vipicontypes.push_back(0); //目前只传0，具体有哪些类型，见OceCxxAdapter/src/VipIconDataReaderAdapter.h中的VIP_TYPE枚举类型
    for (size_t i = 0; i<feed_seq.size(); ++i) {
      uids.push_back(feed_seq[i].get()->actor);
    }
    vipinfo::VipFeedDataSeqMap vipInfoMap;
    const xce::vipinfo::VipFeedDataSeq emptyVipData;
    try {
      vipInfoMap = adapter::vipicondata::VipIconDataReaderAdapter::instance().getVipFeedDataSeqMap(uids, vipicontypes);
    } catch (Ice::Exception& e) {
      MCE_WARN("RetrieveGuideFeedIdAction::Process --> pid:" << cwf_pid <<" getVipFeedData exception:"<< e);
    } catch(std::exception& e) {
      MCE_WARN("RetrieveGuideFeedIdAction::Process --> pid:" << cwf_pid <<" getVipFeedData exception:"<< e.what());
    }

   std::string exposeTime = lexical_cast<std::string>(time(0)); 
   for (size_t i=0,likeidx = 0; i<feed_seq.size(); ++i) {
      std::string output;

      const ilikeit::LikeInfo* likeinfo = 0;
      if (likeinfo_avaible && vbool[i])
        likeinfo = likeseq[likeidx++].get();
      else
        likeinfo = 0;

      std::string stat_log;
      bool f = false;      

      vipinfo::VipFeedDataSeqMap::const_iterator it = vipInfoMap.find(feed_seq[i].get()->actor);
      if(it == vipInfoMap.end() || (it->second).empty()) {
        f = ExpandFeedData(feed_seq[i].get(), u.get(), 0, (WeiredSource)render_type, likeinfo
          , i, emptyVipData, &output, stat_log, exposeTime);
      }
      else {
        f = ExpandFeedData(feed_seq[i].get(), u.get(), 0, (WeiredSource)render_type, likeinfo
          , i, it->second, &output, stat_log, exposeTime);
      }
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
        //cate.push_back("stat");
        cate.push_back("retrieve");
        //prx->AddLog(cate, stat);
        //feed::FeedStatLogAdapter::instance().AddLog(cate, stat);
        CwfStatLog::instance().AddLog(cate, stat);
      //}
    } catch (Ice::Exception& e) {
      MCE_WARN("retrieveguidefeedid.cc::Process --> exception:" << e);
    }
  }

  MCE_INFO("RetrieveGuideFeedIdAction::Process --> pid:" << cwf_pid << " feedid:" << fid << " uid:" << u->id() << " seq: " << feed_seq.size() << " expand: " << feed_count);

  return cwf::HC_OK;
}

static void Init2() {
  cwf::FrameWork::RegisterAction(new RetrieveGuideFeedIdAction);

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
