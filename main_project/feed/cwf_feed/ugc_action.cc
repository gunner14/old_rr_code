#include "feed/cwf_feed/ugc_action.h"

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
#include "FeedMiniReplicaAdapter.h"


#include "feed/rfeed_adapter.h"
#include "feed/feedview/expandfeed.h"
#include "feed/ilikeit_adapter.h"
#include "feed/feedview/headurl.h"
#include "OceCxxAdapter/src/VipIconDataReaderAdapter.h"
#include "FeedStatLogAdapter.h"
#include "feed/cwf_feed/expand_user_feeds_cwf.h"
//XAR_IMPL(exGT30); // expand > 30 ms
//XAR_IMPL(gfGT30);
//XAR_IMPL(likeGT30);

namespace xce {
static const std::string kDefaultContentType("text/html; charset=utf-8");

bool UgcFeedAction::Match(const std::string& url) const {
  // http://www.renren.com/feedretrieve.do
  return boost::starts_with(url, "/getugcfeed.do");
}

const int ugc_type_arr[] = {101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
                            115, 116, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
                            133, 134, 135, 136, 210, 211, 501, 502, 601, 701, 702, 708, 709, 801,
                            901, 902, 903, 904, 3801, 3802, 3803, 3804,  8605, 8607, 8609, 8610,
                            8901, 8902, 8904, 9101, 9102, 9103, 9104, 9105, 9106, 9107, 9108,
                            9301, 9302, 9303, 9304, 9602, 9603, 9604 };
const std::vector<int> types(ugc_type_arr, ugc_type_arr + sizeof(ugc_type_arr) / sizeof(int) );

cwf::HttpStatusCode UgcFeedAction::Process(cwf::Request * req, cwf::Response * res) {
  int cwf_pid = getpid();
  MCE_INFO("UgcFeedAction::Process --> pid:" << cwf_pid << " types:" << types.size());

  boost::scoped_ptr<cwf::User> u(Authorize(req));
  if (!u) {
    return cwf::HC_UNAUTHORIZED;
  }

  int render_type = WS_HOME;

  const int page = req->get("p", 0);
  const int count = 20;
  // std::string host = req->query("host");

  // header
  res->header().set_status_code(cwf::HC_OK, "OK");
  res->header().Add(cwf::HH_CONTENT_TYPE, kDefaultContentType);
// Cache-Control	no-cache
// Pragma	no-cache
// Expires	Thu, 01 Jan 1970 00:00:00 GMT
  // res->header().Add(cwf::HH_CACHE_CONTROL, "no-cache");
  // config in nginx?
  res->OutputHeader();

  feed::FeedDataSeq feed_seq;
  feed::FeedDataResPtr feed_res;

  //if (u->id() % 10 == 2 || u->id() % 10 == 1 || u->id() % 10 == 9 || u->id() % 10 == 3|| u->id() % 10 == 4|| u->id() % 10 == 0) 
  {
    render_type = WS_HOME|WS_LIVE;
    try {
      //feed::FeedNewsManagerPrx fprx = CachedFeedNews3Adapter(u->id());
      MCE_INFO("UgcFeedAction::Process --> pid:" << cwf_pid << " uid:" << u->id());
      //feed::FeedNewsManagerPrx fprx = CachedFeedNews2Adapter(u->id());
      //if (!fprx) {
        //MCE_INFO("UgcFeedAction::Process --> pid:" << cwf_pid << " fprx is NULL!");
        //return cwf::HC_SERVICE_UNAVAILABLE;
      //}
      //feed_seq = FeedNewsReplicaAdapter::instance().GetHotFeedDataByType(uid, types, from, count);

      //feed_res = fprx->GetLiveFeedDataByStype(u->id(), types, page * count, count);
        feed_res = xce::feed::FeedNewsReplicaAdapter::instance().getLiveFeedDataByStype(u->id(), types, page * count, count);
    } catch (Ice::Exception & e) {
      //LOG_F(WARNING) << "feed retrive as hot error. uid=" << u->id() << " : " << e;
      MCE_WARN("UgcFeedAction::Process --> pid:" << cwf_pid << " uid:" << u->id() << " exception:" << e);
      // return cwf::HC_SERVICE_UNAVAILABLE;
    }
    if (feed_res) {
      feed::FeedDataSeq & feed_temp = feed_res->data;
      for(size_t i = 0;i < feed_temp.size(); ++i) {
        if(feed_temp[i]->actor != u->id()) {
          feed_seq.push_back(feed_temp[i]);
        }
      }
    }
  }
  MCE_INFO("UgcFeedAction::Process --> pid:" << cwf_pid << " uid:" << u->id()  << " types size:" << types.size() << " seq: " << feed_seq.size());
 /* else {
    try {
      ptime pt("getfeed", false, 30);
      feed::FeedItemManagerPrx fprx = CachedFeedItemAdapter(u->id());

      LOG_F(INFO) << "feed retrive. type=" << type << " uid=" << u->id();

      std::vector<int> types;
      if (-1 == type) {
        feed_seq = fprx->getFeedData(u->id(), page * count, count);
      } else {
        // 应康伟要求: 如果是取状态，就只取 502 类型的新鲜事，去掉头像变更
        if (5 == type) {
          types.push_back(502);
          feed_seq = fprx->getFeedDataByStype(u->id(), types, page * count, count);
        } else {
          types.push_back(type * 100);
          feed_seq = fprx->getFeedDataByType(u->id(), types, page * count, count);
        }
      }

      if (pt.wall_clock() > 30)
        XAR_INC(gfGT30);
    } catch (Ice::Exception & e) {
      LOG_F(WARNING) << "FeedItemAdapter error : " << e;
      // return cwf::HC_SERVICE_UNAVAILABLE;
    }
  } */

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

    //if (pt.wall_clock() > 30)
     // XAR_INC(likeGT30);
  }
#endif
  MCE_INFO("UgcFeedAction::Process --> pid:" << cwf_pid << " uid:" << u->id() << " expand start.");

  {
    ptime pt("expand", false, 30);
    std::string stat;
    
    MyUtil::IntSeq uids, vipicontypes;
    vipicontypes.push_back(0); //目前只传0，具体有哪些类型，见OceCxxAdapter/src/VipIconDataReaderAdapter.h中的VIP_TYPE枚举类型
    for (size_t i = 0; i<feed_seq.size(); ++i) {
      uids.push_back(feed_seq[i].get()->actor);
    }
    vipinfo::VipFeedDataSeqMap vipInfoMap;
    const vipinfo::VipFeedDataSeq emptyVipData;
    try {
      vipInfoMap = adapter::vipicondata::VipIconDataReaderAdapter::instance().getVipFeedDataSeqMap(uids, vipicontypes);
    } catch (Ice::Exception& e) {
      MCE_WARN("UgcFeedAction::Process --> pid:" << cwf_pid <<" getVipIconData exception:"<< e);
    } catch (std::exception& e) {
      MCE_WARN("UgcFeedAction::Process --> pid:" << cwf_pid <<" getVipIconData exception:"<< e.what());
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
      //xce::feed::FeedStatLogPrx prx = CachedFeedStatLogAdapter(u->id());
      //LOG(INFO) << "prx:" << prx;
      //if (!prx) {
        //MCE_WARN("feedtypeaction.cc::Process --> FeedStatLog prx is NULL !");
      //} else {
        std::vector<std::string> cate;
        cate.push_back("feed");
        //cate.push_back("stat");
        cate.push_back("retrieve");
        //prx->AddLog(cate, stat);
        feed::FeedStatLogAdapter::instance().AddLog(cate, stat);
      //}
    } catch (Ice::Exception& e) {
      MCE_WARN("feedtypeaction.cc::Process --> exception:" << e);
    }


    //if (pt.wall_clock() > 30)
     // XAR_INC(exGT30);
  }

  MCE_INFO("UgcFeedAction::Process --> pid:" << cwf_pid << " uid:" << u->id() 
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
  // LOG(INFO) << "Register UgcFeedAction";
  cwf::FrameWork::RegisterAction(new UgcFeedAction);

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
