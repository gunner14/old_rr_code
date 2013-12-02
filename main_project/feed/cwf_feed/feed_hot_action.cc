
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

#include "LogWrapper.h"

#include "feed/cwf_feed/action.h"
#include "feed/cwf_feed/frame.h"
#include "feed/arch_feed/site_xiaonei.h"

#include "FeedNewsReplicaAdapter.h"
#include "feed/rfeed_adapter.h"
#include "feed/feedview/expandfeed.h"
#include "feed/ilikeit_adapter.h"
#include "feed/feedview/headurl.h"
#include "OceCxxAdapter/src/VipIconDataReaderAdapter.h"
#include "FeedStatLogAdapter.h"

namespace xce {

static const std::string kDefaultContentType("text/html; charset=utf-8");

struct FeedHotAction : public cwf::BaseAction {
  bool Match(const std::string& url) const;
  cwf::HttpStatusCode Process(cwf::Request*, cwf::Response*);
};

bool FeedHotAction::Match(const std::string& url) const {
  return boost::starts_with(url, "/feedhot.do") ||  boost::starts_with(url, "/feedhot2.do");
}

cwf::HttpStatusCode FeedHotAction::Process(cwf::Request * req, cwf::Response * res) {
  int cwf_pid = getpid();
  MCE_INFO("FeedHotAction::Process --> pid:" << cwf_pid);

  boost::scoped_ptr<cwf::User> u(Authorize(req));
  if (!u) {
    return cwf::HC_UNAUTHORIZED;
  }

  const int type = req->form("f", -1);
  const int page = req->get("p", 0);
  const int count = 20;

  // header
  res->header().set_status_code(cwf::HC_OK, "OK");
  res->header().Add(cwf::HH_CONTENT_TYPE, kDefaultContentType);
  res->OutputHeader();

  feed::FeedDataResPtr feed_res;

  try {
    ptime pt("getfeed", false, 30);
    MCE_INFO("FeedHotAction::Process --> pid:" << cwf_pid <<  " uid:" << u->id() << " type:" << type);
    feed::FeedNewsManagerPrx fprx = CachedFeedNews2Adapter(u->id());

    if (!fprx) {
      MCE_INFO("FeedHotAction::Process --> pid:" << cwf_pid << " fprx is NULL!");
      return cwf::HC_SERVICE_UNAVAILABLE;
    }
    

    if (type <= 0) {
      feed_res = fprx->GetHotFeedData(u->id(), page * count, count);
    } else {
      std::vector<int> types;
      if (5 == type) {
        types.push_back(502);
        feed_res = fprx->GetHotFeedDataByStype(u->id(), types, page * count, count);
      } else {
        types.push_back(type * 100);
        feed_res = fprx->GetHotFeedDataByType(u->id(), types, page * count, count);
      }
    }

  } catch (Ice::Exception & e) {
    //LOG_F(WARNING) << "FeedNews2Adapter error : " << e;
    MCE_WARN("FeedHotAction::Process --> pid:" << cwf_pid <<  " uid:" << u->id() << " type:" << type << " exception:" << e);
  }
  MCE_INFO("FeedHotAction::Process --> pid:" << cwf_pid <<  " uid:" << u->id()<< " type:" << type << " seq:" << feed_res->data.size());

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
  MCE_INFO("FeedHotAction::Process --> pid:" << cwf_pid << " type:" << type << " expand start.");

  {
    ptime pt("expand", false, 30);
    std::string stat;
    
    MyUtil::IntSeq uids, vipicontypes;
    vipicontypes.push_back(0); //目前只传0，具体有哪些类型，见OceCxxAdapter/src/VipIconDataReaderAdapter.h中的VIP_TYPE枚举类型
    for (size_t i = 0; i<feed_res->data.size(); ++i) {
      uids.push_back(feed_res->data[i].get()->actor);
    }
    vipinfo::VipFeedDataSeqMap vipInfoMap;
    const vipinfo::VipFeedDataSeq emptyVipData;
    try {
      vipInfoMap = adapter::vipicondata::VipIconDataReaderAdapter::instance().getVipFeedDataSeqMap(uids, vipicontypes);
    } catch (Ice::Exception& e) {
      MCE_WARN("FeedHotAction::Process --> pid:" << cwf_pid <<" getVipIconData exception:"<< e);
    } catch (std::exception& e) {
      MCE_WARN("FeedHotAction::Process --> pid:" << cwf_pid <<" getVipIconData exception:"<< e.what());
    }
   
    std::string exposeTime = lexical_cast<std::string>(time(0)); 
    for (size_t i=0,likeidx = 0; i<feed_res->data.size(); ++i) {
      std::string output;

      const ilikeit::LikeInfo* likeinfo = 0;
      if (likeinfo_avaible && vbool[i])
        likeinfo = likeseq[likeidx++].get();
      else
        likeinfo = 0;

      std::string stat_log;
      bool f = false;

      vipinfo::VipFeedDataSeqMap::const_iterator it = vipInfoMap.find(feed_res->data[i].get()->actor);
      if(it == vipInfoMap.end() || (it->second).empty()) {
        f = ExpandFeedData(feed_res->data[i].get(), u.get(), 0, (WeiredSource)(WS_HOME | WS_HOT), likeinfo
          , page * count + i, emptyVipData, &output, stat_log, exposeTime);
      }
      else {
        f = ExpandFeedData(feed_res->data[i].get(), u.get(), 0, (WeiredSource)(WS_HOME | WS_HOT), likeinfo
          , page * count + i, it->second, &output, stat_log, exposeTime);
      }

      //f = ExpandFeedData(feed_res->data[i].get(), u.get(), 0, WeiredSource(WS_HOME | WS_HOT), likeinfo
      //  , page * count + i, emptyVipIconDataSeq, &output, stat_log);

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
        //LOG(INFO) << "feed_hot_action.cc::Process--> FeedStatLog prx is NULL !";
        //MCE_WARN("FeedHotAction::Process --> pid:" << cwf_pid << " type:" << type << " FeedStatLog prx is NULL !");
      //} else {
        std::vector<std::string> cate;
        cate.push_back("feed");
        //cate.push_back("stat");
        cate.push_back("hot");
        //prx->AddLog(cate, stat);
        feed::FeedStatLogAdapter::instance().AddLog(cate, stat);
      //}
    } catch (Ice::Exception& e) {
      MCE_WARN(e);
    }
  }

  MCE_INFO("FeedHotAction::Process --> pid:" << cwf_pid << " type:" << type << " p: " << page << " seq: " << feed_res->data.size()
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

static void InitHotAction() {
  // LOG(INFO) << "Register FeedHotAction";
  cwf::FrameWork::RegisterAction(new FeedHotAction);

#ifdef WIN32
  return;
#endif

  try {
    xce::Adapter::feed::ResetCached<feed::FeedNewsManagerPrx>("FIM@FeedNews", 20);
  } catch(std::exception & e) {
    MCE_WARN("feednews prx reset error " << e.what());
  }

  HeadUrl::FromPart("");
}

GLOBAL_INIT(CWF_TYPE_ACTION, {
  AddStartup(&InitHotAction);
});

}
