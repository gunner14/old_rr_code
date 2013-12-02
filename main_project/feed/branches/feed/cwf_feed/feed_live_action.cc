#include <stdio.h>
#ifdef WIN32
#define snprintf _snprintf
#endif
#include <boost/algorithm/string/predicate.hpp>       // for starts_with
// #include <boost/smart_ptr/scoped_ptr.hpp>

#include "feed/base_feed/common.h"
#include "feed/base_feed/logging.h"
#include "feed/base_feed/ptime.h"
#include "feed/base_feed/globalinit.h"
#include "feed/base_feed/startuplist.h"

#include "feed/cwf_feed/action.h"
#include "feed/cwf_feed/frame.h"
#include "feed/arch_feed/site_xiaonei.h"

#include "FeedNewsReplicaAdapter.h"
#include "FeedMiniReplicaAdapter.h"
#include "feed/rfeed_adapter.h"
#include "feed/feedview/expandfeed.h"
#include "feed/ilikeit_adapter.h"
#include "feed/feedview/headurl.h"
#include "OceCxxAdapter/src/VipIconDataTripodAdapter.h"
#include "FeedStatLogAdapter.h"

namespace xce {

struct FeedLiveAction : public cwf::BaseAction {
  bool Match(const std::string& url) const;
  cwf::HttpStatusCode Process(cwf::Request*, cwf::Response*);
};

bool FeedLiveAction::Match(const std::string& url) const {
  // http://www.renren.com/feedretrieve.do
  return boost::starts_with(url, "/feedlive.do") || boost::starts_with(url, "/feedlive2.do");
}

cwf::HttpStatusCode FeedLiveAction::Process(cwf::Request * req, cwf::Response * res) {
  int cwf_pid = getpid();
  MCE_INFO("FeedLiveAction::Process --> pid:" << cwf_pid);

  boost::scoped_ptr<cwf::User> u(Authorize(req));
  if (!u) {
    return cwf::HC_UNAUTHORIZED;
  }

  const int type = req->form("f", -1);
  const int page = req->get("p", 0);
  const int count = 20;

  res->header().set_status_code(cwf::HC_OK, "OK");
  res->header().Add(cwf::HH_CONTENT_TYPE, "text/html; charset=utf-8");
  res->OutputHeader();

  feed::FeedDataResPtr feed_res;

  try {
    ptime pt("getfeed", false, 30);
    MCE_INFO("FeedLiveAction::Process --> pid:" << cwf_pid << " type:" << type);
    feed::FeedNewsManagerPrx fprx = CachedFeedNews2Adapter(u->id());

    if (!fprx) {
      MCE_INFO("FeedLiveAction::Process --> pid:" << cwf_pid << " fprx is NULL!");
      return cwf::HC_SERVICE_UNAVAILABLE;
    }

    if (-1 == type) {
      feed_res = fprx->GetLiveFeedData(u->id(), page * count, count);
    } else {
      std::vector<int> types;
      if (5 == type) {
        types.push_back(502);
        feed_res = fprx->GetLiveFeedDataByStype(u->id(), types, page * count, count);
      } else {
        types.push_back(type * 100);
        feed_res = fprx->GetLiveFeedDataByType(u->id(), types, page * count, count);
      }
    }


  } catch (Ice::Exception & e) {
    LOG_F(WARNING) << e;
    return cwf::HC_SERVICE_UNAVAILABLE;
  }
  MCE_INFO("FeedLiveAction::Process --> pid:" << cwf_pid <<  " uid:" << u->id()<< " type:" << type << " seq:" << feed_res->data.size());

  if (!feed_res) {
     return cwf::HC_SERVICE_UNAVAILABLE;
   }

  int feed_count = 0; // 实际输出的 Feed条目数

  // 得到“赞” ilike 信息
  ::xce::ilikeit::LikeInfoSeq likeseq;
  std::vector<char> vbool(0, feed_res->data.size()); // 有些类型并不需要 zan
  bool likeinfo_avaible = false;
#if 1//POSIX
  {
    ptime pt("buildzan", false, 30);
    ::xce::ilikeit::GlobalIdSeq globalseq;
    for (size_t i=0; i<feed_res->data.size(); ++i) {
      feed::FeedData* feed = feed_res->data[i].get();
      if (BuildGlobalId(feed->source, feed->type, &globalseq)) {
        vbool.push_back(true);
      } else {
        vbool.push_back(false);
      }
    }
    
    if (!globalseq.empty()) {
      // try catch inside
      likeinfo_avaible = BuildZan(u->id(), globalseq, &likeseq);
    }
  }
#endif
  MCE_INFO("FeedLiveAction::Process --> pid:" << cwf_pid <<  " uid:" << u->id() << " expand start.");

  {
    ptime pt("expand", false, 30);
    std::string stat;
    
    MyUtil::IntSeq uids, vipicontypes;
    vipicontypes.push_back(0); //目前只传0，具体有哪些类型，见OceCxxAdapter/src/VipIconDataTripodAdapter.h中的VIP_TYPE枚举类型
    for (size_t i = 0; i<feed_res->data.size(); ++i) {
      uids.push_back(feed_res->data[i].get()->actor);
    }
    vipinfo::VipFeedDataSeqMap vipInfoMap;
    const vipinfo::VipFeedDataSeq emptyVipData;
    try {
      vipInfoMap = xce::adapter::vipicondata::VipIconDataTripodAdapter::instance().GetVipFeedDataSeqMap(uids, vipicontypes);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedLiveAction::Process --> pid:" << cwf_pid <<" getVipFeedData exception:"<< e);
    } catch (std::exception& e) {
      MCE_WARN("FeedLiveAction::Process --> pid:" << cwf_pid <<" getVipFeedData exception:"<< e.what());
    }

    for (size_t i=0,likeidx = 0; i<feed_res->data.size(); ++i) {
      std::string output;

      const ilikeit::LikeInfo* likeinfo = 0;
      if (likeinfo_avaible && vbool[i]) {
        likeinfo = likeseq[likeidx++].get();
      } else {
        likeinfo = 0;
      }
      std::string stat_log;
      bool f = false;

      vipinfo::VipFeedDataSeqMap::const_iterator it = vipInfoMap.find(feed_res->data[i].get()->actor);
      if(it == vipInfoMap.end() || (it->second).empty()) {
        f = ExpandFeedData(feed_res->data[i].get(), u.get(), 0, (WeiredSource)(WS_HOME | WS_LIVE), likeinfo
          , page * count + i, emptyVipData, &output, stat_log);
      }
      else {
        f = ExpandFeedData(feed_res->data[i].get(), u.get(), 0, (WeiredSource)(WS_HOME | WS_LIVE), likeinfo
          , page * count + i, it->second, &output, stat_log);
      }

      //f = ExpandFeedData(feed_res->data[i].get(), u.get(), 0, WeiredSource(WS_HOME | WS_LIVE), likeinfo
      //  , page * count + i, emptyVipIconDataSeq, &output, stat_log) ;
      if (f) {
        ++ feed_count;
        res->WriteRaw(output);
        stat += stat_log;
        stat += "\n";
      }
    }

    try {
      //xce::feed::FeedStatLogPrx prx = CachedFeedStatLogAdapter(u->id());
      //LOG(INFO) << "prx:" << prx;
      //if (!prx) {
        //MCE_WARN("FeedLiveAction::Process --> pid:" << cwf_pid <<  " uid:" << u->id() << " FeedStatLog prx is NULL !");
      //} else {
        std::vector<std::string> cate;
        cate.push_back("feed");
       // cate.push_back("stat");
        cate.push_back("live");
        //prx->AddLog(cate, stat);
        feed::FeedStatLogAdapter::instance().AddLog(cate, stat);
      //}
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedLiveAction::Process --> pid:" << cwf_pid << " " << e);
    }


  }

  MCE_WARN("FeedLiveAction::Process --> pid:" << cwf_pid <<  " uid:" << u->id()
      << " p:" << page << " seq:" << feed_res->data.size()
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

static void InitLiveAction() {
  MCE_INFO("Register FeedLiveAction");
  cwf::FrameWork::RegisterAction(new FeedLiveAction);

#ifdef WIN32
  return;
#endif

  try {
    xce::Adapter::feed::ResetCached<feed::FeedNewsManagerPrx>("M@FeedNews", 20);
  } catch(std::exception & e) {
    MCE_WARN("feednews prx reset error " << e.what());
  }

  HeadUrl::FromPart("");
}

GLOBAL_INIT(CWF_TYPE_ACTION, {
  AddStartup(&InitLiveAction);
});

}
