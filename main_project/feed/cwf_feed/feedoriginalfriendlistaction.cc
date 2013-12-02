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
#include "feed/cwf_feed/feedoriginalfriendlistaction.h"

#include "FeedItemWrapperReplicaAdapter.h"
#include "FeedMemcLoaderAdapter.h"
using namespace std;
using namespace xce::feed;
namespace xce {
static const std::string kDefaultContentType("text/html; charset=utf-8");

bool FeedOriginalFriendListAction::Match(const std::string& url) const {
  // http://www.renren.com/feedoriginalfriendlist.do
  // 点击好友原创的全部按钮发出的请求
  return boost::starts_with(url, "/feedoriginalfriendlist.do"); 
}

void FeedOriginalFriendListAction::CheckLoad(const vector<Ice::Long> & fids, FeedContentDict & dict, int uid) {
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
    MCE_WARN("FeedOriginalFriendListAction::CheckLoad --> GetFeedDict uid:" << uid << " size:" << miss_fids.size() << " err, " << e);
    return;
  }
  MCE_INFO("FeedOriginalFriendListAction::CheckLoad --> GetFeedDict uid:" << uid << " size:" << miss_fids.size() << " res:" << dict_loader.size());
  if(!dict_loader.empty()) {
    dict.insert(dict_loader.begin(), dict_loader.end());
  }
}

void FeedOriginalFriendListAction::GetDataByIds(const vector<Ice::Long> & fids, feed::FeedDataSeq & feed_seq, int uid) {
  MCE_INFO("FeedOriginalFriendListAction::GetDataByIds --> uid:" << uid << " fids size: " << fids.size());
  if(fids.empty()) {
    return;
  }
  
  FeedContentDict contentMap = FeedContentAdapter::instance().getFeedDict(fids);
  MCE_INFO("FeedOriginalFriendListAction::GetDataByIds --> uid:" << uid << " FeedContentDict size: " << contentMap.size());
  CheckLoad(fids, contentMap, uid);

  for (std::vector<Ice::Long>::const_iterator it = fids.begin(); it != fids.end(); ++it) {
    FeedContentDict::iterator dictIt = contentMap.find(*it);
    if(dictIt != contentMap.end() && dictIt->second) {
      feed_seq.push_back((dictIt->second)->data);
    }
  }
  MCE_INFO("FeedOriginalFriendListAction::GetDataByIds --> uid:" << uid << " FeedDataSeq size: " << feed_seq.size());
}

struct friend_info {
   string id, name, tinyimg;
};

cwf::HttpStatusCode FeedOriginalFriendListAction::Process(cwf::Request * req, cwf::Response * res) {
  int cwf_pid = getpid();
  MCE_INFO("FeedOriginalFriendListAction::Process --> pid:" << cwf_pid);
  boost::scoped_ptr<cwf::User> u(Authorize(req));
  if (!u) {
    return cwf::HC_UNAUTHORIZED;
  }

  int render_type = WS_HOME;
  
  res->header().set_status_code(cwf::HC_OK, "OK");
  res->header().Add(cwf::HH_CONTENT_TYPE, kDefaultContentType);
  res->OutputHeader();

  feed::FeedDataSeq feed_seq;
  vector<long> fid_seq;
  {
    render_type = WS_HOME|WS_HOT|WS_NEWSFEED|WS_FRIENDORIGINAL;
    try {
      MCE_INFO("FeedOriginalFriendListAction::Process --> pid:" << cwf_pid << " uid:" << u->id());
      fid_seq = feed::FeedItemWrapperReplicaAdapter::instance().GetFriendsFeedsList(u->id());
      if (!fid_seq.empty()) {
        GetDataByIds(fid_seq, feed_seq, u->id());
      }
    } catch (Ice::Exception & e) {
      MCE_WARN("FeedOriginalFriendListAction::Process --> pid:" << cwf_pid << " uid:" << u->id() << " exception:" << e);
    }
  }
  MCE_INFO("FeedOriginalFriendListAction::Process --> pid:" << cwf_pid << " uid:" << u->id() << " seq: " << feed_seq.size());
  
  vector<friend_info> friend_result;

  for(size_t idx = 0;idx < feed_seq.size();++ idx) {
    std::string packed_xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?><feeds>");
    FeedXml fx;
    std::string logic_str;
    packed_xml += feed_seq[idx]->xml;
    packed_xml += "</feeds>";
    int ret = ParseCtemplateXml(packed_xml, &fx);
    if (0 != ret) {
       MCE_WARN("xml parse error: " << ret << " id: " << feed_seq[idx]->feed);
       MCE_WARN("xml:" << packed_xml);
       continue;
    }
    friend_info tmp_friend;
    try {
      tmp_friend.id = fx.find("f_from_id");
      tmp_friend.name = fx.find("f_from_name");
      tmp_friend.tinyimg = fx.find("f_from_tinyimg");
    } catch(std::exception &e) {
      MCE_WARN("FeedOriginalFriendListAction::Process --> e: " << e.what());
    }
    if (!tmp_friend.id.empty() && !tmp_friend.name.empty() && !tmp_friend.tinyimg.empty()) {
      friend_result.push_back(tmp_friend);
    }
  }
  
  ostringstream os;
  os << "{\"friends\":[";
  for(vector<friend_info>::iterator it = friend_result.begin();it < friend_result.end(); ++ it) {
    //解决有些头像不显示问题
    if (it->tinyimg.find("http") == string::npos) {
      if (it->tinyimg == "0/0/men_tiny.gif" || it->tinyimg == "0/0/women_tiny.gif") {
        it->tinyimg = "http://head.xiaonei.com/photos/" + it->tinyimg;
      } else {
        it->tinyimg = "http://hdn.xnimg.cn/photos/" + it->tinyimg;
      }
    }
    os << "{\"id\":\"" << it->id << "\", \"name\":\"" 
       << it->name << "\", \"tinyimg\":\"" << it->tinyimg << "\"}";
    if (it + 1 < friend_result.end()) {
      os << ",";
    }
  }
  os << "]}";
  res->WriteRaw(os.str());
  return cwf::HC_OK;
}

static void Init2() {
  cwf::FrameWork::RegisterAction(new FeedOriginalFriendListAction);

#ifdef WIN32
  return;
#endif

}

GLOBAL_INIT(CWF_TYPE_ACTION, {
  AddStartup(&Init2);
});

}
