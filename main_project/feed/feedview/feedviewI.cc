#include "feed/feedview/feedviewI.h"

//#include "feed/base_feed/logging.h"
#include <time.h>
#include <sys/time.h>
#include "feed/base_feed/ptime.h"
#include "feed/base_feed/pcount.h"
#include "feed/base_feed/startuplist.h"
//#include "feed/base_feed/logrotate.h"

#include "feed/feedview/modifytime.h"
#include "feed/feedview/whitelist.h"
#include "feed/base_feed/signals.h"
#include <signal.h>

#include "feed/rfeed_adapter.h"
#include "feed/feedview/expandfeed.h"
#include "feed/feedview/feed_hdf_parser.h"
#include "feed/ilikeit_adapter.h"
#include "feed/feedview/feedcache.h"
#include "feed/feedview/feedxml.h"
#include "Notify/util/clear_silver_wrap.h"
#include "feed/cwf_feed/site.h"

#include "IceLogger.h"
//#include "FeedNewsReplicaAdapter.h"
#include "FeedNews50Adapter.h"
#include "FeedNewsReplicaAdapter.h"
#include "FeedMiniReplicaAdapter.h"
#include "FeedItemCacheByAppidReplicaAdapter.h"
#include "FeedItemCacheByUseridReplicaAdapter.h"
#include "FeedItemWrapperReplicaAdapter.h"
#include "BuddyByOnlineTimeReplicaAdapter.h"
#include "FeedContentAdapter.h"
#include "FeedMemcLoaderAdapter.h"
#include "TimeLineFeedReplicaAdapter.h"
#include "util/cpp/InvokeMonitor.h"
#include "util/cpp/TimeCost.h"

#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "ctemplate/template.h"
#include "OceCxxAdapter/src/VipIconDataReaderAdapter.h"
#include "OceCxxAdapter/src/UserUrlAdapter.h"
#include "FeedFocusAdapter.h"
//#include "FeedStatLogAdapter.h"
#include "feedview_statLog.h"
#include "feed/feedview/ambcache.h"
#include "feed/feedview/quicklz.h"
#include "FeedAssistantNReplicaAdapter.h"
#include "feedview_statLog.h"
#include "FeedGuideReplicaAdapter.h"
//Controller迁移
#include "feed/ReplicaCluster/ServerState/src/client/cxx/FeedServerStateSubscriber.h"
// 引入降级组件
#include "feed/Degrade/src/Degrade.h"

//#include "feed/arch_feed/service.h"
using namespace xce;
using namespace xce::feed;
using namespace com::xiaonei::xce;
using namespace ::xce::serverstate;

#define PRODUCT_SIGNAL_AMB_LOAD_UID 61

static void SignalReload2(int) {
  ctemplate::Template::ReloadAllIfChanged();
}

static void SignalReload3(int) {
  TaskManager::instance().execute(new AmbInitTask());
}

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&xce::FeedViewI::instance(), service.createIdentity("feedview", ""));


  ctemplate::Template::SetTemplateRootDirectory("/data/xce/XFeed/etc/feed_view_tpl3");
  ctemplate::Template::AddAlternateTemplateRootDirectory("/data/cwf2/tpl3");
  ctemplate::AddModifier("x-time", &friendly_time);
  ctemplate::AddModifier("x-js-escape", &friendly_xjs);
  ctemplate::AddModifier("x-tltime-format", &friendly_timeline_time);

  // TODO: move to perfect place
  InstallSignal(XCE_SIGNAL_CWF_LOAD_TEMPLATE, &SignalReload2);
  InstallSignal(PRODUCT_SIGNAL_AMB_LOAD_UID, &SignalReload3);

  TaskManager::instance().execute(new WhiteInitTask());
  TaskManager::instance().execute(new AmbInitTask());
  TaskManager::instance().scheduleRepeated(new AmbLoadTask());
/*
  std::string name = service.getName();
  std::string log_dir = //props->getPropertyWithDefault(
  service.getCommunicator()->getProperties()->getPropertyWithDefault(
    "Service." + name + ".LogPath"
    , "../log/" + name);


  int log_level = LS_INFO;
  {
    std::string level = //props->getProperty("Service." + name + ".LogLevel");
    service.getCommunicator()->getProperties()->getProperty("Service." + name + ".LogLevel");
    if (!level.empty()) {
      log_level = LogRotate::LogLevel(level);
    }
  }

  LOG(INFO) << "log_dir:" << log_dir << " log_level:" << log_level;

  if (log_level != -1)
    LogRotate::instance().Start(log_dir, log_level);
*/
/*
{

  // 缺省log至 ../log/box_shop.log
  std::string filename_log("../log/box_log");

  std::ofstream* stem = new std::ofstream(filename_log.c_str(), std::ios::binary);
  LogMessage::LogToStream(stem, LS_INFO);

} */

    //detail::IceThreadHook* hook = new detail::IceThreadHook;

/*
    Ice::InitializationData init;
    init.logger = new IceLogger;
    init.threadHook = hook;
    init.properties = props;
    init.stats = communicator->getStats(); // ??? TODO: use
    cloned = Ice::initialize(init);
 */
    //XAR_POLL("icethr", boost::bind(&detail::IceThreadHook::size, hook));

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("FeedViewN.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedViewN.Interval", 5);
  MCE_INFO("mod:" << mod << " interval:" << interval);

  ServerStateSubscriber::instance().initialize("ControllerFeedViewR",&FeedViewI::instance(),mod,interval, new XceFeedChannel);
  xce::feed::serverstate::FeedServerStateSubscriber::instance().initialize("ControllerFeedViewR",&FeedViewI::instance(), mod, interval, new XceFeedControllerChannel());
#if 1
  int is_test = MyUtil::ServiceI::instance().getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedViewN.IsTest", 0);
  FeedTplCache::Instance().Reload(is_test);

  if (!FeedSchemaCache::Instance().Reload(is_test)) {
    MCE_WARN("FeedHdfParser load error!");
  }
#endif
  FeedViewStatLog::instance().initLogger();
  // 设置db源
  DegradeManager::instance().SetDBSource("feed_degrade");
  // 注册降级模块 
  DegradeManager::instance().Register("FeedView_VipInfo");
  DegradeManager::instance().Register("FeedView_LikeInfo");
  // 然后初始化
  DegradeManager::instance().Init();
}

namespace xce {

XAR_IMPL(News);
XAR_IMPL(NewsT);
XAR_IMPL(NewsS);

XAR_IMPL(Mini);
XAR_IMPL(MiniT);
XAR_IMPL(MiniS);

XAR_IMPL(FeedErr);      // exception from FeedItem
XAR_IMPL(ExpandGT10);   // 模板expand超过 10ms

XAR_IMPL(FeedGT30);     // feednews 调用超过 30 ms
XAR_IMPL(LikeGT3);      // ilike 调用超过 3ms

XAR_IMPL(FocusGT30);      // focus/favorite 调用超过 3ms

static feed::HtmlResult kEmptyResult;

static int ExpandUserFeeds(Ice::Int owner, Ice::Int uid, const std::string& name, Ice::Int render
    , feed::FeedDataSeq& feed_seq, Ice::Int from, feed::HtmlResult * html, bool isAdmin = false, Ice::Int view = 0) {
  ASSERT(!feed_seq.empty() && html);
  html->reserve(feed_seq.size());

  int feed_count = 0; // 实际输出的 Feed条目数

  // 得到“赞” ilike 信息
  ::xce::ilikeit::LikeInfoSeq likeseq;
  std::vector<char> vbool(0, feed_seq.size()); // 有些类型并不需要 zan
  bool likeinfo_avaible = false;
  if (DegradeManager::instance().IsEnable("FeedView_LikeInfo"))
  {
    ptime pt("buildzan", false);
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
      struct timeval start, end;
      gettimeofday(&start, NULL);
      likeinfo_avaible = BuildZan(uid, globalseq, &likeseq);
      gettimeofday(&end, NULL);
      double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

      MCE_INFO("ExpandUserFeeds --> uid:" << uid << " zan dur:" << timeuse / 1000000);
    }

    if (pt.wall_clock() > 3)
      XAR_INC(LikeGT3);
  }

  xce::adapter::userurl::UserUrlInfoPtr info;
  string tinyimg = "NoTiny";
  {
    try {
      info = xce::adapter::userurl::UserUrlAdapter::instance().getUserUrl(uid);
      if(info) {
        tinyimg = info->tinyUrl();
      }
    } catch (Ice::Exception& e) {
      MCE_WARN("ExpandUserFeeds --> uid:" << uid <<" getTinyImgFeedData exception:"<< e);
    } catch (std::exception& e) {
      MCE_WARN("ExpandUserFeeds --> uid:" << uid <<" getTinyImgFeedData exception:"<< e.what());
    }
  }
  cwf::User u(name, uid, tinyimg);
  //cwf::User u(name, uid);
  {
    ptime pt("expand", false);
    std::string stat;

    MyUtil::IntSeq uids, types;
    types.push_back(0); //目前只传0，具体有哪些类型，见OceCxxAdapter/src/VipIconDataReaderAdapter.h中的VIP_TYPE枚举类型
    for (size_t i = 0; i<feed_seq.size(); ++i) {
      uids.push_back(feed_seq[i].get()->actor);
    }
    vipinfo::VipFeedDataSeqMap vipInfoMap;
    const vipinfo::VipFeedDataSeq emptyVipData;
    try {
      if (DegradeManager::instance().IsEnable("FeedView_VipInfo")) {
        vipInfoMap = adapter::vipicondata::VipIconDataReaderAdapter::instance().getVipFeedDataSeqMap(uids, types);
      }
    } catch (Ice::Exception& e) {
      MCE_WARN("ExpandUserFeeds --> uid:" << uid <<" getVipFeedData exception:"<< e);
    } catch (std::exception& e) {
      MCE_WARN("ExpandUserFeeds --> uid:" << uid <<" getVipFeedData exception:"<< e.what());
    }

    std::string exposeTime = lexical_cast<std::string>(time(0));
    for (size_t i = 0,likeidx = 0; i<feed_seq.size(); ++i) {
      const ilikeit::LikeInfo* likeinfo = 0;
      if (vbool[i] && likeinfo_avaible)
        likeinfo = likeseq[likeidx++].get();
      else
        likeinfo = 0;

      std::string output;
      std::string stat_log;
      bool f = false;

      vipinfo::VipFeedDataSeqMap::const_iterator it = vipInfoMap.find(feed_seq[i].get()->actor);
      if(it == vipInfoMap.end() || (it->second).empty()) {
        f = ExpandFeedData(feed_seq[i].get(), &u, owner, (WeiredSource)render, likeinfo, from, emptyVipData, &output, stat_log, exposeTime, isAdmin, view);
      }
      else {
        f = ExpandFeedData(feed_seq[i].get(), &u, owner, (WeiredSource)render, likeinfo, from, it->second, &output, stat_log, exposeTime, isAdmin, view);
      }

      //MCE_DEBUG("expand feed data fid:" << feed_seq[i].get()->feed << " u:" << (&u)->id() << " result:" << f);
      if (f) {
        ++ feed_count;
        ++ from;
        html->push_back(output);
        stat += stat_log;
        stat += "\n";
      }
    }
#if 1
    try {
      //xce::feed::FeedStatLogPrx prx = CachedFeedStatLogAdapter(uid);
      //LOG(INFO) << "prx:" << prx;
      //if (!prx) {
        //LOG(INFO) << "feedviewI.cc::expand --> FeedStatLog prx is NULL !";
        //MCE_WARN("feedviewI.cc::expand --> FeedStatLog prx is NULL !");
      //} else {
        std::vector<std::string> cate;
        cate.push_back("feed");
        cate.push_back("stat");
        //LOG(INFO) << "stat:" << stat;
        //prx->AddLog(cate, stat);
        //FeedStatLogAdapter::instance().AddLog(cate, stat);
        FeedViewStatLog::instance().AddLog(cate, stat);
      //}
    } catch (Ice::Exception& e) {
      //LOG_F(WARNING) << e;
      MCE_WARN("feedviewI.cc::expand -->" << e);
    }
#endif
    // ASSERT(feed_count == feed_seq.size());

    if (pt.wall_clock() > 10)
      XAR_INC(ExpandGT10);
  }
  return feed_count;
}

///@brief 新鲜事批量渲染接口
///@function
/// 对一批新鲜事首先解析xml;
/// 批量取likeinfo;
/// 批量取vipinfo;
/// 分条渲染

static int ExpandUserFeedsFeedView(Ice::Int owner, Ice::Int uid, const std::string& name,
    Ice::Int render, feed::FeedDataSeq& feed_seq, Ice::Int from, feed::HtmlResult * html,
    bool isAdmin = false, Ice::Int view = 0) {
  ASSERT(!feed_seq.empty() && html);
  html->reserve(feed_seq.size());

  int feed_count = 0; // 实际输出的 Feed条目数
  xce::adapter::userurl::UserUrlInfoPtr info;
  string tinyimg = "NoTiny";
  {
    try {
      info = xce::adapter::userurl::UserUrlAdapter::instance().getUserUrl(uid);
      if(info) {
        tinyimg = info->tinyUrl();
      }
    } catch (Ice::Exception& e) {
      MCE_WARN("ExpandUserFeeds --> uid:" << uid <<" getTinyImgFeedData exception:"<< e);
    } catch (std::exception& e) {
      MCE_WARN("ExpandUserFeeds --> uid:" << uid <<" getTinyImgFeedData exception:"<< e.what());
    }
  }
  cwf::User u(name, uid, tinyimg);
  //cwf::User u(name, uid);
/// 对一批新鲜事解析

  //判断新鲜事的新版老版
  vector<bool> isClearSilver;
  int cntClearSilver = 0;
  isClearSilver.reserve(feed_seq.size());
  for(size_t i = 0; i < feed_seq.size(); ++ i) {
    const string &xml = (feed_seq[i].get())->xml;
    size_t pos = xml.find_first_of('>');
    isClearSilver.push_back(xml.substr(0, pos).find("tpl=\"cs\"") != string::npos);
    cntClearSilver += isClearSilver[i] == true;
  }
  //声明解析器
  vector<FeedXml> feedXmls;
  feedXmls.resize(feed_seq.size() - cntClearSilver);
  vector<bool> feedXmlsAvailable;
  feedXmlsAvailable.resize(feed_seq.size() - cntClearSilver);
  vector<FeedHdfParser *> feedHdfParsers;
  feedHdfParsers.resize(cntClearSilver);
  //解析
  int curHdfParserIndex = 0;
  int curFeedXmlIndex = 0;
  for(size_t i = 0; i < feed_seq.size(); ++ i) {
    FeedData * data = feed_seq[i].get();
    if (isClearSilver[i]) {
      feedHdfParsers[curHdfParserIndex] = NULL;
      curHdfParserIndex ++;
      const FeedXmlSchema * feed_schema = FeedSchemaCache::Instance().FindFeedSchema(WeiredType(data->type).type,
          WeiredType(data->type).version);
      if (!feed_schema) {
        MCE_WARN("feed schema not found : uid=" << (&u)->id() << " stype=" << WeiredType(data->type).type << " version=" << WeiredType(data->type).version);
        continue ;
      }
      FeedHdfParser * feed_parser = NULL;
      if(WeiredType(data->type).type == 8182) {
        size_t index = data->xml.find("tpl=\"cs\"");
        if(index != std::string::npos && data->xml.find("tpl=\"cs\"", index + 8) != std::string::npos) {
          feed_parser = feed_schema->CreateHdfParserWithAppendByStype();
        } else {
          feed_parser = feed_schema->CreateHdfParser();
        }
      } else {
        feed_parser = feed_schema->CreateHdfParser();
      }

      if (feed_parser->Parse(data->xml) != 0) {
        MCE_WARN("feed data parse error, fid:" << data->feed);
        delete feed_parser;
        feed_parser = NULL;
      }
      feedHdfParsers[curHdfParserIndex - 1] = feed_parser;
    } else {
      FeedXml& fx = feedXmls[curFeedXmlIndex];
      feedXmlsAvailable[curFeedXmlIndex] = true;
      curFeedXmlIndex ++;
      std::string packed_xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?><feeds>");

      packed_xml += data->xml;
      packed_xml += "</feeds>";

      int ret = ParseCtemplateXml(packed_xml, &fx);

      if (0 != ret) {
        MCE_WARN("xml parse error: " << ret
          << " id: " << data->feed);
        MCE_WARN("xml:" << packed_xml);
        feedXmlsAvailable[curFeedXmlIndex - 1] = false;
        continue;
      }

      if (fx.body.empty()) {
        MCE_WARN("empty body, fid:" << data->feed);
        feedXmlsAvailable[curFeedXmlIndex - 1] = false;
        continue;
      }

      if (fx.template_type == "cs") {
        feedXmlsAvailable[curFeedXmlIndex - 1] = false;
        continue;
      }
    }
  }

  vector<bool> canExpand;
  curFeedXmlIndex = 0;
  curHdfParserIndex = 0;
  for(size_t i = 0; i < feed_seq.size(); ++ i) {
    if (isClearSilver[i]) {
      canExpand.push_back(feedHdfParsers[curHdfParserIndex ++] != NULL);
    } else {
      canExpand.push_back(feedXmlsAvailable[curFeedXmlIndex ++]);
    }
  }

///获取喜欢信息
  ::xce::ilikeit::LikeInfoSeq likeseq;
  ::xce::ilikeit::GlobalIdSeq globalseq;
  curFeedXmlIndex = 0;
  curHdfParserIndex = 0;
  vector<bool> likeInfoAvailable(feed_seq.size() * 3, false);
  for(size_t i = 0; i < feed_seq.size(); ++ i) {
    feed::FeedData* feed = feed_seq[i].get();
    if (isClearSilver[i]) {
      ++ curHdfParserIndex;
    } else {
      ++ curFeedXmlIndex;
    }
    likeInfoAvailable[3 * i] = false;
    likeInfoAvailable[3 * i + 1] = false;
    likeInfoAvailable[3 * i + 2] = false;
    if (canExpand[i]) {
      if (BuildGlobalId(feed->source, feed->type, &globalseq)) {
        likeInfoAvailable[3 * i] = true;
      }
      if (!isClearSilver[i]) {
        const std::vector<std::string> reply_v = feedXmls[curFeedXmlIndex - 1].find_v("f_reply_id");
        for(size_t j = 0; j < 2 && j < reply_v.size(); ++ j) {
          long long feed_reply_id = 0;
          try {
            feed_reply_id = boost::lexical_cast<long long>(reply_v[j]);
          } catch(std::exception &e) {
            MCE_WARN("ExpandUserFeedsFeedView --> transform err : " << e.what());
          }
          if (BuildCommentGlobalId(feed_reply_id, feed->type, &globalseq)) {
            likeInfoAvailable[3 * i + 1 + j] = true;
          }
        }
      } else {
        map<string, vector<map<string, string> > >& loop_nodes = feedHdfParsers[curHdfParserIndex - 1]->loop_nodes();
        map<string, vector<map<string, string> > >::iterator it = loop_nodes.find("f.reply");
        long long feed_reply_id = 0;
//        if (it != loop_nodes.end()) {
//          for(map<string, vector<map<string, string> > >::iterator it = loop_nodes.begin();it != loop_nodes.end(); ++ it) {
//            MCE_INFO("ExpandUserFeedsFeedView --> key: " << (it -> first) << "  val.size(): " << (it->second).size());
//            for(int i = 0; i < (it->second).size(); ++ i) {
 //             map<string, string>& m = it->second[i];
//              for(map<string, string>::iterator mit = m.begin(); mit != m.end(); ++ mit) {
//                MCE_INFO("ExpandUserFeedsFeedView --> map key: " << (mit -> first) << "  val: " << (mit->second));
//              }
//            }
//          }
//        }
        if (it != loop_nodes.end() && (it->second).size() > 0) {
          try {
            map<string, string>::iterator mit = (it -> second).at(0).find("id");
            if (mit != (it -> second).at(0).end()) {
//              MCE_INFO("ExpandUserFeedsFeedView --> feed_reply_id 1 : " << (mit->second));
              feed_reply_id = boost::lexical_cast<long long>(mit->second);
            }
          } catch(std::exception &e) {
//            MCE_WARN("ExpandUserFeedsFeedView --> transform err : " << e.what());
          }
          if (BuildCommentGlobalId(feed_reply_id, feed->type, &globalseq)) {
            likeInfoAvailable[3 * i + 1] = true;
          }
//          MCE_INFO("ExpandUserFeedsFeedView --> reply_id : " << feed_reply_id << "likeInfoAvailable 1: " << likeInfoAvailable[3 * i + 1]);
        }
        if (it != loop_nodes.end() && (it->second).size() > 1) {
          try {
            map<string, string>::iterator mit = (it -> second).at(1).find("id");
            if (mit != (it -> second).at(1).end()) {
//              MCE_INFO("ExpandUserFeedsFeedView --> feed_reply_id 2 : " << (mit->second));
              feed_reply_id = boost::lexical_cast<long long>(mit->second);
            }
          } catch(std::exception &e) {
            MCE_WARN("ExpandUserFeedsFeedView --> transform err : " << e.what());
          }
          if (BuildCommentGlobalId(feed_reply_id, feed->type, &globalseq)) {
            likeInfoAvailable[3 * i + 2] = true;
          }
//          MCE_INFO("ExpandUserFeedsFeedView --> reply_id : " << feed_reply_id << "likeInfoAvailable 2: " << likeInfoAvailable[3 * i + 2]);
        }
      }
    }
  }

  bool likeinfo_avaible = false;
  if (!globalseq.empty()) {
    // try catch inside
    struct timeval start, end;
    gettimeofday(&start, NULL);
    if (DegradeManager::instance().IsEnable("FeedView_LikeInfo")) {
      likeinfo_avaible = BuildZan(uid, globalseq, &likeseq);
    }
    gettimeofday(&end, NULL);
    double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    MCE_INFO("ExpandUserFeeds --> uid:" << uid << " zan dur:" << timeuse / 1000000);
  }
/// 获取Vip信息， 分条渲染
  bool isParsed = true;
  {
    ptime pt("expand", false);
    std::string stat;

    MyUtil::IntSeq uids, types;
    types.push_back(0); //目前只传0，具体有哪些类型，见OceCxxAdapter/src/VipIconDataReaderAdapter.h中的VIP_TYPE枚举类型
    for (size_t i = 0; i < feed_seq.size(); ++i) {
      uids.push_back(feed_seq[i].get()->actor);
    }
    vipinfo::VipFeedDataSeqMap vipInfoMap;
    const vipinfo::VipFeedDataSeq emptyVipData;
    try {
      if (DegradeManager::instance().IsEnable("FeedView_VipInfo")) {
        vipInfoMap = adapter::vipicondata::VipIconDataReaderAdapter::instance().getVipFeedDataSeqMap(uids, types);
      }
    } catch (Ice::Exception& e) {
      MCE_WARN("ExpandUserFeeds --> uid:" << uid <<" getVipFeedData exception:"<< e);
    } catch (std::exception& e) {
      MCE_WARN("ExpandUserFeeds --> uid:" << uid <<" getVipFeedData exception:"<< e.what());
    }
    curFeedXmlIndex = 0;
    curHdfParserIndex = 0;
    std::string exposeTime = lexical_cast<std::string>(time(0));
    for (size_t i = 0, iLikeIdx = 0; i<feed_seq.size(); ++i) {
      if (isClearSilver[i]) {
        ++ curHdfParserIndex;
      } else {
        ++ curFeedXmlIndex;
      }
      if (!canExpand[i]) {
        continue;
      }
      vector<ilikeit::LikeInfo*> replyLikeInfoVec(2);
      const ilikeit::LikeInfo* feedLikeInfo;
      if (likeinfo_avaible && likeInfoAvailable[3 * i]) {
        feedLikeInfo = likeseq[iLikeIdx ++].get();
      } else {
        feedLikeInfo = 0;
      }
      for(int j = 1; j < 3; ++ j) {
        if (likeinfo_avaible && likeInfoAvailable[3 * i + j]) {
          replyLikeInfoVec[j - 1] = likeseq[iLikeIdx ++].get();
        } else {
          replyLikeInfoVec[j - 1] = 0;
        }
      }

      std::string output;
      std::string stat_log;
      bool f = false;

      FeedXml fx;
      FeedHdfParser* fhp = NULL;
      if (isClearSilver[i]) {
        fhp = feedHdfParsers[curHdfParserIndex - 1];
      } else {
        fx = feedXmls[curFeedXmlIndex - 1];
      }
      vipinfo::VipFeedDataSeqMap::const_iterator it = vipInfoMap.find(feed_seq[i].get()->actor);
      if (it == vipInfoMap.end() || (it->second).empty()) {
        f = ExpandFeedData(feed_seq[i].get(), &u, owner, (WeiredSource)render, feedLikeInfo, from, emptyVipData, &output, stat_log, exposeTime, isAdmin, view, fhp, fx, replyLikeInfoVec, isParsed);
      } else {
        f = ExpandFeedData(feed_seq[i].get(), &u, owner, (WeiredSource)render, feedLikeInfo, from, it->second, &output, stat_log, exposeTime, isAdmin, view, fhp, fx, replyLikeInfoVec, isParsed);
      }
      if (fhp != NULL) {
        delete fhp;
        fhp = NULL;
      }
      MCE_DEBUG("expand feed data fid:" << feed_seq[i].get()->feed << " u:" << (&u)->id() << " result:" << f);
      if (f) {
        ++ feed_count;
        ++ from;
        html->push_back(output);
        stat += stat_log;
        stat += "\n";
      }
    }
#if 1
    try {
        std::vector<std::string> cate;
        cate.push_back("feed");
        cate.push_back("stat");
        FeedViewStatLog::instance().AddLog(cate, stat);
    } catch (Ice::Exception& e) {
      MCE_WARN("feedviewI.cc::expand -->" << e);
    }
#endif

    if (pt.wall_clock() > 10)
      XAR_INC(ExpandGT10);
  }
  return feed_count;
}


bool FeedViewI::ReloadCSTemplate(const Ice::Current&) {
  int is_test = MyUtil::ServiceI::instance().getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedViewN.IsTest", 0);
  return FeedTplCache::Instance().Reload(is_test);
}

bool FeedViewI::ReloadDataSchema(const Ice::Current&) {
  int is_test = MyUtil::ServiceI::instance().getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FeedViewN.IsTest", 0);
  return FeedSchemaCache::Instance().Reload(is_test);
}

void FeedViewI::ReloadWhitelist(const Ice::Current&) {
  TaskManager::instance().execute(new WhiteInitTask());
}

feed::HtmlResult FeedViewI::Get(Ice::Int uid, const std::string& name, Ice::Int render
      , Ice::Int from, Ice::Int count, const Ice::Current& current) {
  XAR_INC(News);

  //if (uid % 10 == 2 || uid % 10 == 1 || uid % 10 == 9 || uid % 10 == 3 || uid % 10 == 4 || uid % 10 == 0) {
    return GetHotByType(uid, name , render, feed::TypeSeq(), from, count, current);
  //}

  //return GetByType(uid, name, render, feed::TypeSeq(), from, count, current);
}

feed::HtmlResult FeedViewI::GetByType(Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& types
    , Ice::Int from, Ice::Int count, const Ice::Current& cur) {

  XAR_INC(NewsT);
  feed::FeedDataSeq feed_seq;

  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetByType", InvokeClient::INVOKE_LEVEL_INFO);
  //LOG(INFO) << "connection: " << cur.con->toString();
  try {
    ptime pt("getFeed", false);

    // feed::FeedItemManagerPrx fprx = CachedFeedItemAdapter(uid);

    if (types.empty()) {
      // feed_seq = fprx->getFeedData(uid, from, count);
    } else {
      // TODO : stop TEST. feed_seq = fprx->getFeedDataByType(uid, types, from, count);
      feed::FeedDataResPtr feed_res;
      feed_res = FeedNewsReplicaAdapter::instance().GetHotFeedDataByType(uid, types, from, count);
      feed_seq = feed_res->data;
    }

    if (pt.wall_clock() > 30)
      XAR_INC(FeedGT30);
  } catch (Ice::Exception & e) {
    XAR_INC(FeedErr);
    LOG_F(WARNING) << e;
    MCE_WARN("FeedViewI::GetByType() -->" << e);
  }

  //LOG(INFO) << "GetByType() uid=" << uid << " type.size=" << types.size() << " count=" << feed_seq.size() << '/' << count;
  MCE_INFO("FeedViewI::GetByType() --> uid :" << uid << " type.size :" << types.size() << " count :" << feed_seq.size() << '/' << count);


  if (feed_seq.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  ExpandUserFeeds(0, uid, name, render, feed_seq, from, &ret);

  return ret;
}

feed::HtmlResult FeedViewI::GetByStype(Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& types
    , Ice::Int from, Ice::Int count, const Ice::Current& cur) {
  XAR_INC(NewsS);

  feed::FeedDataSeq feed_seq;
  //LOG(INFO) << "connection: " << cur.con->toString();
  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetByStype", InvokeClient::INVOKE_LEVEL_INFO);
  try {
    ptime pt("getFeedByStype", false);
    // feed::FeedItemManagerPrx fprx = CachedFeedItemAdapter(uid);
    // TODO : stop test. feed_seq = fprx->getFeedDataByStype(uid, types, from, count);
    // feed_seq = FeedNewsReplicaAdapter::instance().GetHotFeedDataBySType(uid, types, from, count);
    feed::FeedDataResPtr feed_res = FeedNewsReplicaAdapter::instance().GetHotFeedDataByStype(uid, types, from, count);

    feed_seq = feed_res->data;

    if (pt.wall_clock() > 3)
      XAR_INC(FeedGT30);
  } catch (Ice::Exception & e) {
    XAR_INC(FeedErr);
    LOG_F(WARNING) << e;
  }

  //LOG(INFO) << "GetByStype() uid=" << uid << " type.size=" << types.size() << " count=" << feed_seq.size() << '/' << count;
  MCE_INFO("FeedViewI::GetBySType() --> uid :" << uid << " type.size :" << types.size() << " count :" << feed_seq.size() << '/' << count);

  if (feed_seq.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  ExpandUserFeeds(0, uid, name, render, feed_seq, from, &ret);
  return ret;
}

feed::HtmlResult FeedViewI::GetByIds(Ice::Int uid, const std::string& name
    , const feed::FeedIdSeq & fids
    , const Ice::Current&) {
  return GetByIds2(uid, name, 0, fids);
}

feed::HtmlResult FeedViewI::GetByIds2(Ice::Int uid, const std::string& name, Ice::Int render
    , const feed::FeedIdSeq & fids
    , const Ice::Current& cur) {
  //////////////
  //LOG(INFO) << "GetByIds2: " << uid << " " << name << " " << render;
  if (fids.empty()) {
    MCE_INFO("FeedViewI::GetByIds2() --> GetByIds2: uid=" << uid << " " << name << " " << render << " size=0");
  } else {
    MCE_INFO("FeedViewI::GetByIds2() --> GetByIds2: uid=" << uid << " " << name << " " << render << " id[0]=" << fids[0]);
  }
  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetByIds2", InvokeClient::INVOKE_LEVEL_INFO);
////////////////
  //render = 0;
  //LOG(INFO) << "connection: " ;
  feed::FeedDataSeq feed_seq;

  try {
    //LOG(INFO) << "before fprx.";
   // feed::FeedItemManagerPrx fprx = CachedFeedItemAdapter(uid);
    //feed::FeedNewsManagerPrx fprx = CachedFeedNews2Adapter(uid);
    //LOG(INFO) << "fprx: " << fprx;
    //feed_seq = fprx->GetFeedDataByIds(fids);
    //LOG(INFO) << "feed_seq.size:" << feed_seq.size();
    feed_seq = FeedNewsReplicaAdapter::instance().GetFeedDataByIds(fids);
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedViewI::GetByIds2() --> uid=" << uid << " "<< e);
  }

  MCE_INFO("FeedViewI::GetByIds2() --> uid=" << uid  << feed_seq.size());

  if (feed_seq.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  if (!(render & WS_MINIFEED)) {
    render |= WS_HOME;
  }
  render |= WS_SIMILAR;
  //LOG(INFO) << "before expand.";
  ExpandUserFeeds(0, uid, name, render, feed_seq, 0, &ret);
  //LOG(INFO) << "ret size:" << ret.size();
  return ret;
}


feed::HtmlResult FeedViewI::GetByIdsAndStype(Ice::Int uid, const std::string& name
    , const feed::FeedIdSeq & fids, int stype, long firstID
    , const Ice::Current&) {
  return GetByIdsAndStype2(uid, name, 0, fids, stype, firstID);
}

feed::HtmlResult FeedViewI::GetByIdsAndStype2(Ice::Int uid, const std::string& name, Ice::Int render
    , const feed::FeedIdSeq & fids, int stype, long firstID
    , const Ice::Current& cur) {
  //////////////
  //LOG(INFO) << "GetByIdsAndStype2: " << uid << " " << name << " " << render << " " << stype << " " << firstID;
  MCE_INFO("FeedViewI::GetByIdsAndStype2 --> " << uid << " " << name << " " << render << " " << stype << " " << firstID);
  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetByIdsAndStype2", InvokeClient::INVOKE_LEVEL_INFO);
////////////////
  //render = 0;
  //LOG(INFO) << "connection: " ;
  feed::FeedDataSeq feed_seq;

  try {
    ptime pt("getFeedById", false);
    //LOG(INFO) << "before fprx.";
   // feed::FeedItemManagerPrx fprx = CachedFeedItemAdapter(uid);
    //feed::FeedNewsManagerPrx fprx = CachedFeedNews2Adapter(uid);
    //feed::FeedNewsManagerPrx fprx = CachedFeedNews3Adapter(uid);
    //feed_seq = fprx->GetFeedDataByIdsWithUid(uid, fids, firstID, stype);
    feed_seq = FeedNewsReplicaAdapter::instance().GetFeedDataByIdsWithUid(uid, fids, firstID, stype);

    MCE_INFO("FeedViewI::GetByIdsAndStype2 --> feed_seq.size:" << feed_seq.size() << " firstID:" << firstID << " stype:" << stype);

    if (pt.wall_clock() > 3)
      XAR_INC(FeedGT30);
  } catch (Ice::Exception & e) {
    XAR_INC(FeedErr);
    LOG_F(WARNING) << e;
  }


  if (feed_seq.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  if (!(render & WS_MINIFEED)) {
    render |= WS_HOME;
  }
  //LOG(INFO) << "before expand.";
  ExpandUserFeeds(0, uid, name, render, feed_seq, 0, &ret);
  //LOG(INFO) << "ret size:" << ret.size();
  return ret;
}


feed::HtmlResult FeedViewI::GetMini(Ice::Int owner, Ice::Int uid, const std::string& name, Ice::Int render
      , Ice::Int from, Ice::Int count, const Ice::Current& current) {
  XAR_INC(Mini);
  return GetMiniByType(owner, uid, name, render|WS_MINIFEED, feed::TypeSeq()
    , from, count, current);
}

feed::HtmlResult FeedViewI::GetMiniByType(Ice::Int owner, Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& types
    , Ice::Int from, Ice::Int count, const Ice::Current& cur) {
  XAR_INC(MiniT);

  feed::FeedDataSeq feed_seq;

  struct timeval start, end;

  try {
    ptime pt("getFeed", false);
    //feed::FeedItemManagerPrx fprx = CachedFeedMiniReplicaAdapter(owner);
    //LOG(VERBOSE) << "owner: " << owner << " uid: " << uid << " proxy: " << fprx;
    MCE_INFO("FeedViewI::GetMiniByType --> owner: " << owner << " uid: " << uid);
    InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetMiniByType", InvokeClient::INVOKE_LEVEL_INFO);
    gettimeofday(&start, NULL);

    if (types.empty()) {
     //feed_seq = fprx->getFeedData(owner, from, count);
      feed_seq = FeedMiniReplicaAdapter::instance().getFeedData(owner, from, count);
      //MCE_INFO("Get all types");
    } else {
      //feed_seq = fprx->getFeedDataByType(owner, types, from, count);
      //for (size_t i = 0; i < types.size(); ++i) {
      //  MCE_INFO("type[" << i << '/' << types.size() << "]= " << types[i]);
      //}
      feed_seq = FeedMiniReplicaAdapter::instance().getFeedDataByType(owner, types, from, count);
    }
    if (pt.wall_clock() > 3)
      XAR_INC(FeedGT30);
  } catch (Ice::Exception & e) {
    XAR_INC(FeedErr);
    MCE_WARN("FeedViewI::GetMiniByType -->  owner:" << owner << " uid:" << uid << " exception:" << e);
  }

  gettimeofday(&end, NULL);
  double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

  MCE_INFO("FeedViewI::GetMiniByType -->  owner:" << owner << " uid:" << uid << " type.size:" << types.size() << " count:" << feed_seq.size() << '/' << count << " ice call dur:" << timeuse / 1000000);


  if (feed_seq.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  gettimeofday(&start, NULL);
  int expand_feed_suc = ExpandUserFeedsFeedView(owner, uid, name, render|WS_MINIFEED, feed_seq, from, &ret);
  gettimeofday(&end, NULL);
  timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

  MCE_INFO("FeedViewI::GetMiniByType -->  owner:" << owner << " uid:" << uid << " expand dur:" << timeuse / 1000000 << " expand_feed_suc:" << expand_feed_suc);
  return ret;
}

feed::HtmlResult FeedViewI::GetMiniByStype(Ice::Int owner, Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& types
    , Ice::Int from, Ice::Int count, const Ice::Current& cur) {
  XAR_INC(MiniS);

  feed::FeedDataSeq feed_seq;

  MCE_INFO("FeedViewI::GetMiniByStype --> uid=" << uid);
  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetMiniByStype", InvokeClient::INVOKE_LEVEL_INFO);
  try {
    ptime pt("getFeedByStype", false);
    //feed::FeedItemManagerPrx fprx = CachedFeedMiniReplicaAdapter(owner);
    //feed_seq = fprx->getFeedDataByStype(owner, types, from, count);
    feed_seq = FeedMiniReplicaAdapter::instance().getFeedDataByStype(owner, types, from, count);

    if (pt.wall_clock() > 3)
      XAR_INC(FeedGT30);
  } catch (Ice::Exception & e) {
    XAR_INC(FeedErr);
    //LOG_F(WARNING) << "GetMiniByStype() uid=" << uid << " error:" << e;
    MCE_WARN("FeedViewI::GetMiniByStype --> uid=" << uid << " error:" << e);
  }

  //LOG(INFO) << "GetMiniByStype() uid=" << uid << " type.size=" << types.size() << " count=" << feed_seq.size() << '/' << count;
  MCE_INFO("FeedViewI::GetMiniByStype --> uid=" << uid << " type.size=" << types.size() << " count=" << feed_seq.size() << '/' << count);

  if (feed_seq.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  ExpandUserFeeds(owner, uid, name, render|WS_MINIFEED, feed_seq, from, &ret);
  return ret;
}

feed::HtmlResult FeedViewI::GetFavorite(Ice::Int uid, const std::string& name
    , Ice::Int render, Ice::Int from, Ice::Int count, const Ice::Current& cur) {
  feed::FeedDataSeq feed_seq;

  MCE_INFO("FeedViewI::GetFavorite --> uid:" << uid);
  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetFavorite", InvokeClient::INVOKE_LEVEL_INFO);
  try {
    ptime pt("getFeed", false);
    //feed::FocusManagerPrx fprx = CachedFeedFocusAdapter(uid);

    //feed_seq = fprx->GetFocusFeedData(uid, from, count);
    feed_seq = feed::FeedFocusAdapter::instance().GetFocusFeedData(uid, from, count);

    if (pt.wall_clock() > 30)
      XAR_INC(FocusGT30);
  } catch (Ice::Exception & e) {
    XAR_INC(FeedErr);
    //LOG_F(WARNING) << "GetFavorite() uid=" << uid << " error:" << e;
    MCE_WARN("FeedViewI::GetFavorite --> uid:" << uid << " error:" << e);
  }

  MCE_INFO("FeedViewI::GetFavorite --> uid:" << uid << " count:" << feed_seq.size() << '/' << count);


  if (feed_seq.empty())
   return kEmptyResult;

  feed::HtmlResult ret;
  ExpandUserFeeds(0, uid, name, render | WS_FAVORITE, feed_seq, from, &ret);
  return ret;
}

feed::HtmlResult FeedViewI::GetFriendOriginal(Ice::Int uid, const std::string& name
    , Ice::Int render, Ice::Int from, Ice::Int count, const Ice::Current& cur) {

  feed::FeedDataResPtr feed_seq;
  FeedDataSeq exp_feed;

  ostringstream os;
  os << "FeedViewI::GetFriendOriginal --> uid:" << uid << " begin:" << from << " limit:" << count;
  InvokeClient tm = InvokeClient::create(cur, os.str(), InvokeClient::INVOKE_LEVEL_INFO);

  struct timeval start, end;
  gettimeofday(&start, NULL);

  vector<int> friend_id_seq;
  try{
    feed_seq = FeedNewsReplicaAdapter::instance().GetFriendOriginal(uid, friend_id_seq, from, count);
  } catch(Ice::Exception& e) {
    MCE_WARN("FeedViewI::GetFriendOriginal --> uid:" << uid << " error:" << e);
    gettimeofday(&end, NULL);
    double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    MCE_WARN("FeedViewI::GetFriendOriginal -->  uid:" << uid << " ice call error.  dur:" << timeuse / 1000000);
    return kEmptyResult;
  }

  if (!feed_seq || feed_seq->data.empty()){
    return kEmptyResult;
  }
  MCE_INFO("FeedViewI::GetFriendOriginal --> uid:" << uid << " count=" << feed_seq->data.size() << '/' << count);

  feed::HtmlResult ret;
  int expand_feed_suc = ExpandUserFeeds(0, uid, name, render | WS_FRIENDORIGINAL, feed_seq->data, from, &ret);
  MCE_INFO("FeedViewI::GetFriendOriginal --> uid:" << uid << " expand_feed_suc:" <<  expand_feed_suc);

  // 如果有条缺本体，就拼一个空值，以确保后面可以获得
  if (feed_seq && feed_seq->UnviewedCount == count && feed_seq->data.size() == count - 1) {
    ret.push_back("<li></li>");
  }
  return ret;
}

feed::HtmlResult FeedViewI::GetHot(Ice::Int uid, const std::string& name
    , Ice::Int render, Ice::Int from, Ice::Int count, const Ice::Current& cur) {

  feed::FeedDataResPtr feed_seq;
  FeedDataSeq exp_feed;
 /*
  try {
    feed::FeedNewsManagerPrx fprx = CachedFeedNews2Adapter(uid);
     //feed::FeedNewsManagerPrx fprx = CachedFeedNews3Adapter(uid);
    feed_seq = fprx->GetHotFeedData(uid, from, count);
  } catch (Ice::Exception & e) {
    LOG_F(WARNING) << "GetHot() uid=" << uid << " error:" << e;
    return kEmptyResult;
  }
*/

  ostringstream os;
  os << "FeedViewI::GetHot --> uid:" << uid << " begin:" << from << " limit:" << count;
  InvokeClient tm = InvokeClient::create(cur, os.str(), InvokeClient::INVOKE_LEVEL_INFO);

  struct timeval start, end;
  gettimeofday(&start, NULL);
  try{
    int white_view = Whitelist::instance().IsWhiteUser(uid);
    if(white_view > 0) {
      feed_seq = FeedNews50Adapter::instance().getHotFeedData(uid, from, count);
    } else {
      feed_seq = FeedNewsReplicaAdapter::instance().getHotFeedData(uid, from, count);
    }
  }catch(Ice::Exception& e){
    MCE_WARN("FeedViewI::GetHot --> uid:" << uid << " error:" << e);
    gettimeofday(&end, NULL);
    double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    MCE_WARN("FeedViewI::GetHot -->  uid:" << uid << " ice call error.  dur:" << timeuse / 1000000);
    return kEmptyResult;
    /*
    if(!feed_seq || feed_seq->data.empty()) {
       if(!FeedCache::instance().Get(uid, exp_feed)) {
         MCE_WARN("FeedViewI::GetHot --> FeedCache.Get() failed. uid:" << uid );
         return kEmptyResult;
       }
       MCE_INFO("FeedViewI::GetHot --> FeedCache.Get() success. uid:" << uid );
    }
    */
  }catch (std::exception & e) {
    MCE_WARN("FeedViewI::GetHot --> uid:" << uid << " error:" << e.what());
    gettimeofday(&end, NULL);
    double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    MCE_WARN("FeedViewI::GetHot -->  uid:" << uid << " ice call error.  dur:" << timeuse / 1000000);
    return kEmptyResult;
    /*
    if(!feed_seq || feed_seq->data.empty()) {
      if(!FeedCache::instance().Get(uid, exp_feed)) {
        MCE_WARN("FeedViewI::GetHot --> FeedCache.Get() failed. uid:" << uid );
        return kEmptyResult;
      }
      MCE_INFO("FeedViewI::GetHot --> FeedCache.Get() success. uid:" << uid );
    }
    */
  }

  if (feed_seq && !feed_seq->data.empty()){
    exp_feed = feed_seq->data;
    //MCE_INFO("FeedViewI::GetHot --> FeedCache add. uid:" << uid);
    //if(!FeedCache::instance().Add(uid, exp_feed)) {
    //  MCE_WARN("FeedViewI::GetHot --> FeedCache::instance().Add(" << uid << ", feed_seq) failed!" );
    //}
  }
  MCE_INFO("FeedViewI::GetHot --> uid:" << uid << " count=" << exp_feed.size() << '/' << count);

  if (exp_feed.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  //ExpandUserFeeds(0, uid, name, render | WS_HOT, feed_seq, from, &ret);
  gettimeofday(&start, NULL);
  int expand_feed_suc = ExpandUserFeedsFeedView(0, uid, name, render | WS_HOT, exp_feed, from, &ret);
  gettimeofday(&end, NULL);
  double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  MCE_INFO("FeedViewI::GetHot --> uid:" << uid << " expand dur:" << timeuse / 1000000 << " expand_feed_suc:" <<  expand_feed_suc);
  return ret;
}

feed::HtmlResult FeedViewI::GetHotByType(Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& types
    , Ice::Int from, Ice::Int count, const Ice::Current& cur) {
  feed::FeedDataResPtr feed_seq;
  MCE_INFO("FeedViewI::GetHotByType uid=" << uid);
  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetHotByType", InvokeClient::INVOKE_LEVEL_INFO);
  FeedDataSeq exp_feed;

  struct timeval start, end;
  gettimeofday(&start, NULL);

  try {
     //feed::FeedNewsManagerPrx fprx = CachedFeedNews2Adapter(uid);
     //feed_seq = fprx->GetHotFeedDataByType(uid, types, from, count);
    feed_seq = FeedNewsReplicaAdapter::instance().GetHotFeedDataByType(uid, types, from, count);
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedViewI::GetHotByType --> uid:" << uid << " error:" << e);

    gettimeofday(&end, NULL);
    double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    MCE_INFO("FeedViewI::GetHotByType -->  uid:" << uid << " ice call error.  dur:" << timeuse / 1000000);
    return kEmptyResult;
    //added by qun.liu 20110414
    /*
    if(!feed_seq || feed_seq->data.empty()) {
      MCE_INFO("FeedViewI::GetHotByType() --> feed_seq is empty.  uid:" << uid);
      if(!FeedCache::instance().Get(uid, exp_feed)) {
        MCE_INFO("FeedViewI::GetHotByType() --> FeedCache.Get() failed. uid:" << uid );
        return kEmptyResult;
      }
      MCE_INFO("FeedViewI::GetHotByType() --> FeedCache.Get() success. uid:" << uid );
    }
    */
  } catch (std::exception & e) {
    MCE_WARN("FeedViewI::GetHotByType --> uid:" << uid << " error:" << e.what());

    gettimeofday(&end, NULL);
    double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    MCE_INFO("FeedViewI::GetHotByType -->  uid:" << uid << " ice call error.  dur:" << timeuse / 1000000);
    return kEmptyResult;
    //added by qun.liu 20110414
    /*
    if(!feed_seq || feed_seq->data.empty()) {
      MCE_INFO("FeedViewI::GetHotByType() --> feed_seq is empty.  uid:" << uid);
      if(!FeedCache::instance().Get(uid, exp_feed)) {
        MCE_INFO("FeedViewI::GetHotByType() --> FeedCache.Get() failed. uid:" << uid );
        return kEmptyResult;
      }
    }
    if (feed_seq) {
      MCE_INFO("FeedViewI::GetHotByType() ice call error, use FeedCache. feed_seq size:" << feed_seq->data.size() << " uid:" << uid);
    }
    */
  }

  if (feed_seq && !feed_seq->data.empty()) {
    exp_feed = feed_seq->data;
    //MCE_INFO("FeedViewI::GetHotByType() --> FeedCache add. uid:" << uid);
    //if(!FeedCache::instance().Add(uid, exp_feed)) {
    //  MCE_WARN("FeedViewI::GetHotByType() --> FeedCache::instance().Add(" << uid << ", feed_seq) failed!" );
    //}
  }

  MCE_INFO("FeedViewI::GetHotByType --> uid:" << uid << " count:" << exp_feed.size() << '/' << count);

  //if (feed_seq->data.empty())
  if (exp_feed.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  //ExpandUserFeeds(0, uid, name, render | WS_HOT, feedSeq, from, &ret);
  gettimeofday(&start, NULL);

  //ExpandUserFeeds(0, uid, name, render | WS_HOT, feed_seq->data, from, &ret);
  int expand_feed_suc = ExpandUserFeeds(0, uid, name, render | WS_HOT, exp_feed, from, &ret);

  gettimeofday(&end, NULL);
  double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  MCE_INFO("FeedViewI::GetHotByType --> uid:" << uid << " expand dur:" << timeuse / 1000000 << " expand_feed_suc:" <<  expand_feed_suc);
  return ret;
}

feed::HtmlResult FeedViewI::GetHotByStype(Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& types, Ice::Int from, Ice::Int count, const Ice::Current& cur) {

  feed::FeedDataResPtr feed_seq;
  MCE_INFO("FeedViewI::GetHotByStype --> uid:" << uid);
  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetHotByStype", InvokeClient::INVOKE_LEVEL_INFO);
  try {
    feed_seq = FeedNewsReplicaAdapter::instance().GetHotFeedDataByStype(uid, types, from, count);
  } catch (Ice::Exception & e) {
    //LOG_F(WARNING) << "GetHotByStype() uid=" << uid << " error:" << e;
    MCE_WARN("FeedViewI::GetHotByStype --> uid:" << uid << " error:" << e);
    feed_seq = NULL;
    //return kEmptyResult;
  }

  MCE_INFO("FeedViewI::GetHotByStype --> uid:" << uid << " type.size=" << types.size() << " count=" << feed_seq->data.size() << '/' << count);
  if (feed_seq->data.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  //ExpandUserFeeds(0, uid, name, render | WS_HOT, feedSeq, from, &ret);
  ExpandUserFeeds(0, uid, name, render | WS_HOT, feed_seq->data, from, &ret);
  return ret;
}

feed::HtmlResult FeedViewI::GetLive(Ice::Int uid, const std::string& name
    , Ice::Int render, Ice::Int from, Ice::Int count, const Ice::Current& cur) {
  feed::FeedDataResPtr feed_seq;
  MCE_INFO("FeedViewI::GetLive --> uid:" << uid);
  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetLive", InvokeClient::INVOKE_LEVEL_INFO);

  try {
    //feed::FeedNewsManagerPrx fprx = CachedFeedNews2Adapter(uid);
    //feed_seq = fprx->GetLiveFeedData(uid, from, count);
    feed_seq = FeedNewsReplicaAdapter::instance().getLiveFeedData(uid, from, count);
  } catch (Ice::Exception & e) {
    //LOG_F(WARNING) << "GetLive() uid=" << uid << " error:" << e;
    MCE_WARN("FeedViewI::GetLive --> uid:" << uid << " error:" << e);
    feed_seq = NULL;
    //return kEmptyResult;
  }

  //LOG(INFO) << "GetLive() uid=" << uid << " count=" << feed_seq->data.size() << '/' << count;
  MCE_INFO("FeedViewI::GetLive --> uid:" << uid << " count=" << feed_seq->data.size() << '/' << count);


  //if (feed_seq->data.empty())
  //  return kEmptyResult;

  feed::HtmlResult ret;
  //ExpandUserFeeds(0, uid, name, render | WS_LIVE, feedSeq, from, &ret);
  ExpandUserFeeds(0, uid, name, render | WS_LIVE, feed_seq->data, from, &ret);
  return ret;
}

feed::HtmlResult FeedViewI::GetLiveByType(Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& types, Ice::Int from, Ice::Int count, const Ice::Current& cur) {
  feed::FeedDataResPtr feed_seq;
  MCE_INFO("FeedViewI::GetLiveByType --> uid:" << uid);
  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetLiveByType", InvokeClient::INVOKE_LEVEL_INFO);

  try {
    //feed::FeedNewsManagerPrx fprx = CachedFeedNews2Adapter(uid);
    //feed_seq = fprx->GetLiveFeedDataByType(uid, types, from, count);
    feed_seq = FeedNewsReplicaAdapter::instance().getLiveFeedDataByType(uid, types, from, count);
  } catch (Ice::Exception & e) {
    //LOG_F(WARNING) << "GetLiveByType() uid=" << uid << " error:" << e;
    MCE_WARN("FeedViewI::GetLiveByType --> uid:" << uid << " error:" << e);
    feed_seq = NULL;
    //return kEmptyResult;
  }

  //LOG(INFO) << "GetLiveByType() uid=" << uid << " count=" << feed_seq->data.size() << '/' << count;
  MCE_INFO("FeedViewI::GetLiveByType --> uid:" << uid << " count=" << feed_seq->data.size() << '/' << count);


  //if (feed_seq->data.empty())
  //  return kEmptyResult;

  feed::HtmlResult ret;
  //ExpandUserFeeds(0, uid, name, render | WS_LIVE, feedSeq, from, &ret);
  ExpandUserFeeds(0, uid, name, render | WS_LIVE, feed_seq->data, from, &ret);
  return ret;
}

feed::HtmlResult FeedViewI::GetLiveByStype(Ice::Int uid, const std::string& name
    , Ice::Int render, const ::xce::feed::TypeSeq& stypes, Ice::Int from, Ice::Int count, const Ice::Current& cur) {
  feed::FeedDataResPtr feed_seq;

  MCE_INFO("FeedViewI::GetLiveByStype --> uid:" << uid);
  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetLiveByStype", InvokeClient::INVOKE_LEVEL_INFO);
  try {
    //feed::FeedNewsManagerPrx fprx = CachedFeedNews2Adapter(uid);
    //feed_seq = fprx->GetLiveFeedDataByStype(uid, stypes, from, count);
    feed_seq = FeedNewsReplicaAdapter::instance().getLiveFeedDataByStype(uid, stypes, from, count);
  } catch (Ice::Exception & e) {
    //LOG_F(WARNING) << "GetLiveByStype() uid=" << uid << " error:" << e;
    MCE_WARN("FeedViewI::GetLiveByStype --> uid:" << uid << " error:" << e);
    feed_seq = NULL;
    //return kEmptyResult;
  }

  //LOG(INFO) << "GetLiveByStype() uid=" << uid << " count=" << feed_seq->data.size() << '/' << count;
  MCE_INFO("FeedViewI::GetLiveByStype --> uid:" << uid << " count=" << feed_seq->data.size() << '/' << count);


  if (feed_seq->data.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  //ExpandUserFeeds(0, uid, name, render | WS_LIVE, feedSeq, from, &ret);
  ExpandUserFeeds(0, uid, name, render | WS_LIVE, feed_seq->data, from, &ret);
  return ret;
}
feed::HtmlResult FeedViewI::GetMiniGroupFeedData(Ice::Int gid, const std::string& name,  Ice::Int render, Ice::Int memberid, Ice::Int begin, Ice::Int limit, const Ice::Current& cur) {

  feed::FeedDataSeq feed_seq;
  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetMiniGroupFeedData", InvokeClient::INVOKE_LEVEL_INFO);
  try {
    feed::FeedGroupManagerPrx fprx =  CachedFeedGroupAdapter(gid);
    feed_seq = fprx->GetFeedData(gid, memberid, begin, limit);
  } catch (Ice::Exception & e) {
    //LOG_F(WARNING) << "GetMiniGroupFeedData() gid=" << gid << " memberid:" << memberid << " error:" << e;
    MCE_WARN("GetMiniGroupFeedData() gid:" << gid << " memberid:" << memberid << " error:" << e);
    feed_seq.clear();
    //return kEmptyResult;
  }

  //LOG(INFO) << "GetMiniGroupFeedData() gid=" << gid << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit;
  MCE_INFO("GetMiniGroupFeedData() gid:" << gid << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit);

  if (feed_seq.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  ExpandUserFeeds(gid, memberid, name, render | WS_MINIFEED, feed_seq, begin, &ret);
  return ret;
}

feed::HtmlResult FeedViewI::GetMiniGroupFeedDataByType(Ice::Int gid, const std::string& name,  Ice::Int render,  Ice::Int memberid, const ::xce::feed::IntSeq& stypes, Ice::Int begin, Ice::Int limit, const Ice::Current& cur) {

  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetMiniGroupFeedDataByType", InvokeClient::INVOKE_LEVEL_INFO);
  feed::FeedDataSeq feed_seq;
  try {
    feed::FeedGroupManagerPrx fprx =  CachedFeedGroupAdapter(gid);
    feed_seq = fprx->GetFeedDataByType(gid, memberid, stypes, begin, limit);
  } catch (Ice::Exception & e) {
    //LOG_F(WARNING) << "GetMiniGroupFeedDataByType() gid=" << gid  << " memberid:" << memberid << " error:" << e;
    MCE_WARN("GetMiniGroupFeedDataByType() gid=" << gid  << " memberid:" << memberid << " error:" << e);
    feed_seq.clear();
    //return kEmptyResult;
  }

  //LOG(INFO) << "GetMiniGroupFeedDataByType() gid=" << gid  << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit;
  MCE_INFO("GetMiniGroupFeedDataByType() gid=" << gid  << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit);


  if (feed_seq.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  ExpandUserFeeds(gid, memberid, name, render | WS_MINIFEED, feed_seq, begin, &ret);
  return ret;
}

feed::HtmlResult FeedViewI::GetMiniGroupFeedDataByStype(Ice::Int gid, const std::string& name, Ice::Int render, Ice::Int memberid, const ::xce::feed::IntSeq& stypes, Ice::Int begin, Ice::Int limit, const Ice::Current& cur) {

  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetMiniGroupFeedDataByStype", InvokeClient::INVOKE_LEVEL_INFO);
  feed::FeedDataSeq feed_seq;
  try {
    feed::FeedGroupManagerPrx fprx =  CachedFeedGroupAdapter(gid);
    feed_seq = fprx->GetFeedDataByStype(gid, memberid, stypes, begin, limit);
  } catch (Ice::Exception & e) {
    //LOG_F(WARNING) << "GetMiniGroupFeedDataBySType() gid=" << gid  << " memberid:" << memberid << " error:" << e;
    MCE_WARN("GetMiniGroupFeedDataBySType() gid=" << gid  << " memberid:" << memberid << " error:" << e);
    feed_seq.clear();
    //return kEmptyResult;
  }

  //LOG(INFO) << "GetMiniGroupFeedDataBySType() gid=" << gid  << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit;
  MCE_INFO("GetMiniGroupFeedDataBySType() gid=" << gid  << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit);


  if (feed_seq.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  ExpandUserFeeds(gid, memberid, name, render | WS_MINIFEED, feed_seq, begin, &ret);
  return ret;
}


feed::HtmlResult FeedViewI::GetMiniGroupFeedData2CS(Ice::Int gid, const std::string& name,  Ice::Int render, Ice::Int memberid, Ice::Int begin, Ice::Int limit, bool isAdmin,  Ice::Int view, const Ice::Current& cur) {

  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetMiniGroupFeedData2CS", InvokeClient::INVOKE_LEVEL_INFO);
  feed::FeedDataSeq feed_seq;
  try {
    feed::FeedGroupManagerPrx fprx =  CachedFeedGroupAdapter(gid);
    feed_seq = fprx->GetFeedData(gid, memberid, begin, limit);
  } catch (Ice::Exception & e) {
    //LOG_F(WARNING) << "GetMiniGroupFeedData2() gid=" << gid << " memberid:" << memberid << " error:" << e;
    MCE_WARN("GetMiniGroupFeedData2CS() gid=" << gid << " memberid:" << memberid << " error:" << e);
    feed_seq.clear();
    //return kEmptyResult;
  }

  //LOG(INFO) << "GetMiniGroupFeedData2() gid=" << gid << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit << " isAdmin:" << isAdmin;
  MCE_INFO("GetMiniGroupFeedData2CS() gid=" << gid << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit << " isAdmin:" << isAdmin << " render:" << render << " view:" << view);


  if (feed_seq.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  ExpandUserFeeds(gid, memberid, name, render | WS_MINIFEED| WS_MINIGROUP, feed_seq, begin, &ret, isAdmin, view);
  return ret;
}


feed::HtmlResult FeedViewI::GetMiniGroupFeedData2(Ice::Int gid, const std::string& name,  Ice::Int render, Ice::Int memberid, Ice::Int begin, Ice::Int limit, bool isAdmin, const Ice::Current& cur) {

  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetMiniGroupFeedData2", InvokeClient::INVOKE_LEVEL_INFO);
  feed::FeedDataSeq feed_seq;
  try {
    feed::FeedGroupManagerPrx fprx =  CachedFeedGroupAdapter(gid);
    feed_seq = fprx->GetFeedData(gid, memberid, begin, limit);
  } catch (Ice::Exception & e) {
    //LOG_F(WARNING) << "GetMiniGroupFeedData2() gid=" << gid << " memberid:" << memberid << " error:" << e;
    MCE_WARN("GetMiniGroupFeedData2() gid=" << gid << " memberid:" << memberid << " error:" << e);
    feed_seq.clear();
    //return kEmptyResult;
  }

  //LOG(INFO) << "GetMiniGroupFeedData2() gid=" << gid << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit << " isAdmin:" << isAdmin;
  MCE_INFO("GetMiniGroupFeedData2() gid=" << gid << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit << " isAdmin:" << isAdmin << " render:" << render);


  if (feed_seq.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  ExpandUserFeeds(gid, memberid, name, render | WS_MINIFEED| WS_MINIGROUP, feed_seq, begin, &ret, isAdmin);
  return ret;
}

feed::HtmlResult FeedViewI::GetMiniGroupFeedDataByType2(Ice::Int gid, const std::string& name,  Ice::Int render,  Ice::Int memberid, const ::xce::feed::IntSeq& stypes, Ice::Int begin, Ice::Int limit, bool isAdmin, const Ice::Current& cur) {

  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetMiniGroupFeedDataByType2", InvokeClient::INVOKE_LEVEL_INFO);
  feed::FeedDataSeq feed_seq;
  try {
    feed::FeedGroupManagerPrx fprx =  CachedFeedGroupAdapter(gid);
    feed_seq = fprx->GetFeedDataByType(gid, memberid, stypes, begin, limit);
  } catch (Ice::Exception & e) {
    //LOG_F(WARNING) << "GetMiniGroupFeedDataByType2() gid=" << gid  << " memberid:" << memberid << " error:" << e;
    MCE_WARN("GetMiniGroupFeedDataByType2() gid=" << gid  << " memberid:" << memberid << " error:" << e);
    feed_seq.clear();
    //return kEmptyResult;
  }

  //LOG(INFO) << "GetMiniGroupFeedDataByType2() gid=" << gid  << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit << " isAdmin:" << isAdmin;
  MCE_INFO("GetMiniGroupFeedDataByType2() gid=" << gid  << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit << " isAdmin:" << isAdmin);


  if (feed_seq.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  ExpandUserFeeds(gid, memberid, name, render | WS_MINIFEED| WS_MINIGROUP, feed_seq, begin, &ret, isAdmin);
  return ret;
}

feed::HtmlResult FeedViewI::GetMiniGroupFeedDataByStype2(Ice::Int gid, const std::string& name, Ice::Int render, Ice::Int memberid, const ::xce::feed::IntSeq& stypes, Ice::Int begin, Ice::Int limit, bool isAdmin, const Ice::Current& cur) {

  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetMiniGroupFeedDataByStype2", InvokeClient::INVOKE_LEVEL_INFO);
  feed::FeedDataSeq feed_seq;
  try {
    feed::FeedGroupManagerPrx fprx =  CachedFeedGroupAdapter(gid);
    feed_seq = fprx->GetFeedDataByStype(gid, memberid, stypes, begin, limit);
  } catch (Ice::Exception & e) {
    //LOG_F(WARNING) << "GetMiniGroupFeedDataBySType2() gid=" << gid  << " memberid:" << memberid << " error:" << e;
    MCE_WARN("GetMiniGroupFeedDataBySType2() gid=" << gid  << " memberid:" << memberid << " error:" << e);
    feed_seq.clear();
    //return kEmptyResult;
  }

  //LOG(INFO) << "GetMiniGroupFeedDataBySType2() gid=" << gid  << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit << " isAdmin:" << isAdmin;
  MCE_INFO("GetMiniGroupFeedDataBySType2() gid=" << gid  << " memberid:" << memberid << " count=" << feed_seq.size() << '/' << limit << " isAdmin:" << isAdmin);

  if (feed_seq.empty())
    return kEmptyResult;

  feed::HtmlResult ret;
  ExpandUserFeeds(gid, memberid, name, render | WS_MINIFEED | WS_MINIGROUP, feed_seq, begin, &ret, isAdmin);
  return ret;
}

static int ExpandNewFeedOnly(Ice::Int owner, Ice::Int uid, const std::string& name, Ice::Int render
    , feed::FeedDataSeq & feed_seq, Ice::Int view = 0, Ice::Int begin = 0) {
  if (feed_seq.empty()) {
    return -1;
  }

  // 得到“赞” ilike 信息
  ::xce::ilikeit::LikeInfoSeq likeseq;
  std::vector<char> vbool(0, feed_seq.size()); // 有些类型并不需要 zan

  int feed_count = begin;
  int new_feed_count = 0;
  std::string stat;
  std::string exposeTime = lexical_cast<std::string>(time(0));
    xce::adapter::userurl::UserUrlInfoPtr info;
  string tinyimg = "NoTiny";
  {
    try {
      info = xce::adapter::userurl::UserUrlAdapter::instance().getUserUrl(uid);
      if(info) {
        tinyimg = info->tinyUrl();
      }
    } catch (Ice::Exception& e) {
      MCE_WARN("ExpandUserFeeds --> uid:" << uid <<" getTinyImgFeedData exception:"<< e);
    } catch (std::exception& e) {
      MCE_WARN("ExpandUserFeeds --> uid:" << uid <<" getTinyImgFeedData exception:"<< e.what());
    }
  }
  cwf::User u(name, uid, tinyimg);
  //cwf::User u(name, uid);
  for (size_t i = 0; i<feed_seq.size(); ++i) {
    std::string stat_log;
    if (WeiredType(feed_seq[i]->type).version > 0) {
      const vipinfo::VipFeedDataSeq emptyVipData;
      std::string output;
      bool f = ExpandFeedData(feed_seq[i].get(), &u, owner, (WeiredSource)render, NULL, feed_count, emptyVipData, &output, stat_log, exposeTime, false, view);
      if (f) {
        stat += stat_log;
        stat += "\n";
        ++feed_count;
        ++new_feed_count;
        feed_seq[i]->xml = output;
      }
      continue;
    }

    std::ostringstream os;
    os << "stype:" << lexical_cast<std::string>(WeiredType(feed_seq[i]->type).type);
    os << " owner:" << owner;
    os << " uid:" << uid;
    os << " f_id:" << feed_seq[i]->feed;
    os << " rank:" << feed_count;
    os << " actor:" << feed_seq[i]->actor;
    os << " source:" << feed_seq[i]->source;
    os << " create_time:" << feed_seq[i]->time;
    os << " type:" << feed_seq[i]->type;
    os << " platform:" << view;
    os << " view:" << view;
    os << " stat_id: level: origin_url: expose_time:" + exposeTime;
    os << " wiki_word:";
    os << " weired_source:" << render;
    stat += os.str() + "\n";
    ++feed_count;
  }

  try {
    std::vector<std::string> cate;
    cate.push_back("feed");
    cate.push_back("stat");
    //FeedStatLogAdapter::instance().AddLog(cate, stat);
    FeedViewStatLog::instance().AddLog(cate, stat);
  } catch (Ice::Exception& e) {
      MCE_WARN("ExpandNewFeedOnly --> uid:" << uid << " FeedStatLogAdapter err:" << e);
  }

  MCE_INFO("ExpandNewFeedOnly --> uid:" << uid << " expand new feeds:" << new_feed_count << "/" << feed_seq.size());
  return 0;
}

feed::CSTplCheckRes FeedViewI::CheckCSTemplate(const std::string& tpl, const Ice::Current&) {
  feed::CSTplCheckRes res;
  int code = xce::notify::CSTemplate::CheckTpl(tpl, &res.reason);
  res.code = code;
  MCE_INFO("check tpl ------" << tpl << "--------- res : " << code << " " << res.reason);
  return res;
}

feed::FeedDataResPtr FeedViewI::GetHotDataByStype(::Ice::Int uid, const std::string& name, ::Ice::Int render,
    const ::xce::feed::TypeSeq& stypes,
    ::Ice::Int begin, ::Ice::Int limit,
    const ::Ice::Current& cur) {
  feed::FeedDataResPtr feed_res;
  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetHotDataByStype", InvokeClient::INVOKE_LEVEL_INFO);

  try {
    feed_res = FeedNewsReplicaAdapter::instance().GetHotFeedDataByStype(uid, stypes, begin, limit);
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedNewsReplicaAdapter::GetHotFeedDataByStype() error : " << e);
    return NULL;
  }

  if (feed_res) {
    feed::FeedDataSeq & feed_seq  = feed_res->data;
    ExpandNewFeedOnly(0, uid, name, render, feed_seq);
  }

  return feed_res;
}

feed::FeedDataResPtr FeedViewI::GetLiveDataByStype(::Ice::Int uid, const std::string& name, ::Ice::Int render,
    const ::xce::feed::TypeSeq& stypes,
    ::Ice::Int begin, ::Ice::Int limit,
    const ::Ice::Current& cur) {
  feed::FeedDataResPtr feed_res;

  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetLiveDataByStype", InvokeClient::INVOKE_LEVEL_INFO);
  try {
    feed_res = FeedNewsReplicaAdapter::instance().getLiveFeedDataByStype(uid, stypes, begin, limit);
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedNewsReplicaAdapter::GetHotFeedDataByStype() error : " << e);
    return NULL;
  }

  if (feed_res) {
    feed::FeedDataSeq & feed_seq  = feed_res->data;
    ExpandNewFeedOnly(0, uid, name, render, feed_seq);
  }

  return feed_res;
}

feed::FeedDataSeq FeedViewI::GetMiniDataByStype(::Ice::Int uid, const std::string& name, ::Ice::Int render,
    const ::xce::feed::TypeSeq& stypes,
    ::Ice::Int begin, ::Ice::Int limit,
    const ::Ice::Current& cur) {
  feed::FeedDataSeq feed_seq;

  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetMiniDataByStype", InvokeClient::INVOKE_LEVEL_INFO);
  try {
    feed_seq = FeedMiniReplicaAdapter::instance().getFeedDataByStype(uid, stypes, begin, limit);
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedNewsReplicaAdapter::GetHotFeedDataByStype() error : " << e);
  }

  ExpandNewFeedOnly(0, uid, name, render, feed_seq);

  return feed_seq;
}


feed::FeedDataResPtr FeedViewI::GetHotDataByStype2(::Ice::Int uid, const std::string& name, ::Ice::Int render,
    const ::xce::feed::TypeSeq& stypes,
    ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view,
    const ::Ice::Current& cur) {
  feed::FeedDataResPtr feed_res;

  ostringstream os;
  os << "FeedViewI::GetHotDataByStype2 --> uid:" << uid << " begin:" << begin << " limit:" << limit;
  InvokeClient tm = InvokeClient::create(cur, os.str(), InvokeClient::INVOKE_LEVEL_INFO);
  try {
    feed_res = FeedNewsReplicaAdapter::instance().GetHotFeedDataByStype(uid, stypes, begin, limit);
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedViewI::GetHotDataByStype2 --> uid: " << uid << " error : " << e);
    return NULL;
  }

  if (feed_res) {
    feed::FeedDataSeq & feed_seq  = feed_res->data;
    MCE_INFO("FeedViewI::GetHotDataByStype2 --> uid:" << uid << " feedSeq size:" << feed_seq.size());
    ExpandNewFeedOnly(0, uid, name, render, feed_seq, view, begin);
  }

  return feed_res;
}

feed::FeedDataResPtr FeedViewI::GetLiveDataByStype2(::Ice::Int uid, const std::string& name, ::Ice::Int render,
    const ::xce::feed::TypeSeq& stypes,
    ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view,
    const ::Ice::Current& cur) {
  feed::FeedDataResPtr feed_res;

  ostringstream os;
  os << "FeedViewI::GetLiveDataByStype2 --> uid:" << uid;
  InvokeClient tm = InvokeClient::create(cur, os.str(), InvokeClient::INVOKE_LEVEL_INFO);
  try {
    feed_res = FeedNewsReplicaAdapter::instance().getLiveFeedDataByStype(uid, stypes, begin, limit);
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedViewI::GetLiveDataByStype2 --> uid: " << uid << " error : " << e);
    return NULL;
  }

  if (feed_res) {
    feed::FeedDataSeq & feed_seq  = feed_res->data;
    MCE_INFO("FeedViewI::GetLiveDataByStype2 --> uid:" << uid << " begin:" << begin << " limit:" << limit << " feedSeq size:" << feed_seq.size());
    ExpandNewFeedOnly(0, uid, name, render, feed_seq, view, begin);
  }

  return feed_res;
}

feed::FeedDataByteResPtr FeedViewI::GetLiveDataByStypeCompressed(::Ice::Int uid, const std::string& name, ::Ice::Int render,
      const ::xce::feed::TypeSeq& stypes,
      ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view,
      const ::Ice::Current& cur) {
  feed::FeedDataResPtr feed_res;
  feed::FeedDataByteResPtr feedbyte_res = NULL;

  ostringstream os;
  os << "FeedViewI::GetLiveDataByStypeCompressed --> uid:" << uid << " begin:" << begin << " limit:" << limit;
  InvokeClient tm = InvokeClient::create(cur, os.str(), InvokeClient::INVOKE_LEVEL_INFO);

  feed_res = GetLiveDataByStype2(uid, name, render, stypes, begin, limit, view, cur);
  if (feed_res) {
    feed::FeedDataSeq & feed_seq = feed_res->data;
    feedbyte_res = new FeedDataByteRes();
    feedbyte_res->UnviewedCount = feed_res->UnviewedCount;

    size_t len = 1, len2 = 1;
    feed::FeedDataBytePtr temp;
    for(size_t i = 0;i < feed_seq.size(); ++i) {
      temp = new FeedDataByte();
      temp->feed = feed_seq[i]->feed;
      temp->source = feed_seq[i]->source;
      temp->actor = feed_seq[i]->actor;
      temp->weight = feed_seq[i]->weight;
      temp->type = feed_seq[i]->type;
      temp->time = feed_seq[i]->time;
      //在此压缩数据
      qlz_state_compress *state_compress = (qlz_state_compress *)malloc(sizeof(qlz_state_compress));
      len = feed_seq[i]->xml.size();
      char * dst = (char*) malloc(len + 400);
      len2 = qlz_compress((void *)feed_seq[i]->xml.data(), dst, len, state_compress);
      temp->xml.resize(len2);
      temp->xml.assign(dst,dst+len2);
      free(state_compress);
      free(dst);
      feedbyte_res->data.push_back(temp);
    }
  }
  return feedbyte_res;
}

feed::FeedDataSeq FeedViewI::GetMiniDataByStype2(::Ice::Int uid, const std::string& name, ::Ice::Int render,
    const ::xce::feed::TypeSeq& stypes,
    ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view,
    const ::Ice::Current& cur) {
  feed::FeedDataSeq feed_seq;

  ostringstream os;
  os << "FeedViewI::GetMiniDataByStype2 --> uid:" << uid << " begin:" << begin << " limit:" << limit;
  InvokeClient tm = InvokeClient::create(cur, os.str(), InvokeClient::INVOKE_LEVEL_INFO);
  try {
    feed_seq = FeedMiniReplicaAdapter::instance().getFeedDataByStype(uid, stypes, begin, limit);
    MCE_INFO("FeedViewI::GetMiniDataByStype2 --> uid:" << uid << " feedSeq size:" << feed_seq.size());
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedViewI::GetMiniDataByStype2 --> uid: " << uid << " error : " << e);
    return feed::FeedDataSeq();
  }

  ExpandNewFeedOnly(0, uid, name, render, feed_seq, view, begin);

  return feed_seq;
}

feed::FeedDataByteSeq FeedViewI::GetMiniDataByStypeCompressed(::Ice::Int uid, const std::string& name, ::Ice::Int render,
      const ::xce::feed::TypeSeq& stypes,
      ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view,
      const ::Ice::Current& cur) {
  feed::FeedDataSeq feed_seq;
  feed::FeedDataByteSeq feedbyte_seq;

  ostringstream os;
  os << "FeedViewI::GetMiniDataByStypeCompressed --> uid:" << uid << " begin:" << begin << " limit:" << limit;
  InvokeClient tm = InvokeClient::create(cur, os.str(), InvokeClient::INVOKE_LEVEL_INFO);

  feed_seq = GetMiniDataByStype2(uid, name, render, stypes, begin, limit, view, cur);
  if (!feed_seq.empty()) {
    feed::FeedDataBytePtr temp;
    size_t len, len2;
    for(size_t i = 0;i < feed_seq.size(); ++i) {
      temp = new FeedDataByte();
      temp->feed = feed_seq[i]->feed;
      temp->source = feed_seq[i]->source;
      temp->actor = feed_seq[i]->actor;
      temp->weight = feed_seq[i]->weight;
      temp->type = feed_seq[i]->type;
      temp->time = feed_seq[i]->time;
      //在此压缩数据
      qlz_state_compress *state_compress = (qlz_state_compress *)malloc(sizeof(qlz_state_compress));
      len = feed_seq[i]->xml.size();
      char * dst = (char*) malloc(len + 400);
      len2 = qlz_compress((void *)feed_seq[i]->xml.data(), dst, len, state_compress);
      temp->xml.resize(len2);
      temp->xml.assign(dst,dst+len2);
      free(state_compress);
      free(dst);
      feedbyte_seq.push_back(temp);
    }
  }
  return feedbyte_seq;
}

feed::FeedDataSeq FeedViewI::GetFavorite2(::Ice::Int uid, const std::string& name
    , ::Ice::Int render, ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view, const Ice::Current& cur) {
  feed::FeedDataSeq feed_seq;

  ostringstream os;
  os << "FeedViewI::GetFavorite2 --> uid:" << uid << " begin:" << begin << " limit:" << limit;
  InvokeClient tm = InvokeClient::create(cur, os.str(), InvokeClient::INVOKE_LEVEL_INFO);
  try {
    feed_seq = feed::FeedFocusAdapter::instance().GetFocusFeedData(uid, begin, limit);
    MCE_INFO("FeedViewI::GetFavorite2 --> uid:" << uid << " feedseq size:" << feed_seq.size());
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedViewI::GetFavorite2 --> uid:" << uid << " error:" << e);
    return feed::FeedDataSeq();
  }

  ExpandNewFeedOnly(0, uid, name, render, feed_seq, view, begin);

  return feed_seq;
}

feed::FeedDataSeq FeedViewI::GetFavorite2ByStype(::Ice::Int uid, const std::string& name
    , ::Ice::Int render, const ::xce::feed::TypeSeq& stypes, ::Ice::Int begin, ::Ice::Int limit, ::Ice::Int view, const Ice::Current& cur) {
  feed::FeedDataSeq feed_seq;

  ostringstream os;
  os << "FeedViewI::GetFavorite2ByStype --> uid:" << uid << "begin: " << begin << "limit: " << limit;
  InvokeClient tm = InvokeClient::create(cur, os.str(), InvokeClient::INVOKE_LEVEL_INFO);
  try {
    feed_seq = feed::FeedFocusAdapter::instance().GetFocusFeedDataByStype(uid, begin, limit, stypes);
    MCE_INFO("FeedViewI::GetFavorite2ByStype --> uid:" << uid << "feedseq size: " << feed_seq.size());
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedViewI::GetFavorite2ByStype --> uid:" << uid << " error:" << e);
    return feed::FeedDataSeq();
  }

  ExpandNewFeedOnly(0, uid, name, render, feed_seq, view, begin);

  return feed_seq;
}

feed::FeedDataSeq FeedViewI::GetByIds2View(Ice::Int uid, const std::string& name, Ice::Int render
    , const feed::FeedIdSeq & fids, Ice::Int view, const Ice::Current& cur) {
  feed::FeedDataSeq feed_seq;

  InvokeClient tm = InvokeClient::create(cur, "FeedViewI::GetByIds2View", InvokeClient::INVOKE_LEVEL_INFO);
  if (fids.empty()) {
    MCE_INFO("FeedViewI::GetByIds2View --> uid:" << uid << " size=0");
    return feed_seq;
  }

  try {
    //feed_seq = FeedNewsReplicaAdapter::instance().GetFeedDataByIds(fids);
    GetDataByIds(fids, feed_seq, uid);
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedViewI::GetByIds2View --> uid:" << uid << " "<< e);
    return feed::FeedDataSeq();
  }

  ExpandNewFeedOnly(0, uid, name, render, feed_seq, view);
  return feed_seq;
}

void FeedViewI::CheckLoad(const vector<Ice::Long> & fids, FeedContentDict & dict, int uid) {
  vector<Ice::Long> miss_fids;
  for(vector<Ice::Long>::const_iterator it = fids.begin(); it != fids.end(); ++ it) {
    if(!dict.count(*it)) {
      miss_fids.push_back(*it);
    }
  }
  if(miss_fids.empty()) return;

  FeedContentDict dict_loader;
  try {
    dict_loader= FeedMemcLoaderAdapter::instance().GetFeedDict(miss_fids, 100);
  } catch (Ice::Exception& e) {
    MCE_WARN("FeedViewI::CheckLoad --> GetFeedDict uid:" << uid << " size:" << miss_fids.size() << " err, " << e);
    return;
  }
  MCE_INFO("FeedViewI::CheckLoad --> GetFeedDict uid:" << uid << " size:" << miss_fids.size() << " res:" << dict_loader.size());
  if(!dict_loader.empty()) {
    dict.insert(dict_loader.begin(), dict_loader.end());
  }
}

void FeedViewI::GetDataByIds(const vector<Ice::Long> & fids, feed::FeedDataSeq & feed_seq, int uid) {
  MCE_INFO("FeedViewI::GetDataByIds --> uid:" << uid << " fids size: " << fids.size());
  if(fids.empty()) {
    return;
  }

  FeedContentDict contentMap = FeedContentAdapter::instance().getFeedDict(fids);
  MCE_INFO("FeedViewI::GetDataByIds --> uid:" << uid << " FeedContentDict size: " << contentMap.size());
  CheckLoad(fids, contentMap, uid);

  for (std::vector<Ice::Long>::const_iterator it = fids.begin(); it != fids.end(); ++it) {
    FeedContentDict::iterator dictIt = contentMap.find(*it);
    if(dictIt != contentMap.end() && dictIt->second) {
      if((dictIt->second)->reply) {
        ostringstream xml;
        xml << ((dictIt->second)->data)->xml;
        xml << ((dictIt->second)->reply)->oldReply << ((dictIt->second)->reply)->newReply
          << "<reply_count>" << ((dictIt->second)->reply)->count << "</reply_count>";
        ((dictIt->second)->data)->xml = xml.str();
      }
      feed_seq.push_back((dictIt->second)->data);
      } else {
        try {
//        feed::timeline::TimeLineFeedReplicaAdapter::instance().SetReadMiniById(uid, -1, *it);
        } catch (Ice::Exception &e) {
          MCE_WARN("FeedViewI::GetDataByIds --> uid:" << uid << " feedid: " << *it << " error: " << e);
        } catch (std::exception & e) {
          MCE_WARN("FeedViewI::GetDataByIds --> uid:" << uid << " feedid: " << *it << " error: " << e.what());
        }
     }
  }
  MCE_INFO("FeedViewI::GetDataByIds --> uid:" << uid << " FeedDataSeq size: " << feed_seq.size());
}

feed::FeedDataSeq FeedViewI::GetGreaterThanFeedidWithUseridByType(::Ice::Int appid, ::Ice::Long feedid, ::Ice::Int limit,
  ::Ice::Int uid, const ::xce::feed::TypeSeq& types, ::Ice::Int render, Ice::Int view, const ::Ice::Current& cur) {
  FeedDataSeq feed_seq;
  FeedMetaSeq meta;

  ostringstream os;
  os << "FeedViewI::GetGreaterThanFeedidWithUseridByType --> appid:" << appid << " uid:" << uid << " feedid:" << feedid << " limit:" << limit;
  InvokeClient tm = InvokeClient::create(cur, os.str(), InvokeClient::INVOKE_LEVEL_INFO);
  try {
    if(appid >= 0) {
      meta = FeedItemCacheByAppidReplicaAdapter::instance().getGreaterThanFeedidWithUseridByType(appid, feedid, limit, uid, types);
    } else {
      feed::IntSeq friends;
      friends = buddy::adapter::BuddyByOnlineTimeCacheAdapter::instance().getFriendList(uid, 1000);
      meta = FeedItemCacheByUseridReplicaAdapter::instance().getBiggerThanFeedid(friends, feedid, 0, limit, types, feed::StrSeq());
    }
    MCE_INFO("FeedViewI::GetGreaterThanFeedidWithUseridByType --> uid:" << uid << " FeedMetaSeq size: " << meta.size());
    std::vector<Ice::Long> feedids;
    for(FeedMetaSeq::const_iterator it = meta.begin(); it != meta.end(); ++ it) {
      feedids.push_back(it->feedid);
    }
    GetDataByIds(feedids, feed_seq, uid);
  } catch(Ice::Exception& e){
    MCE_WARN("FeedViewI::GetGreaterThanFeedidWithUseridByType --> uid:" << uid << " error: " << e);
  } catch (std::exception & e) {
    MCE_WARN("FeedViewI::GetGreaterThanFeedidWithUseridByType --> uid:" << uid << " error: " << e.what());
  }

  ExpandNewFeedOnly(0, uid, "", render, feed_seq, view);

  return feed_seq;
}

feed::FeedDataByteSeq FeedViewI::GetUnlimitedDataByStype(::Ice::Int uid, ::Ice::Int begin, ::Ice::Int limit,
      const ::xce::feed::TypeSeq& stypes,::Ice::Int render, ::Ice::Int view,
      const ::Ice::Current& cur) {
  feed::FeedDataSeq feed_seq;
  feed::FeedDataByteSeq feedbyte_seq;
  FeedMetaSeq meta;

  ostringstream os;
  os << "FeedViewI::GetUnlimitedDataByStype --> uid:" << uid << " begin:" << begin << " limit:" << limit;
  InvokeClient tm = InvokeClient::create(cur, os.str(), InvokeClient::INVOKE_LEVEL_INFO);

  try {
    meta = FeedItemWrapperReplicaAdapter::instance().getByTypes(uid, begin, limit, stypes);
    MCE_INFO("FeedViewI::GetUnlimitedDataByStype --> uid:" << uid << " FeedMetaSeq size: " << meta.size());
    std::vector<Ice::Long> feedids;
    for(FeedMetaSeq::const_iterator it = meta.begin(); it != meta.end(); ++ it) {
      feedids.push_back(it->feedid);
    }
    GetDataByIds(feedids, feed_seq, uid);
  } catch(Ice::Exception& e){
    MCE_WARN("FeedViewI::GetUnlimitedDataByStype --> uid:" << uid << " error: " << e);
  } catch (std::exception & e) {
    MCE_WARN("FeedViewI::GetUnlimitedDataByStype --> uid:" << uid << " error: " << e.what());
  }

  ExpandNewFeedOnly(0, uid, "", render, feed_seq, view, begin);
  if (!feed_seq.empty()) {
     feed::FeedDataBytePtr temp;
     size_t len, len2;
     for(size_t i = 0;i < feed_seq.size(); ++i) {
       temp = new FeedDataByte();
       temp->feed = feed_seq[i]->feed;
       temp->source = feed_seq[i]->source;
       temp->actor = feed_seq[i]->actor;
       temp->weight = feed_seq[i]->weight;
       temp->type = feed_seq[i]->type;
       temp->time = feed_seq[i]->time;
       //在此压缩数据
       qlz_state_compress *state_compress = (qlz_state_compress *)malloc(sizeof(qlz_state_compress));
       len = feed_seq[i]->xml.size();
       char * dst = (char*) malloc(len + 400);
       len2 = qlz_compress((void *)feed_seq[i]->xml.data(), dst, len, state_compress);
       temp->xml.resize(len2);
       temp->xml.assign(dst,dst+len2);
       free(state_compress);
       free(dst);
       feedbyte_seq.push_back(temp);
     }
   }
  MCE_INFO("FeedViewI::GetUnlimitedDataByStype --> uid:" << uid << " FeedDataByteSeq size: " << feedbyte_seq.size());

  return feedbyte_seq;
}

feed::HtmlResult FeedViewI::GetBySourceStypeAndActor(Ice::Int uid, const std::string& name, Ice::Long source, Ice::Int stype, Ice::Int actor, Ice::Int render, Ice::Int view, const Ice::Current& current) {
  ostringstream os;
  os << "FeedViewI::GetBySourceStypeAndActor --> uid: " << uid << " source: " << source << " stype:" << stype << " actor:" << actor << " render:" << render << " view:" << view;
  InvokeClient tm = InvokeClient::create(current, os.str(), InvokeClient::INVOKE_LEVEL_INFO);

  feed::FeedDataSeq feed_seq;
  struct timeval start, end;

  try {
    std::vector<Ice::Long> feedids;
    Ice::Long feedid =  FeedAssistantNReplicaAdapter::instance().getFeedId(source, stype, actor);
    feedids.push_back(feedid);
    GetDataByIds(feedids, feed_seq, uid);
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedViewI::GetBySourceStypeAndActor -->  uid:" << uid << " exception:" << e);
  }

  if (feed_seq.empty())
    return kEmptyResult;

  if(view == 6) {
    for(feed::FeedDataSeq::const_iterator it = feed_seq.begin(); it != feed_seq.end(); ++it) {
      if(((*it)->type & 0xffff) == 1205) { //目前timeline下只有1205支持编辑
        bool isHighlight = false;
        try {
          isHighlight = feed::timeline::TimeLineFeedReplicaAdapter::instance().IsWideScreen((*it)->actor, -1, (*it)->feed);
        } catch (Ice::Exception & e) {
          MCE_WARN("FeedViewI::GetBySourceStypeAndActor -->  uid:" << uid << " exception:" << e);
        }
        if(isHighlight) {
          (*it)->type = ((unsigned int)(*it)->type) | 0x1000000;
        } else {
          (*it)->type = ((unsigned int)(*it)->type) & 0xFEFFFFFF;
        }
      }
    }
  }

  feed::HtmlResult ret;
  gettimeofday(&start, NULL);
  int expand_feed_suc = ExpandUserFeeds(actor, uid, name, render, feed_seq, 0, &ret, false, view);
  gettimeofday(&end, NULL);
  double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  MCE_INFO("FeedViewI::GetBySourceStypeAndActor -->  uid: " << uid << " source: " << source << " stype:" << stype << " actor:" << actor << " expand_dur:" << timeuse / 1000000 << " expand_feed_suc:" << expand_feed_suc);

  return ret;
}

feed::timeline::UidYear FeedViewI::GetTimelineYearMonth(Ice::Int uid, const Ice::Current& current) {
  ostringstream os;
  os << "FeedViewI::GetTimelineYearMonth -->  uid: " << uid;
  InvokeClient tm = InvokeClient::create(current, os.str(), InvokeClient::INVOKE_LEVEL_INFO);

  feed::timeline::UidYear uidYearMonth;
  try {
    uidYearMonth = feed::timeline::TimeLineFeedReplicaAdapter::instance().Query(uid);
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedViewI::GetTimelineYearMonth -->  uid:" << uid << " exception:" << e);
  }
  return uidYearMonth;
}
feed::HtmlResult FeedViewI::GetTimelineData(Ice::Int owner, Ice::Int uid, const std::string& name, Ice::Int year, Ice::Int month, Ice::Int render,
      Ice::Int begin, Ice::Int limit, const Ice::Current& current) {
  ostringstream os;
  os << "FeedViewI::GetTimelineData --> owner: " << owner << " uid: " << uid << " year:" << year << " month:" << month << " begin:" << begin << " limit:" << limit;
  InvokeClient tm = InvokeClient::create(current, os.str(), InvokeClient::INVOKE_LEVEL_INFO);

  feed::FeedDataSeq feed_seq;
  feed::timeline::TimeLineFeedResult tlfeedres;
  struct timeval start, end;

  Ice::Int stype_array[] = {102, 103, 104, 107, 110, 502, 504, 601, 701, 702, 709, 801, 808,
                            1201, 1205, 1206, 2003, 2004, 2005, 2006, 2008, 2009,
                            2012, 2013, 2015, 2016, 2032, 2035, 2036, 2038, 2039, 8210, 8211,
                            8301, 9705, 9801, 9802, 9803, 9804};
  vector<Ice::Int> stypes(stype_array, stype_array + sizeof(stype_array)/sizeof(Ice::Int));

  try {
    gettimeofday(&start, NULL);
    std::map<Ice::Long, unsigned int> feeds;
    std::vector<Ice::Long> feedids;
    tlfeedres = feed::timeline::TimeLineFeedReplicaAdapter::instance().QueryFeedInfoByStype(owner, year, month, stypes, begin, limit);

    unsigned int appendIndex = 0;
    for(feed::timeline::TimeLineFeedInfoSeq::const_iterator it1 = tlfeedres.feeds.begin(); it1 != tlfeedres.feeds.end(); ++it1, ++appendIndex) {
      for(MyUtil::LongSeq::const_iterator it2 = (it1->feeds).begin(); it2 != (it1->feeds).end(); ++it2) {
        feeds.insert(std::make_pair(*it2, ((unsigned int)(it1->flag) | (appendIndex << 1))));
        feedids.push_back(*it2);
      }
    }
    feed::FeedDataSeq tmp_feed_seq;
    GetDataByIds(feedids, tmp_feed_seq, owner);

    int currentAppendIndex = -1, feed_seq_index = -1;
    for(feed::FeedDataSeq::const_iterator it = tmp_feed_seq.begin(); it != tmp_feed_seq.end(); ++it) {
      std::map<Ice::Long, unsigned int>::const_iterator iter = feeds.find((*it)->feed);
      if(iter != feeds.end()) {
        if((iter->second >> 1) != currentAppendIndex) {
          ++feed_seq_index;
          feed_seq.push_back((*it));
          if((iter->second & 0x1) == 1) {
            feed_seq[feed_seq_index]->type = ((unsigned int)feed_seq[feed_seq_index]->type) | 0x1000000;
          } else {
            feed_seq[feed_seq_index]->type = ((unsigned int)feed_seq[feed_seq_index]->type) & 0xFEFFFFFF;
          }
        } else {
          MCE_INFO("FeedViewI::GetTimelineData --> feed append. owner: " << owner << " first feed:" << feed_seq[feed_seq_index]->feed << " append feed:" << (*it)->feed);
          feed_seq[feed_seq_index]->xml += (*it)->xml;
        }
        currentAppendIndex = (iter->second >> 1);
      }
    }
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedViewI::GetTimelineData -->  owner:" << owner << " uid:" << uid << " exception:" << e);
  }

  gettimeofday(&end, NULL);
  double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

  MCE_INFO("FeedViewI::GetTimelineData -->  owner:" << owner << " uid:" << uid << " count:" << feed_seq.size() << '/' << limit << " ice call dur:" << timeuse / 1000000);

  if (feed_seq.empty())
    return kEmptyResult;
  for(feed::FeedDataSeq::const_iterator it = feed_seq.begin(); it != feed_seq.end(); ++it) {
    MCE_INFO("timeline get feed: " << (*it)->feed);
  }

  feed::HtmlResult ret;
  gettimeofday(&start, NULL);
  int expand_feed_suc = ExpandUserFeeds(owner, uid, name, render|WS_TIMELINE, feed_seq, begin, &ret, false, 6);
  gettimeofday(&end, NULL);
  timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  MCE_INFO("FeedViewI::GetTimelineData -->  owner:" << owner << " uid:" << uid << " expand dur:" << timeuse / 1000000 << " expand_feed_suc:" << expand_feed_suc);

  if(begin == 0 && ret.size() > 0) {
    ostringstream os;
    os << "<input type=\"hidden\" value=\"" << tlfeedres.size << "\" />" << std::endl;
    ret[0] = os.str() + ret[0];
  }

  return ret;
}

feed::HtmlResult FeedViewI::GetTimelineDataWithAdminByStypes(Ice::Int owner, Ice::Int uid, const std::string& name, Ice::Int year, Ice::Int month, Ice::Int render,
      Ice::Int begin, Ice::Int limit, bool isAdmin, const ::xce::feed::TypeSeq& stypes, const Ice::Current& current) {
  ostringstream os;
  os << "FeedViewI::GetTimelineData --> owner: " << owner << " uid: " << uid << " year:" << year << " month:" << month << " begin:" << begin << " limit:" << limit << " isAdmin:" << isAdmin;
  InvokeClient tm = InvokeClient::create(current, os.str(), InvokeClient::INVOKE_LEVEL_INFO);

  feed::FeedDataSeq feed_seq;
  feed::timeline::TimeLineFeedResult tlfeedres;
  struct timeval start, end;

  Ice::Int stype_array[] = {102, 103, 104, 107, 110, 502, 504, 601, 701, 702, 709, 801, 808,
                            1201, 1205, 1206, 2003, 2004, 2005, 2006, 2008, 2009,
                            2012, 2013, 2015, 2016, 2032, 2035, 2036, 2038, 2039, 8210, 8211,
                            8301, 9705, 9801, 9802, 9803, 9804};
  //vector<Ice::Int> stypes(stype_array, stype_array + sizeof(stype_array)/sizeof(Ice::Int));

  try {
    gettimeofday(&start, NULL);
    std::map<Ice::Long, unsigned int> feeds;
    std::vector<Ice::Long> feedids;
    tlfeedres = feed::timeline::TimeLineFeedReplicaAdapter::instance().QueryFeedInfoByStype(owner, year, month, stypes, begin, limit);

    unsigned int appendIndex = 0;
    for(feed::timeline::TimeLineFeedInfoSeq::const_iterator it1 = tlfeedres.feeds.begin(); it1 != tlfeedres.feeds.end(); ++it1, ++appendIndex) {
      for(MyUtil::LongSeq::const_iterator it2 = (it1->feeds).begin(); it2 != (it1->feeds).end(); ++it2) {
        feeds.insert(std::make_pair(*it2, ((unsigned int)(it1->flag) | (appendIndex << 1))));
        feedids.push_back(*it2);
      }
    }
    feed::FeedDataSeq tmp_feed_seq;
    GetDataByIds(feedids, tmp_feed_seq, owner);

    int currentAppendIndex = -1, feed_seq_index = -1;
    for(feed::FeedDataSeq::const_iterator it = tmp_feed_seq.begin(); it != tmp_feed_seq.end(); ++it) {
      std::map<Ice::Long, unsigned int>::const_iterator iter = feeds.find((*it)->feed);
      if(iter != feeds.end()) {
        if((iter->second >> 1) != currentAppendIndex) {
          ++feed_seq_index;
          feed_seq.push_back((*it));
          if((iter->second & 0x1) == 1) {
            feed_seq[feed_seq_index]->type = ((unsigned int)feed_seq[feed_seq_index]->type) | 0x1000000;
          } else {
            feed_seq[feed_seq_index]->type = ((unsigned int)feed_seq[feed_seq_index]->type) & 0xFEFFFFFF;
          }
        } else {
          MCE_INFO("FeedViewI::GetTimelineData --> feed append. owner: " << owner << " first feed:" << feed_seq[feed_seq_index]->feed << " append feed:" << (*it)->feed);
          feed_seq[feed_seq_index]->xml += (*it)->xml;
        }
        currentAppendIndex = (iter->second >> 1);
      }
    }
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedViewI::GetTimelineData -->  owner:" << owner << " uid:" << uid << " exception:" << e);
  }

  gettimeofday(&end, NULL);
  double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

  MCE_INFO("FeedViewI::GetTimelineData -->  owner:" << owner << " uid:" << uid << " count:" << feed_seq.size() << '/' << limit << " ice call dur:" << timeuse / 1000000);

  if (feed_seq.empty())
    return kEmptyResult;
  for(feed::FeedDataSeq::const_iterator it = feed_seq.begin(); it != feed_seq.end(); ++it) {
    MCE_INFO("timeline get feed: " << (*it)->feed);
  }

  feed::HtmlResult ret;
  gettimeofday(&start, NULL);
  int expand_feed_suc = ExpandUserFeeds(owner, uid, name, render|WS_TIMELINE, feed_seq, begin, &ret, isAdmin, 6);
  gettimeofday(&end, NULL);
  timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  MCE_INFO("FeedViewI::GetTimelineData -->  owner:" << owner << " uid:" << uid << " expand dur:" << timeuse / 1000000 << " expand_feed_suc:" << expand_feed_suc);

  if(begin == 0 && ret.size() > 0) {
    ostringstream os;
    os << "<input type=\"hidden\" value=\"" << tlfeedres.size << "\" />" << std::endl;
    ret[0] = os.str() + ret[0];
  }

  return ret;
}

feed::HtmlResult FeedViewI::GetTimelineDataWithAdmin(Ice::Int owner, Ice::Int uid, const std::string& name, Ice::Int year, Ice::Int month, Ice::Int render,
      Ice::Int begin, Ice::Int limit, bool isAdmin, const Ice::Current& current) {
  ostringstream os;
  os << "FeedViewI::GetTimelineData --> owner: " << owner << " uid: " << uid << " year:" << year << " month:" << month << " begin:" << begin << " limit:" << limit << " isAdmin:" << isAdmin;
  InvokeClient tm = InvokeClient::create(current, os.str(), InvokeClient::INVOKE_LEVEL_INFO);

  feed::FeedDataSeq feed_seq;
  feed::timeline::TimeLineFeedResult tlfeedres;
  struct timeval start, end;

  Ice::Int stype_array[] = {102, 103, 104, 107, 110, 502, 504, 601, 701, 702, 709, 801, 808,
                            1201, 1205, 1206, 2003, 2004, 2005, 2006, 2008, 2009,
                            2012, 2013, 2015, 2016, 2032, 2035, 2036, 2038, 2039, 8210, 8211,
                            8301, 9705, 9801, 9802, 9803, 9804};
  vector<Ice::Int> stypes(stype_array, stype_array + sizeof(stype_array)/sizeof(Ice::Int));

  try {
    gettimeofday(&start, NULL);
    std::map<Ice::Long, unsigned int> feeds;
    std::vector<Ice::Long> feedids;
    tlfeedres = feed::timeline::TimeLineFeedReplicaAdapter::instance().QueryFeedInfoByStype(owner, year, month, stypes, begin, limit);

    unsigned int appendIndex = 0;
    for(feed::timeline::TimeLineFeedInfoSeq::const_iterator it1 = tlfeedres.feeds.begin(); it1 != tlfeedres.feeds.end(); ++it1, ++appendIndex) {
      for(MyUtil::LongSeq::const_iterator it2 = (it1->feeds).begin(); it2 != (it1->feeds).end(); ++it2) {
        feeds.insert(std::make_pair(*it2, ((unsigned int)(it1->flag) | (appendIndex << 1))));
        feedids.push_back(*it2);
      }
    }
    feed::FeedDataSeq tmp_feed_seq;
    GetDataByIds(feedids, tmp_feed_seq, owner);

    int currentAppendIndex = -1, feed_seq_index = -1;
    for(feed::FeedDataSeq::const_iterator it = tmp_feed_seq.begin(); it != tmp_feed_seq.end(); ++it) {
      std::map<Ice::Long, unsigned int>::const_iterator iter = feeds.find((*it)->feed);
      if(iter != feeds.end()) {
        if((iter->second >> 1) != currentAppendIndex) {
          ++feed_seq_index;
          feed_seq.push_back((*it));
          if((iter->second & 0x1) == 1) {
            feed_seq[feed_seq_index]->type = ((unsigned int)feed_seq[feed_seq_index]->type) | 0x1000000;
          } else {
            feed_seq[feed_seq_index]->type = ((unsigned int)feed_seq[feed_seq_index]->type) & 0xFEFFFFFF;
          }
        } else {
          MCE_INFO("FeedViewI::GetTimelineData --> feed append. owner: " << owner << " first feed:" << feed_seq[feed_seq_index]->feed << " append feed:" << (*it)->feed);
          feed_seq[feed_seq_index]->xml += (*it)->xml;
        }
        currentAppendIndex = (iter->second >> 1);
      }
    }
  } catch (Ice::Exception & e) {
    MCE_WARN("FeedViewI::GetTimelineData -->  owner:" << owner << " uid:" << uid << " exception:" << e);
  }

  gettimeofday(&end, NULL);
  double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

  MCE_INFO("FeedViewI::GetTimelineData -->  owner:" << owner << " uid:" << uid << " count:" << feed_seq.size() << '/' << limit << " ice call dur:" << timeuse / 1000000);

  if (feed_seq.empty())
    return kEmptyResult;
  for(feed::FeedDataSeq::const_iterator it = feed_seq.begin(); it != feed_seq.end(); ++it) {
    MCE_INFO("timeline get feed: " << (*it)->feed);
  }

  feed::HtmlResult ret;
  gettimeofday(&start, NULL);
  int expand_feed_suc = ExpandUserFeeds(owner, uid, name, render|WS_TIMELINE, feed_seq, begin, &ret, isAdmin, 6);
  gettimeofday(&end, NULL);
  timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
  MCE_INFO("FeedViewI::GetTimelineData -->  owner:" << owner << " uid:" << uid << " expand dur:" << timeuse / 1000000 << " expand_feed_suc:" << expand_feed_suc);

  if(begin == 0 && ret.size() > 0) {
    ostringstream os;
    os << "<input type=\"hidden\" value=\"" << tlfeedres.size << "\" />" << std::endl;
    ret[0] = os.str() + ret[0];
  }

  return ret;
}

void FeedViewI::Test(Ice::Int uid, const std::string& name
    , Ice::Int render, Ice::Int from, Ice::Int count, const Ice::Current&) {
  feed::FeedDataResPtr feed_seq;

  MCE_INFO("FeedViewI::Test --> uid:" << uid);

  try {
    feed::FeedNewsManagerPrx fprx = CachedFeedNews2Adapter(uid);
     //feed::FeedNewsManagerPrx fprx = CachedFeedNews3Adapter(uid);
    feed_seq = fprx->GetHotFeedData(uid, from, count);
  } catch (Ice::Exception & e) {
    LOG_F(WARNING) << "GetHot() uid:" << uid << " error:" << e;
    feed_seq = NULL;
    //return ;
  }

/*
  try{
    feed_seq = FeedNewsReplicaAdapter::instance().getHotFeedData(uid, from, count);
  }catch(Ice::Exception& e){
    MCE_WARN("FeedViewI::Test --> uid:" << uid << " error:" << e);
    return ;
  }
*/
  MCE_INFO("FeedViewI::Test--> uid:" << uid << " count=" << feed_seq->data.size() << '/' << count);
  //LOG(INFO) << "GetHot() uid=" << uid << " count=" << feed_seq->data.size() << '/' << count;


  if (feed_seq->data.empty())
    return ;

  feed::HtmlResult ret;
  ExpandUserFeeds(0, uid, name, render | WS_HOT, feed_seq->data, from, &ret);
  //ExpandUserFeeds(0, uid, name, render | WS_HOT, feedSeq, from, &ret);
  //return ret;
}

}

/*
BEGIN_SERVANT_MAP("feedview")
  SERVANT_ENTRY(feedview, &xce::ServantFactory< xce::FeedViewI >::CreateServant, &xce::ServantFactory< xce::FeedViewI >::Destroy)
END_SERVANT_MAP()

// 静态函数可能添加不上，需要main内部再显示添加一次
static void Init() {
  ctemplate::Template::SetTemplateRootDirectory("/data/xce/XFeed/etc/feed_view_tpl3");
  ctemplate::Template::AddAlternateTemplateRootDirectory("/data/cwf2/tpl3");

}

GLOBAL_INIT(FEEDVIEWI, {
  xce::AddStartup(&Init, true);
});

*/
feed::HtmlResult FeedViewI::GetGuideFeed(Ice::Int uid, const std::string& name
    , Ice::Int render, Ice::Int from, Ice::Int count, const Ice::Current& cur) {

  feed::FeedDataResPtr feed_seq;
  FeedDataSeq exp_feed;

  ostringstream os;
  os << "FeedViewI::GetGuideFeed --> uid:" << uid << " begin:" << from << " limit:" << count;
  InvokeClient tm = InvokeClient::create(cur, os.str(), InvokeClient::INVOKE_LEVEL_INFO);

  struct timeval start, end;
  gettimeofday(&start, NULL);
  try{
    if (uid % 10 == 6) {
      feed_seq = FeedGuideReplicaAdapter::instance().GetGuideFeedData(uid, from, count);
    } else {
      feed_seq = FeedNewsReplicaAdapter::instance().getHotFeedData(uid, from, count);
    }
  } catch(Ice::Exception& e) {
    MCE_WARN("FeedViewI::GetGuideFeed --> uid:" << uid << " error:" << e);
    gettimeofday(&end, NULL);
    double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    MCE_WARN("FeedViewI::GetGuideFeed -->  uid:" << uid << " ice call error.  dur:" << timeuse / 1000000);
    return kEmptyResult;
  }

  if (!feed_seq || feed_seq->data.empty()){
    return kEmptyResult;
  }
  MCE_INFO("FeedViewI::GetGuideFeed --> uid:" << uid << " count=" << feed_seq->data.size() << '/' << count);

  feed::HtmlResult ret;
  if (uid % 10 == 6) {
    int expand_feed_suc = ExpandUserFeedsFeedView(0, uid, name, render | WS_GUIDEFEED, feed_seq->data, from, &ret);
    MCE_INFO("FeedViewI::GetGuideFeed --> uid:" << uid << " expand_feed_suc:" <<  expand_feed_suc);
  } else {
    int expand_feed_suc = ExpandUserFeedsFeedView(0, uid, name, render, feed_seq->data, from, &ret);
    MCE_INFO("FeedViewI::GetGuideFeed --> uid:" << uid << " expand_feed_suc:" <<  expand_feed_suc);
  }
 return ret;
}

