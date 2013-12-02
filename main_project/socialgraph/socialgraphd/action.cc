#include "socialgraph/socialgraphd/action.h"

#include <algorithm>
#include <set>
#include <list>

#include <boost/foreach.hpp>
#include <ctemplate/template.h>

#include "base/ptime.h"
#include "base/logging.h"
#include "base/asyncall.h"
#include "base/stringdigest.h"

#include "OceCxxAdapter/src/ContentRcdAdapter.h"
#include "OceCxxAdapter/src/ActiveTrackAdapter.h"
#include "OceCxxAdapter/src/SocialGraphRcdMessageServiceAdapter.h"
#include "OceCxxAdapter/src/RecommendationManangerAdapter.h"
#include "OceCxxAdapter/src/DistributionManagerAdapter.h"
#include "socialgraph/socialgraphd/socialgraphlogic_adapter.h"
#include "socialgraph/socialgraphd/headurl.h"
#include "socialgraph/socialgraphutil/algostuff.h"
#include "socialgraph/socialgraphutil/mc_adapter.h"
#include "socialgraph/socialgraphd/tpl/rvideo.tpl.varnames.h"
#include "socialgraph/socialgraphd/tpl/rblog.tpl.varnames.h"
#include "OceCxxAdapter/src/RecommendationPhotofriendAdapter.h"
using namespace xlog;
using namespace std;
namespace xce {
namespace socialgraph {

com::xiaonei::service::XceLoggerPtr LogStatsTask::remote_logger_ = NULL;
std::vector<std::string> LogStatsTask::log_paths_;

XLogInstance* XLogInstance::m_pInstance = NULL;
std::vector<std::string> XLogInstance::xlog_paths_;
bool XLogInstance::_init = XLogInstance::Init();

//-----------------------------------------------------------------------------------------------------------

cwf::HttpStatusCode BlogAction::DoPost(cwf::Request* request, cwf::Response* response, cwf::User* host) {
  std::string blogid_str = request->form("blog_id");	
	//int userid = host->id();		//用户id
	std::ostringstream oss;
	oss << "BlogAction::DoPost() host:" << host->id();
	PTIME(pt, oss.str(), true, 0);
	//long blogid = 0l;		//当前日志id
	//try {
	//	blogid = boost::lexical_cast<long>(blogid_str);		//当前日志id
	//} catch (boost::bad_lexical_cast& e) {
	//	LOG(LERROR) << "[BlogAction] DoPost bad_lexical_cast" << e.what();
  //	return cwf::HC_OK;
	//} catch (...) {
	//	LOG(LERROR) << "[BlogAction] DoPost unknown exception";
  //	return cwf::HC_OK;
	//}

  //Items type_items = GetBlogDataByBlogTag(blogid);
  //Items action_items = GetBlogDataByUseraction(userid);

  //int type_items_size = type_items.items_size();
  //int action_items_size = action_items.items_size();

	//LOG(INFO)	<< "[BlogAction] DoPost blogid:" << blogid << " userid:" << userid 
	//	<< " type_items_size:" << type_items_size << " action_items_size:" << action_items_size;
  //
  //std::vector<LayoutPtr> layout_list; 
  //std::set<long> fliter_blogid;			//blogid过滤器
  //for (int i = 0; i < type_items_size; ++i) {			//优先将按相关度推荐的日志放入cache中
	//	std::string message = type_items.items(i).message();			//按相关度推荐日志:message  share_id:blog_id:weight
	//	std::vector<std::string> long_list;
	//	boost::split(long_list, message, boost::is_any_of(":"));
	//	if (3 != (int)long_list.size()) {
	//		continue;
	//	}

	//	long rec_blogid = 0l;
	//	try {
	//		rec_blogid = boost::lexical_cast<long>(long_list.at(1));
	//	} catch (boost::bad_lexical_cast& e) {
	//		LOG(LERROR) << "[BlogAction] DoPost BLOGTAG bad_lexical_cast" << e.what();
	//		continue;
	//	} catch (...) {
	//		LOG(LERROR) << "[BlogAction] DoPost BLOGTAG unknown exception";
	//		continue;
	//	}

  //  LayoutPtr layout(new BlogLayout(type_items.items(i), "BLOGTAG"));
  //  layout_list.push_back(layout);
  //  fliter_blogid.insert(rec_blogid);		//插入过滤器中
  //}

  //for (int i = 0; i < action_items_size; ++i) {		//需要过滤两种数据源数据
	//	std::string message = action_items.items(i).message();		//按用户行为推荐日志:message  share_id:blog_id
	//	std::vector<std::string> long_list;
	//	boost::split(long_list, message, boost::is_any_of(":"));
	//	if (2 != (int)long_list.size()) {
	//		continue;
	//	}

	//	long rec_blogid = 0l;
	//	try {
	//		rec_blogid = boost::lexical_cast<long>(long_list.at(1));
	//	} catch (boost::bad_lexical_cast& e) {
	//		LOG(LERROR) << "[BlogAction] DoPost BLOGACTION bad_lexical_cast" << e.what();
	//		continue;
	//	} catch (...) {
	//		LOG(LERROR) << "[BlogAction] DoPost BLOGACTION unknown exception";
	//		continue;
	//	}

	//	if (fliter_blogid.find(rec_blogid) != fliter_blogid.end()) {		//过滤重复的blogid
	//		continue;
	//	}
	//	LayoutPtr layout(new BlogLayout(action_items.items(i), "BLOGACTION"));
	//	layout_list.push_back(layout);
  //}

  //std::random_shuffle(layout_list.begin(), layout_list.end());
  //if ((int)layout_list.size() > 2) {    //添加layout_list的size判断
  //  layout_list.resize(2);      //前端每次只展示2个
  //}

  //BOOST_FOREACH (LayoutPtr ptr, layout_list) {
  //  std::string out = ptr->DoLayout(NULL);
  //  response->WriteRaw(out);
  //}
	std::string out = "";
	response->WriteRaw(out);
  return cwf::HC_OK;
}

xce::socialgraph::Items BlogAction::GetBlogDataByBlogTag(long blogid) {
	std::string blog_key = xce::Comb2("BLOG", blogid);  //拼接出获取blog信息的key
	RawData binary_data;
  MemcachedAdapter::GetInstance().Get2(blog_key, binary_data);  //获取该blog对应的type信息
  if (binary_data.empty()) {
    return Items();
  }

  Items blog_item;
  blog_item.ParseFromArray(binary_data.data(), binary_data.size());
  Item blog = blog_item.items(0);     //取权值type近似度最大的

  std::string type_key = xce::Comb2("BLOGTAG", blog.id());
  RawData type_data;
  MemcachedAdapter::GetInstance().Get(type_key, type_data);   //取该类tags的推荐列表
  if (type_data.empty()) {
    return Items();
  }

  Items type_item;
  type_item.ParseFromArray(type_data.data(), type_data.size());
  return type_item;
}

xce::socialgraph::Items BlogAction::GetBlogDataByUseraction(int userid) {
  std::string key = xce::Comb2("BLOGACTION", userid);  		//拼接出获取blog信息的key
  RawData action_data;
  MemcachedAdapter::GetInstance().Get(key, action_data);  	//获取该key对应的推荐数据信息
	if (action_data.empty()) {
		return Items();
	}

  Items action_item;
  action_item.ParseFromArray(action_data.data(), action_data.size());

  return action_item;
}

//-----------------------------------------------------------------------------------------------------------

cwf::HttpStatusCode VideoAction::DoPost(cwf::Request* request, cwf::Response* response, cwf::User* host) {
	int size_limit = 2;		//limit of content is 2
	std::ostringstream oss;
	oss << "VideoAction::DoPost() host:" << host->id();
	PTIME(pt, oss.str(), true, 0);
  //std::string sv_url = request->form("sv_url");
  //if (sv_url.empty()) {
  //  sv_url = request->query("sv_url");
  //  if (sv_url.empty()) {
  //    LOG(INFO) << "sv_url is empty";
  //    response->WriteRaw("");
  //    return cwf::HC_OK;
  //  }
  //}
  //const std::string md5 = xce::MD5(sv_url);
  //LOG(INFO) << " sv_url:" << sv_url << "\tmd5:" << md5;
  //ContentRcdSeq rcd_seq;
  //try {
  //  ContentRcd cr;
  //  cr.md5 = xce::MD5(sv_url);
  //  cr.lisc = 2;
  //  rcd_seq = ContentRcdAdapter::instance().GetVideoRcd(host->id(), cr);
  //} catch (Ice::Exception& e) {
  //  LOG(LERROR) << e.what();
  //} catch (...) {
  //  LOG(LERROR) << "ContentRcdAdapter::instance().GetVideoRcd() ... exception" << " sv_url:" << sv_url;
  //}

  //if (rcd_seq.empty()) {
  //  LOG(INFO) << "rcd_seq is empty() host:" << host->id() << " sv_url:" << sv_url;
  //}

	//if ((int)rcd_seq.size() > size_limit) {		//resize if error in recommendation of 
	//	rcd_seq.resize(size_limit);
	//}

  //std::vector<LayoutPtr> layouts;
  //BOOST_FOREACH(ContentRcd& rcd, rcd_seq) {
  //  LayoutPtr layout(new VideoLayout(rcd.thumburl, rcd.title, rcd.sid, rcd.uid));
  //  layouts.push_back(layout);    
  //}
  //BOOST_FOREACH(LayoutPtr layout, layouts) {
  //  std::string out = layout->DoLayout(NULL);
  //  response->WriteRaw(out);
  //}
	std::string out = "";
  response->WriteRaw(out);
  return cwf::HC_OK;
}

//-----------------------------------------------------------------------------------------------------------

void LogStatsTask::InitLogPath() {
  using namespace com::xiaonei::service;
  log_paths_.push_back("socialgraph");
  log_paths_.push_back("reconnect");
  log_paths_.push_back("homeReconnect");
  remote_logger_ = XceLoggerAdapter::instance().getLogger();
  remote_logger_->initialize();
}

void LogStatsTask::handle() {
  if (!param_.empty()) {
    remote_logger_->addLog(log_paths_, param_);
  }
}
//xlog task,write logs to hadoop of yz
//-----------------------------------------------------------------------------------------------------------

void XLogTask::InitLogPath() {
}

void XLogTask::handle() {
  if (!param_.empty()) {
		time_t now_time;
                now_time = time(NULL);
                ostringstream ss;
                ss << now_time ;
                param_ = ss.str() + "|" + param_;
        	int ret = XLogInstance::GetInstance()->xremote_logger_->append(param_);
  }
}
//-----------------------------------------------------------------------------------------------------------

cwf::HttpStatusCode GetAction5::DoPost(cwf::Request* request, cwf::Response* response, cwf::User* host) {
	std::ostringstream oss;
	oss << "GetAction5::DoPost() host:" << host->id();
	PTIME(pt, oss.str(), true, 0);
	std::vector<LayoutPtr> layouts = PrepareFriendsLayout(host->id());
  std::vector<LayoutPtr> other_layouts = PrepareOtherLayout(host->id());
	layouts.insert(layouts.end(), other_layouts.begin(), other_layouts.end());		//use default strategy
	
  std::vector<int> user_id_seq = GetUserIdSeq(&layouts);
  UserCacheMap users;
  UserCacheMap usercaches_map;
  try {
		usercaches_map = xce::usercache::UserCacheAdapter::instance().GetUserCache(user_id_seq);
    FilterUserCache(usercaches_map, users);
  } catch (...) {
    LOG(LERROR) << "xce::usercache::UserCacheAdapter::instance().GetUserCache() exception...  host:" << host->id();
  }

  const std::string bizType("Rcd4NewFriendsLayout");
	bool biz_write = false;
	std::string out = "";				//output of result
	BOOST_FOREACH(LayoutPtr layout, layouts) {
		if (bizType == layout->getBizType()) {
			out += layout->DoLayout(&users);
			biz_write = true;
		}
	}

	if (biz_write &&
			!out.empty()) {			//justify whether write biz type and whether first 4 friends is empty
		out += "##########";
	}

	BOOST_FOREACH(LayoutPtr layout, layouts) {
		if (bizType != layout->getBizType()) {
			out += layout->DoLayout(&users);
		}
	}
	response->WriteRaw(out);

  return cwf::HC_OK;
}

void GetAction5::OutputShowLog(int uid, const RecommendItemSeq& item_seq) {
	std::ostringstream oss;
	oss << "GetAction5 OutputShowLog uid:" << uid;
	for (RecommendItemSeq::const_iterator iter = item_seq.begin();
			iter != item_seq.end(); ++iter) {
		oss << "|" << iter->id;
	}

	LOG(INFO) << oss.str();
}

std::vector<LayoutPtr> GetAction5::PrepareFriendsLayout(int uid) {
	int limit = -1;			//get all
  std::vector<LayoutPtr> layout_seq;
	RecommendItemSeq item_seq;

	try {
		//item_seq = RecommendationManangerAdapter::instance().GetFriendRecommendByHome(uid, limit);
		DistributionManagerPrx prx = DistributionManagerAdapter::instance().getProxy(uid);
		LOG(INFO) << "DistributionManagerAdapter uid(" << uid << ") proxy(" << prx << ")";
		item_seq = DistributionManagerAdapter::instance().GetFriendRecommendByHome(uid, limit);
	} catch (Ice::Exception& e) {
		LOG(LERROR) << "GetAction5::PrepareFriendsLayout Ice::Exception " << e.what();
		return layout_seq;
	} catch (...) {
		LOG(LERROR) << "GetAction5::PrepareFriendsLayout unknown Exception";
		return layout_seq;
	}
	std::map<std::string, int> type2count_map1;
	std::map<std::string, int> type2count_map2;
  int currentRcdFriendsNum = 0;

//	OutputShowLog(uid, item_seq);		//output show item_seq

	BOOST_FOREACH (const RecommendItem& item, item_seq) {
		if (kNewRcdFriendsSize > currentRcdFriendsNum++) {
    	layout_seq.push_back(LayoutPtr(new Rcd4NewFriendsLayout(uid, item)));		//create different layout by type
			++type2count_map1[item.type + "_" + item.experiment];			//stats num of different type in product of show 4 in a line		FFW_O or FFW_E1
		} else {
    	layout_seq.push_back(LayoutPtr(new Rcd3NewFriendsLayout(uid, item)));		//create different layout by type
			++type2count_map2[item.type + "_" + item.experiment];			//stats num of different type in product show 3 in a column
		}
	}

	OutputLogStatsByProduct(uid, "RecFriend", type2count_map1);		//output hadoop log of RecFriend
	OutputLogStatsByProduct(uid, "RecMixed", type2count_map2);		//output hadoop log of RecMixed
	LOG(INFO) << "GetAction5::PrepareFriendsLayout() user_id:" << uid << " items.size():" << item_seq.size();

  return layout_seq;
}

std::vector<LayoutPtr> GetAction5::PrepareOtherLayout(int uid) {
  std::vector<LayoutPtr> layout_seq;
  const static std::string kRcdBiz = "RCD";
  RawData bin_data;
  bool fetch = BusinessCacheAdapter::GetInstance().Get(kRcdBiz, uid, bin_data);
  if (!fetch) {
    LOG(INFO) << "GetAction5::PrepareOtherLayout() user_id:" << uid << " NO_RCD_DATA";
  } else {
		std::map<std::string, int> type2count_map;
    Items items;
    items.ParseFromArray(bin_data.data(), bin_data.size());
		BOOST_FOREACH (const Item& item, items.items()) {
			if (COMMON_FRIENDS != item.message() &&
					FREINDS_CLUSTER != item.message() &&
					NEW_COMMON_FRIENDS != item.message()) {					//exclude people recommendation
		//		LOG(INFO)<<"item:message:"<<item.message();
				layout_seq.push_back(LayoutFactory::Create(uid, item));		//create different layout by type
				++type2count_map[item.message()];					//stats num of different type in product show 3 in a column
			}
		}
		OutputLogStatsByProduct(uid, "RecMixed", type2count_map);				//output hadoop log of RecMixed
    LOG(INFO) << "GetAction5::PrepareOtherLayout() user_id:" << uid << " items.size():" << layout_seq.size();
  }
	std::random_shuffle(layout_seq.begin(), layout_seq.end());			//random strategy
  
  return layout_seq;
}

std::vector<LayoutPtr> GetAction5::PrepareLayout(int uid) {
  std::vector<LayoutPtr> layout_seq;
  const static std::string kRcdBiz = "RCD";
  RawData bin_data;
  bool fetch = BusinessCacheAdapter::GetInstance().Get(kRcdBiz, uid, bin_data);
  if (!fetch) {
    LOG(INFO) << "GetAction5::PrepareLayout() user_id:" << uid << " NO_RCD_DATA";
  } else {
    Items items;
    items.ParseFromArray(bin_data.data(), bin_data.size());
		std::map<std::string, int> type2count_map1;
		std::map<std::string, int> type2count_map2;
    currentRcdFriendsNum = 0;
    BOOST_FOREACH(const Item& item, items.items()) {
      if ((kNewRcdFriendsSize > currentRcdFriendsNum) && 
        (COMMON_FRIENDS == item.message()) || FREINDS_CLUSTER == item.message()) {
        layout_seq.push_back(LayoutPtr(new Rcd4FriendsLayout(uid, item)));		//create different layout by type
        currentRcdFriendsNum++;
				++type2count_map1[item.message()];			//stats num of different type in product of show 4 in a line
      } else {
        layout_seq.push_back(LayoutFactory::Create(uid, item));		//create different layout by type
				++type2count_map2[item.message()];			//stats num of different type in product show 3 in a column
      }
    }
		OutputLogStatsByProduct(uid, "RecFriend", type2count_map1);		//output hadoop log of RecFriend
		OutputLogStatsByProduct(uid, "RecMixed", type2count_map2);		//output hadoop log of RecMixed
    LOG(INFO) << "GetAction5::PrepareLayout() user_id:" << uid << " items.size():" << items.items_size() << " data.size():" << items.ByteSize();
  }
  
  return layout_seq;
}

//product|uid|count1|count2|count3|...|type1|type2|type3|...；其中，type个数和count个数对应。
void GetAction5::OutputLogStatsByProduct(int userid, 
														 						 const std::string& biz, 
														 						 const std::map<std::string, int>& type2count) {
	std::string s_output = biz + "_numOfRec|";     //prefix
	try {
		std::string s_userid = boost::lexical_cast<std::string>(userid);		//hostid
                std::string s_tempcount = "";
		std::string s_temptype  = "";
        	for (std::map<std::string, int>::const_iterator iter = type2count.begin();
				iter != type2count.end(); ++iter) {
			std::string s_count = boost::lexical_cast<std::string>(iter->second);
			s_tempcount += "|" + s_count;   
                        s_temptype  += "|" + iter->first;	
		}
                s_output += s_userid + s_tempcount + s_temptype + "\n";
              //  LOG(INFO) << "[GetAction5] OutputLogStatsByProduct log:" << s_output; 
		if (!s_output.empty()) {
			MyUtil::TaskManager::instance().execute(new LogStatsTask(userid, s_output));			//stats total recommendation by biz
			MyUtil::TaskManager::instance().execute(new XLogTask(userid, s_output));			//stats total recommendation by biz
			
		}
	} catch (boost::bad_lexical_cast& e) {
		LOG(LERROR) << "[GetAction5] OutputLogStatsByProduct bad_lexical_cast" << e.what();
	} catch (...) {
		LOG(LERROR) << "[GetAction5] OutputLogStatsByProduct unknown exception";
	}
}

std::vector<int> GetAction5::GetUserIdSeq(std::vector<LayoutPtr>* layouts) {
  std::vector<int> id_seq;
  std::set<int> id_set;
  BOOST_FOREACH(LayoutPtr layout, *layouts) {
    std::set<int> t_set = layout->UserIdSet();
    id_set.insert(t_set.begin(), t_set.end());
  }
  id_seq.insert(id_seq.end(), id_set.begin(), id_set.end());
  return id_seq;
}

void GetAction5::FilterUserCache(const UserCacheMap& map1, UserCacheMap& map2) {
  UserCacheMap::const_iterator it = map1.begin();
  for (; it != map1.end(); ++it) {
    int status = it->second->status();   
    if (status != 6 && status != 7 && status != 8) {
      map2.insert(std::make_pair(it->first, it->second));
    }
  }
}

//-----------------------------------------------------------------------------------------------------------

cwf::HttpStatusCode RemoveAction5::DoPost(cwf::Request* request, cwf::Response* response, cwf::User* host) {
	std::ostringstream oss;
	oss << "RemoveAction5::DoPost() host:" << host->id();
	PTIME(pt, oss.str(), true, 0);

  std::string tmp_biz_name = request->get("type");		//FFW, FFUCR => FFW_E1, FFUCR_O
  std::string biz_name     = tmp_biz_name;
  int  start = tmp_biz_name.find("_");
  int  end   = tmp_biz_name.rfind("_");
  int  rcdPos= tmp_biz_name.find("RCD");
 
  //LOG(INFO) << "RemoveAction5()::DoPost() original type " << tmp_biz_name;
  if((rcdPos<0) && (start>0) && (start==end)){ //Filter: contians only one string "_" and not contians string "RCD";
     biz_name = tmp_biz_name.substr(0,start);
     //LOG(INFO) << "RemoveAction5()::DoPost() subtype from " << tmp_biz_name << " to type " << biz_name;
  }
  //LOG(INFO) << "RemoveAction5()::DoPost() result type " << biz_name;	
  if (biz_name.empty()) {
    return cwf::HC_OK;
  }
  long guest = request->get("guest", -1);
  if (guest == -1) {
    return cwf::HC_OK;
  }

  try {
	if (biz_name == "FOF") {
		RcdMessageServiceAdapter::instance().AddSgBlockAndRemoveFoFCache(host->id(), guest, biz_name);
	} else {
		RcdMessageServiceAdapter::instance().AddSgBlockAndRemoveRcdCache(host->id(), guest, biz_name);
	}
  } catch (Ice::Exception& e) {
    LOG(LERROR) << e.what();
  } catch (...) {
    LOG(LERROR) << "RemoveAction5()::DoPost() .. exception host:" << host->id() << " guest:" << guest << " biz_name:" << biz_name;
  }

  return cwf::HC_OK;
}

//-----------------------------------------------------------------------------------------------------------

StatAction::StatAction() {
  using namespace com::xiaonei::service;
  log_paths_.push_back("socialgraph");
  log_paths_.push_back("reconnect");
  log_paths_.push_back("homeReconnect");
  remote_logger_ = XceLoggerAdapter::instance().getLogger();
  remote_logger_->initialize();
}

cwf::HttpStatusCode StatAction::DoPost(cwf::Request* request, cwf::Response* response, cwf::User* host) {
	std::ostringstream oss;
	oss << "StatAction::DoPost() host:" << host->id();
	PTIME(pt, oss.str(), true, 0);
  std::string param = request->get("param");
  if (!param.empty()) {
    remote_logger_->addLog(log_paths_, param);
  }
  return cwf::HC_OK;
}

//-----------------------------------------------------------------------------------------------------------

cwf::HttpStatusCode GetOtherAction5::DoPost(cwf::Request* request, cwf::Response* response, cwf::User* host) {
	std::ostringstream oss;
	oss << "GetOtherAction5::DoPost() host:" << host->id();
	PTIME(pt, oss.str(), true, 0);

  std::vector<LayoutPtr> layouts = PrepareFriendsLayout(host->id());
  std::vector<LayoutPtr> other_layouts = PrepareOtherLayout(host->id());
	layouts.insert(layouts.begin(), other_layouts.begin(), other_layouts.end());

  std::vector<int> user_id_seq = GetUserIdSeq(&layouts);
  UserCacheMap users;
  UserCacheMap usercaches_map;
  try {
		usercaches_map = xce::usercache::UserCacheAdapter::instance().GetUserCache(user_id_seq);
    FilterUserCache(usercaches_map, users); 
  } catch (...) {
    LOG(LERROR) << "xce::usercache::UserCacheAdapter::instance().GetUserCache() exception...  host:" << host->id();
  }

  std::random_shuffle(layouts.begin(), layouts.end());

  BOOST_FOREACH(LayoutPtr layout, layouts) {
    std::string out = layout->DoLayout(&users);
    response->WriteRaw(out);
  }
	return cwf::HC_OK;
}

std::vector<LayoutPtr> GetOtherAction5::PrepareFriendsLayout(int uid) {
	int limit = -1;			//get all
  std::vector<LayoutPtr> layout_seq;

	RecommendItemSeq item_seq;
	try {
		//item_seq = RecommendationManangerAdapter::instance().GetFriendRecommendByHome(uid, limit);
		item_seq = DistributionManagerAdapter::instance().GetFriendRecommendByHome(uid, limit);
	} catch (Ice::Exception& e) {
		LOG(LERROR) << "GetOtherAction5::PrepareFriendsLayout Ice::Exception " << e.what();
		return layout_seq;
	} catch (...) {
		LOG(LERROR) << "GetOtherAction5::PrepareFriendsLayout unknown Exception";
		return layout_seq;
	}

	std::map<std::string, int> type2count_map;
	BOOST_FOREACH (const RecommendItem& item, item_seq) {
   	layout_seq.push_back(LayoutPtr(new ShareFriendsLayout(uid, item)));		//create different layout by type
		++type2count_map[item.type + "_" + item.experiment];			//stats num of different type in product of share profile FFW_O or FFW_E1
	}
	OutputLogStatsByProduct(uid, "RecShare", type2count_map);		//output hadoop log of RecFriend
  LOG(INFO) << "GetOtherAction5::PrepareFriendsLayout() user_id:" << uid << " items.size():" << layout_seq.size();


  return layout_seq;
}

std::vector<LayoutPtr> GetOtherAction5::PrepareOtherLayout(int uid) {
  std::vector<LayoutPtr> layout_seq;
  const static std::string kRcdBiz = "RCD";
  RawData bin_data;
  bool fetch = BusinessCacheAdapter::GetInstance().Get(kRcdBiz, uid, bin_data);
  if (!fetch) {
    LOG(INFO) << "GetOtherAction5::PrepareLayout() user_id:" << uid << " NO_RCD_DATA";
  } else {
    Items items;
    items.ParseFromArray(bin_data.data(), bin_data.size());

		std::map<std::string, int> type2count_map;
    BOOST_FOREACH(const Item& item, items.items()) {
			std::string type = "SHARE_" + item.message();
			if (type == SHARE_PAGE) {					//only show page in other entry
				layout_seq.push_back(LayoutFactory::Create(uid, item, "SHARE"));	//create different layout by type and biz
				++type2count_map[item.message()];					//stats num of different type in share in a column
			}
    }
		OutputLogStatsByProduct(uid, "RecShare", type2count_map);				//output hadoop log of RecMixed
    LOG(INFO) << "GetOtherAction5::PrepareOtherLayout() user_id:" << uid << " items.size():" << layout_seq.size();
  }
  
  return layout_seq;
}

std::vector<LayoutPtr> GetOtherAction5::PrepareLayout(int uid) {
  std::vector<LayoutPtr> layout_seq;
  const static std::string kRcdBiz = "RCD";
  RawData bin_data;
  bool fetch = BusinessCacheAdapter::GetInstance().Get(kRcdBiz, uid, bin_data);
  if (!fetch) {
    LOG(INFO) << "GetOtherAction5::PrepareLayout() user_id:" << uid << " NO_RCD_DATA";
  } else {
    Items items;
    items.ParseFromArray(bin_data.data(), bin_data.size());

    BOOST_FOREACH(const Item& item, items.items()) {
			std::string type = "SHARE_" + item.message();
			if (type == SHARE_COMMON_FRIENDS ||				//only show common friends, cluster friends and page in other entry
					type == SHARE_FREINDS_CLUSTER ||
					type == SHARE_PAGE) {
				layout_seq.push_back(LayoutFactory::Create(uid, item, "SHARE"));	//create different layout by type and biz
			}
    }
    LOG(INFO) << "GetOtherAction5::PrepareLayout() user_id:" << uid << " items.size():" << items.items_size() << " data.size():" << items.ByteSize();
  }
  
  return layout_seq;
}

std::vector<int> GetOtherAction5::GetUserIdSeq(std::vector<LayoutPtr>* layouts) {
  std::vector<int> id_seq;
  std::set<int> id_set;
  BOOST_FOREACH(LayoutPtr layout, *layouts) {
    std::set<int> t_set = layout->UserIdSet();
    id_set.insert(t_set.begin(), t_set.end());
  }
  id_seq.insert(id_seq.end(), id_set.begin(), id_set.end());
  return id_seq;
}

void GetOtherAction5::FilterUserCache(const UserCacheMap& map1, UserCacheMap& map2) {
  UserCacheMap::const_iterator it = map1.begin();
  for (; it != map1.end(); ++it) {
    int status = it->second->status();   
    if (status != 6 && status != 7 && status != 8) {
      map2.insert(std::make_pair(it->first, it->second));
    }
  }
}

void GetOtherAction5::OutputLogStatsByProduct(int userid, 
															 								const std::string& biz, 
															 								const std::map<std::string, int>& type2count) {
	std::string s_output = biz + "_numOfRec|";
	try {
		std::string s_userid = boost::lexical_cast<std::string>(userid);                //hostid
                std::string s_tempcount = "";
                std::string s_temptype  = "";
                for (std::map<std::string, int>::const_iterator iter = type2count.begin();
                                iter != type2count.end(); ++iter) {
                        std::string s_count = boost::lexical_cast<std::string>(iter->second);
                        s_tempcount += "|" + s_count;    
                        s_temptype  += "|" + iter->first;       
                }
                s_output += s_userid + s_tempcount + s_temptype + "\n";
              //  LOG(INFO) << "[GetOtherAction5] OutputLogStatsByProduct log:" << s_output;
                if (!s_output.empty()) {
			MyUtil::TaskManager::instance().execute(new LogStatsTask(userid, s_output));			//stats total recommendation by biz
			MyUtil::TaskManager::instance().execute(new XLogTask(userid, s_output));			//stats total recommendation by biz
		}
	} catch (boost::bad_lexical_cast& e) {
		LOG(LERROR) << "[GetAction5] OutputLogStatsByProduct bad_lexical_cast" << e.what();
	} catch (...) {
		LOG(LERROR) << "[GetAction5] OutputLogStatsByProduct unknown exception";
	}
}

//-----------------------------------------------------------------------------------------------------------

cwf::HttpStatusCode RemoveOtherAction5::DoPost(cwf::Request* request, cwf::Response* response, cwf::User* host) {
	std::ostringstream oss;
	oss << "RemoveOtherAction5::DoPost() host:" << host->id();
	PTIME(pt, oss.str(), true, 0);

  std::string tmp_biz_name = request->get("type");              //FFW, FFUCR => FFW_E1, FFUCR_O
  std::string biz_name     = tmp_biz_name;
  int  start = tmp_biz_name.find("_");
  int  end   = tmp_biz_name.rfind("_");
  int  rcdPos= tmp_biz_name.find("RCD");

  //LOG(INFO) << "RemoveOtherAction5()::DoPost() original type " << tmp_biz_name;
  if((rcdPos<0) && (start>0) && (start==end)){ //Filter: contians only one string "_" and not contians string "RCD"; 
     biz_name = tmp_biz_name.substr(0,start);
  //LOG(INFO) << "RemoveOtherAction5()::DoPost() subtype from " << tmp_biz_name << " to type " << biz_name;
  }
  //LOG(INFO) << "RemoveOtherAction5()::DoPost() result type " << biz_name;   
  if (biz_name.empty()) {
    return cwf::HC_OK;
  }
  long guest = request->get("guest", -1);
  if (guest == -1) {
    return cwf::HC_OK;
  }

  try {
	  if (biz_name == "FOF") {
		RcdMessageServiceAdapter::instance().AddSgBlockAndRemoveFoFCache(host->id(), guest, biz_name);
	  } else {
		RcdMessageServiceAdapter::instance().AddSgBlockAndRemoveRcdCache(host->id(), guest, biz_name);
	}
  } catch (Ice::Exception& e) {
    LOG(LERROR) << e.what();
  } catch (...) {
    LOG(LERROR) << "RemoveOtherAction5()::DoPost() .. exception host:" << host->id() << " guest:" << guest << " biz_name:" << biz_name;
  }

  return cwf::HC_OK;
}

cwf::HttpStatusCode GetPhotoFriendAction5::DoPost(cwf::Request* request, cwf::Response* response, cwf::User* host) {
        std::ostringstream oss;
        oss << "GetPhotoFriendAction5::DoPost() host:" << host->id();
        PTIME(pt, oss.str(), true, 0);
        std::vector<LayoutPtr> layouts = PrepareFriendsLayout(host->id());
        std::vector<int> user_id_seq = GetUserIdSeq(&layouts);
        UserCacheMap users;
        UserCacheMap usercaches_map;
        try {
                usercaches_map = xce::usercache::UserCacheAdapter::instance().GetUserCache(user_id_seq);
                FilterUserCache(usercaches_map, users);
        } catch (...) {
                LOG(LERROR) << "xce::usercache::UserCacheAdapter::instance().GetUserCache() exception...  host:" << host->id();
        }

        const std::string bizType("Rcd4NewFriendsLayout");
        std::string out = "";                           //output of result
        BOOST_FOREACH(LayoutPtr layout, layouts) {
                        out += layout->DoLayout(&users);
        }
        response->WriteRaw(out);
        return cwf::HC_OK;
}


std::vector<LayoutPtr> GetPhotoFriendAction5::PrepareFriendsLayout(int uid) {
        int limit = -1;                 //get all
        std::vector<LayoutPtr> layout_seq;
        RecommendItemSeq item_seq;
        try {
                item_seq = RecommendationPhotofriendAdapter::instance().GetFriendRecommendByHome(uid, limit);
        } catch (Ice::Exception& e) {
                LOG(LERROR) << "GetPhotoFriendAction5::PrepareFriendsLayout Ice::Exception " << e.what();
                return layout_seq;
        } catch (...) {
                LOG(LERROR) << "GetPhotoFriendAction5::PrepareFriendsLayout unknown Exception";
                return layout_seq;
                }
        int currentRcdFriendsNum = 0;

//add for hadoop log.
	std::map<std::string, int> type2count_map1;

//        OutputShowLog(uid, item_seq);           //output show item_seq
	BOOST_FOREACH (const RecommendItem& item, item_seq) {
                layout_seq.push_back(LayoutPtr(new PhotoFriendLayout(uid, item)));
		++type2count_map1[item.type + "_PhotoFriend"]; 
        }

        OutputLogStatsByProduct(uid, "RecPhotoFriend", type2count_map1);             //output hadoop log of RecFriend
        LOG(INFO) << "GetPhotoFriendAction5::PrepareFriendsLayout() user_id:" << uid << " items.size():" << item_seq.size();
//end for hadoop log.
        return layout_seq;
}

std::vector<int> GetPhotoFriendAction5::GetUserIdSeq(std::vector<LayoutPtr>* layouts) {
        std::vector<int> id_seq;
        std::set<int> id_set;
        BOOST_FOREACH(LayoutPtr layout, *layouts) {
                std::set<int> t_set = layout->UserIdSet();
                id_set.insert(t_set.begin(), t_set.end());
        }
        id_seq.insert(id_seq.end(), id_set.begin(), id_set.end());
        return id_seq;
}

void GetPhotoFriendAction5::FilterUserCache(const UserCacheMap& map1, UserCacheMap& map2) {
        UserCacheMap::const_iterator it = map1.begin();
        for (; it != map1.end(); ++it) {
                int status = it->second->status();
                if (status != 6 && status != 7 && status != 8) {
                        map2.insert(std::make_pair(it->first, it->second));
                }
        }
}

//product|uid|count1|count2|count3|...|type1|type2|type3|...；其中，type个数和count个数对应。
void GetPhotoFriendAction5::OutputLogStatsByProduct(int userid,
  const std::string& biz,
  const std::map<std::string, int>& type2count) {
        std::string s_output = biz + "_numOfRec|";     //prefix
        try {
                std::string s_userid = boost::lexical_cast<std::string>(userid);                //hostid
                std::string s_tempcount = "";
                std::string s_temptype  = "";
                for (std::map<std::string, int>::const_iterator iter = type2count.begin();
                                iter != type2count.end(); ++iter) {
                        std::string s_count = boost::lexical_cast<std::string>(iter->second);
                        s_tempcount += "|" + s_count;
                        s_temptype  += "|" + iter->first;
                }
                s_output += s_userid + s_tempcount + s_temptype + "\n";
                if (!s_output.empty()) {
                        MyUtil::TaskManager::instance().execute(new LogStatsTask(userid, s_output));                    //stats total recommendation by biz
			MyUtil::TaskManager::instance().execute(new XLogTask(userid, s_output));			//stats total recommendation by biz
                }
        } catch (boost::bad_lexical_cast& e) {
                LOG(LERROR) << "[GetPhotoFriendAction5] OutputLogStatsByProduct bad_lexical_cast" << e.what();
        } catch (...) {
                LOG(LERROR) << "[GetPhotoFriendAction5] OutputLogStatsByProduct unknown exception";
        }
}

void GetPhotoFriendAction5::OutputShowLog(int uid, const RecommendItemSeq& item_seq) {
        std::ostringstream oss;
        oss << "GetPhotoFriendAction5 OutputShowLog uid:" << uid;
        for (RecommendItemSeq::const_iterator iter = item_seq.begin();
                        iter != item_seq.end(); ++iter) {
                oss << "|" << iter->id;
        }

        LOG(INFO) << oss.str();
}

cwf::HttpStatusCode RemovePhotoFriendAction5::DoPost(cwf::Request* request, cwf::Response* response, cwf::User* host) {
        std::ostringstream oss;
        oss << "RemovePhotoFriendAction5::DoPost() host:" << host->id();
        PTIME(pt, oss.str(), true, 0);

  	std::string tmp_biz_name = request->get("type");              //FFW, FFUCR => FFW_E1, FFUCR_O
  	std::string biz_name     = tmp_biz_name;
	LOG(INFO)<<"tmp_biz_name"<<tmp_biz_name;
  	int  start = tmp_biz_name.find("_");
  	int  end   = tmp_biz_name.rfind("_");
  	int  rcdPos= tmp_biz_name.find("RCD");

  	if((rcdPos<0) && (start>0) && (start==end)){ //Filter: contians only one string "_" and not contians string "RCD";
     		biz_name = tmp_biz_name.substr(0,start);
  	}
  	if (biz_name.empty()) {
    		return cwf::HC_OK;
  	}
  	long guest = request->get("guest", -1);
  	if (guest == -1) {
    		return cwf::HC_OK;
  	}

  	try {
        	if (biz_name == "FOF") {
                	RcdMessageServiceAdapter::instance().AddSgBlockAndRemoveFoFCache(host->id(), guest, biz_name);
		} else {
                	RcdMessageServiceAdapter::instance().AddSgBlockAndRemoveRcdCache(host->id(), guest, biz_name);
        	}
  	} catch (Ice::Exception& e) {
    		LOG(LERROR) << e.what();
  		} catch (...) {
    	LOG(LERROR) << "RemoveAction5()::DoPost() .. exception host:" << host->id() << " guest:" << guest << " biz_name:" << biz_name;
  	}

  	return cwf::HC_OK;
}

}} // end xce::socialgraph


