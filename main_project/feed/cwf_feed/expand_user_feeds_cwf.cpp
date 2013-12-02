#include "feed/cwf_feed/expand_user_feeds_cwf.h"
#include "feed/ilikeit_adapter.h"
#include "feed/feedview/feed_hdf_parser.h"
#include "feed/feedview/expandfeed.h"
#include "OceCxxAdapter/src/UserUrlAdapter.h"
#include "feed/feedview/feedxml.h"
#include <sys/resource.h>
using namespace std;
namespace xce{
using namespace feed;
int ExpandUserFeedsCwf(int owner, int uid, const std::string& name,
    int render, feed::FeedDataSeq& feed_seq, int from, cwf::Response * res,
    std::string& stat1, bool isAdmin, int view) {

  MCE_INFO("ExpandUserFeedsCwf Entering");
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
      //MCE_INFO("ExpandUserFeedsCwf feedid: " << (feed->feed) << " stype = " << (feed->type & 0xFFFF));
      if (BuildGlobalId(feed->source, feed->type, &globalseq)) {
        likeInfoAvailable[3 * i] = true;
      }
      if (!isClearSilver[i]) {
        const std::vector<std::string>  reply_v = feedXmls[curFeedXmlIndex - 1].find_v("f_reply_id");
        for(size_t j = 0; j < 2 && j < reply_v.size(); ++ j) {
          long long feed_reply_id = 0;
          try {
        //    MCE_INFO("ExpandUserFeedsCwf --> feed_reply_id 0 : " << reply_v[j]);
            feed_reply_id = boost::lexical_cast<long long>(reply_v[j]);
          } catch(std::exception &e) {
            MCE_WARN("ExpandUserFeedsCwf --> transform err : " << e.what());
          }
          if (BuildCommentGlobalId(feed_reply_id, feed->type, &globalseq)) {
            likeInfoAvailable[3 * i + 1 + j] = true;
          }
          //MCE_INFO("ExpandUserFeedsCwf Old Tpl --> feedid : " << (feed->feed) << " reply_id : " << feed_reply_id << "likeInfoAvailable : " << likeInfoAvailable[3 * i + 1 + j]);
        }
      } else {
        map<string, vector<map<string, string> > >& loop_nodes = feedHdfParsers[curHdfParserIndex - 1]->loop_nodes();
        map<string, vector<map<string, string> > >::iterator it = loop_nodes.find("f.reply");
        long long feed_reply_id = 0;
//        if (it != loop_nodes.end()) {
//          for(map<string, vector<map<string, string> > >::iterator it = loop_nodes.begin();it != loop_nodes.end(); ++ it) {
//            MCE_INFO("ExpandUserFeedsCwf --> key: " << (it -> first) << "  val.size(): " << (it->second).size());
//           for(int i = 0; i < (it->second).size(); ++ i) {
//              map<string, string>& m = it->second[i];
//              for(map<string, string>::iterator mit = m.begin(); mit != m.end(); ++ mit) {
//                MCE_INFO("ExpandUserFeedsCwf --> map key: " << (mit -> first) << "  val: " << (mit->second));
//              }
//            }
//          }
//        }
        if (it != loop_nodes.end() && (it->second).size() > 0) {
          try {
            map<string, string>::iterator mit = (it -> second).at(0).find("id");
            if (mit != (it -> second).at(0).end()) {
//              MCE_INFO("ExpandUserFeedsCwf --> feed_reply_id 1 : " << (mit->second));
              feed_reply_id = boost::lexical_cast<long long>(mit->second);
            }
          } catch(std::exception &e) {
            MCE_WARN("ExpandUserFeedsCwf --> transform err : " << e.what());
          }
          if (BuildCommentGlobalId(feed_reply_id, feed->type, &globalseq)) {
            likeInfoAvailable[3 * i + 1] = true;
          }
//          MCE_INFO("ExpandUserFeedsCwf --> reply_id : " << feed_reply_id << "likeInfoAvailable 1: " << likeInfoAvailable[3 * i + 1]);
        }
        if (it != loop_nodes.end() && (it->second).size() > 1) {
          try {
            map<string, string>::iterator mit = (it -> second).at(1).find("id");
            if (mit != (it -> second).at(1).end()) {
//              MCE_INFO("ExpandUserFeedsCwf --> feed_reply_id 2 : " << (mit->second));
              feed_reply_id = boost::lexical_cast<long long>(mit->second);
            }
          } catch(std::exception &e) {
            MCE_WARN("ExpandUserFeedsCwf --> transform err : " << e.what());
          }
          if (BuildCommentGlobalId(feed_reply_id, feed->type, &globalseq)) {
            likeInfoAvailable[3 * i + 2] = true;
          }
//          MCE_INFO("ExpandUserFeedsCwf --> reply_id : " << feed_reply_id << "likeInfoAvailable 2: " << likeInfoAvailable[3 * i + 2]);
        }
      }
    }
  }

  bool likeinfo_avaible = false;
  if (!globalseq.empty()) {
    // try catch inside
    struct timeval start, end;
    gettimeofday(&start, NULL);
    likeinfo_avaible = BuildZan(uid, globalseq, &likeseq);
    gettimeofday(&end, NULL);
    double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    MCE_INFO("ExpandUserFeeds --> uid:" << uid << " zan dur:" << timeuse / 1000000);
  }
/// 获取Vip信息， 分条渲染
  std::string stats;
  bool isParsed = true;
  {

    MyUtil::IntSeq uids, types;
    types.push_back(0); //目前只传0，具体有哪些类型，见OceCxxAdapter/src/VipIconDataReaderAdapter.h中的VIP_TYPE枚举类型
    for (size_t i = 0; i < feed_seq.size(); ++i) {
      uids.push_back(feed_seq[i].get()->actor);
    }
    vipinfo::VipFeedDataSeqMap vipInfoMap;
    const vipinfo::VipFeedDataSeq emptyVipData;
    try {
      vipInfoMap = adapter::vipicondata::VipIconDataReaderAdapter::instance().getVipFeedDataSeqMap(uids, types);
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
      MCE_DEBUG("expand_user_feeds_cwf --> fid:" << feed_seq[i].get()->feed << " u:" << (&u)->id() << " result:" << f);
      if (f) {
        ++ feed_count;
        ++ from;
        res->WriteRaw(output);
        stats += stat_log;
        stats += "\n";
      }
    }
  }
  stat1 = stats;
  return feed_count;
}
}
