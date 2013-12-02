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

#include "FeedGuideReplicaAdapter.h"
#include "feed/rfeed_adapter.h"
#include "feed/feedview/expandfeed.h"
#include "feed/ilikeit_adapter.h"
#include "feed/feedview/headurl.h"
#include "OceCxxAdapter/src/VipIconDataReaderAdapter.h"
#include "FeedStatLogAdapter.h"
#include "feed/cwf_feed/feedguideaction.h"
#include "FeedNewsReplicaAdapter.h"
#include "feed/cwf_feed/expand_user_feeds_cwf.h"

namespace xce {
static const std::string kDefaultContentType("text/html; charset=utf-8");

bool FeedGuideAction::Match(const std::string& url) const {
  // http://guide.renren.com/feedguide.do
  return boost::starts_with(url, "/feedguide.do"); 
}

cwf::HttpStatusCode FeedGuideAction::Process(cwf::Request * req, cwf::Response * res) {
  int cwf_pid = getpid();
  MCE_INFO("FeedGuideAction::Process --> pid:" << cwf_pid);
  boost::scoped_ptr<cwf::User> u(Authorize(req));
  if (!u) {
    return cwf::HC_UNAUTHORIZED;
  }

  const int type = req->form("f", -1); //type == -1 是“全部”的访问
  int render_type = WS_HOME;

  const int page = req->get("p", 0);
  const int count = 20;
  // std::string host = req->query("host");

  // header
  
  res->header().set_status_code(cwf::HC_OK, "OK");
  res->header().Add(cwf::HH_CONTENT_TYPE, kDefaultContentType);
  res->OutputHeader();

  feed::FeedDataSeq feed_seq;
  feed::FeedDataResPtr feed_res;

  if (u->id() % 10 == 6) {
    render_type = WS_HOME|WS_HOT|WS_NEWSFEED|WS_GUIDEFEED;
    try {
      MCE_INFO("FeedGuideAction::Process --> pid:" << cwf_pid << " type:" << type << " uid:" << u->id());
      if (type <= 0) {
         feed_res = feed::FeedGuideReplicaAdapter::instance().GetGuideFeedDataByStype(u->id(), std::vector<int>(), page * count, count);
         MCE_INFO("ByType stype empty");
      } else {
        std::vector<int> types;
        types.push_back(type * 100);
        feed_res = xce::feed::FeedGuideReplicaAdapter::instance().GetGuideFeedDataByStype(u->id(), types, page * count, count);
        for(size_t i = 0; i < types.size(); ++i) {
          MCE_INFO("ByType stype " << i << " = " << types[i]);
        }
      }
    } catch (Ice::Exception & e) {
      MCE_WARN("FeedGuideAction::Process --> pid:" << cwf_pid << " type:" << type << " uid:" << u->id() << " exception:" << e);
    }
    if (feed_res) {
      feed_seq = feed_res->data;
    }
  } else {
    render_type = WS_HOME|WS_HOT|WS_NEWSFEED;
    try {
      MCE_INFO("FeedGuideAction::Process --> pid:" << cwf_pid << " type:" << type << " uid:" << u->id());
      if (type <= 0) {
        //feed_res = feed::FeedNewsReplicaAdapter::instance().GetHotFeedDataByType(u->id(), std::vector<int>(), page * count, count);
        feed_res = feed::FeedNewsReplicaAdapter::instance().GetHotFeedDataByType(u->id(), std::vector<int>(), page * count, count);
        MCE_INFO("ByType stype empty");
      } else {
        std::vector<int> types;
        types.push_back(type * 100);
        feed_res = feed::FeedNewsReplicaAdapter::instance().GetHotFeedDataByType(u->id(), types, page * count, count);
        for(size_t i = 0; i < types.size(); ++i) {
          MCE_INFO("ByType stype " << i << " = " << types[i]);
        }
      }
    } catch (Ice::Exception & e) {
      MCE_WARN("FeedGuideAction::Process --> pid:" << cwf_pid << " type:" << type << " uid:" << u->id() << " exception:" << e);
    }
    if (feed_res) {
      feed_seq = feed_res->data;
    }
  }
  MCE_INFO("FeedGuideAction::Process --> pid:" << cwf_pid << " type:" << type << " uid:" << u->id() << " seq: " << feed_seq.size());
  
  int feed_count = 0; // 实际输出的 Feed条目数
  std::string stat;
  const std::string& username = u->name();
  feed_count = ExpandUserFeedsCwf(0, u->id(), username, render_type, feed_seq, page * count, res, stat);
  {
/*  // 得到“赞” ilike 信息
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
  MCE_INFO("FeedGuideAction::Process --> pid:" << cwf_pid << " type:" << type << " uid:" << u->id() << " expand start.");

  {
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
      MCE_WARN("FeedGuideAction::Process --> pid:" << cwf_pid <<" getVipFeedData exception:"<< e);
    } catch(std::exception& e) {
      MCE_WARN("FeedGuideAction::Process --> pid:" << cwf_pid <<" getVipFeedData exception:"<< e.what());
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
          , page * count + i, emptyVipData, &output, stat_log, exposeTime);
      }
      else {
        f = ExpandFeedData(feed_seq[i].get(), u.get(), 0, (WeiredSource)render_type, likeinfo
          , page * count + i, it->second, &output, stat_log, exposeTime);
      }

      //f = ExpandFeedData(feed_seq[i].get(), u.get(), 0, (WeiredSource)render_type, likeinfo
      //  , page * count + i, emptyVipIconDataSeq, &output, stat_log);
      if (f) {
        ++ feed_count;
        res->WriteRaw(output);
        stat += stat_log;
        stat += "\n";
      }
    }
*/
    try {
        std::vector<std::string> cate;
        cate.push_back("feed");
        //cate.push_back("stat");
        cate.push_back("retrieve");
        //prx->AddLog(cate, stat);
        feed::FeedStatLogAdapter::instance().AddLog(cate, stat);
      //}
    } catch (Ice::Exception& e) {
      MCE_WARN("feedguideaction.cc::Process --> exception:" << e);
    }

  }

  MCE_INFO("FeedGuideAction::Process --> pid:" << cwf_pid << " type:" << type << " uid:" << u->id() 
      << "p: " << page << " seq: " << feed_seq.size()
      << " expand: " << feed_count);

  // 最后添加一个如下字符串，为js服务，否则不会添加进列表
  // 第一页 ##@L#1##@L#20##@L#20
  // 第二页 ##@L#21##@L#40##@L#20
  char buf[100];
  snprintf(buf, sizeof(buf), "##@L#%d##@L#%d##@L#%d"
    , 1 + page * count, page * count + feed_count, feed_count);
  res->WriteRaw(buf);
  return cwf::HC_OK;
}

static void Init2() {
  cwf::FrameWork::RegisterAction(new FeedGuideAction);

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
