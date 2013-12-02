#include "create_feed_helper.h"

#include "OceCxxAdapter/src/FeedCreatorAdapter.h"
#include "OceCxxAdapter/src/UserNameTripodAdapter.h"
#include "OceCxxAdapter/src/UserWorkplaceTripodAdapter.h"
#include "OceCxxAdapter/src/UserUrlTripodAdapter.h"
#include "OceCxxAdapter/src/UserUniversityTripodAdapter.h"

namespace xce {
namespace socialgraph {

using namespace MyUtil;
using namespace xce::serverstate;
using namespace com::xiaonei::xce;
using namespace xce::tripod;
using namespace xce::socialgraph;
using namespace xce::adapter;
using namespace xce::adapter::username;
using namespace xce::adapter::userurl;

bool CreateFeedHelper::CreatePageFeedRecommendation(int userid, const RecommendItemSeq& item_seq) {
	std::map<std::string, std::string> result_map;
	
	time_t nowtime = time(NULL);
	nowtime *= 1000;
	
	std::string strtime = "";
	std::string toid = "";
	std::string pagecount = "";
	std::string fromid = "600002246";

	try {
		strtime = boost::lexical_cast<std::string>(nowtime);
		toid = boost::lexical_cast<std::string>(userid);
		pagecount = boost::lexical_cast<std::string>(item_seq.size());
	} catch (boost::bad_lexical_cast& e) {
		MCE_WARN("CreateFeedHelper::CreatePageFeedRecommendation " << e.what());
		return false;
	} catch (...) {
		return false;
	}

	result_map["title.comment"] = "您应该喜欢这些公共主页";			//title detail
	result_map["more"] = "http://page.renren.com/renren";				//more link
	result_map["to.id"] = toid;																	//user recieve page recommendation
	result_map["to.count"] = pagecount;													//number of page recommendation
	result_map["from.id"] = fromid;															//fromid
	result_map["from.name"] = "人人网";
	result_map["from.link"] = "http://page.renren.com/renren";	//link of fromid
	result_map["from.tinyimg"] = "http://hdn.xnimg.cn/photos/hdn421/20090923/1935/tiny_ODII_19242g019116.jpg";	//head of fromid
	result_map["time"] = strtime;						//timestamp
	
	for (int i = 0; i < item_seq.size(); ++i) {
		std::string prefix = "target." + boost::lexical_cast<std::string>(i) + ".";

		std::string pageid = "";
		std::string pagename = "";
		std::string maining = "";
		std::string explain = "";
		std::string tag = item_seq.at(i).experiment;
		std::string page_link = "http://page.renren.com/";

		try {
			UserNameInfoPtr name_ptr = UserNameTripodAdapter::instance().Get(item_seq.at(i).id); 
			UserUrlInfoPtr url_ptr = UserUrlTripodAdapter::instance().Get(item_seq.at(i).id);

			pageid = boost::lexical_cast<std::string>(item_seq.at(i).id);
			pagename = name_ptr->name();
			maining = url_ptr->mainUrl();
			maining = album_util::AlbumUtil::getPhotoURL(maining.c_str());
			page_link += pageid;
		} catch (Ice::Exception& e) {
			MCE_WARN("CreateFeedHelper::CreatePageFeedRecommendation " << e.what());
			return false;
		} catch (...) {
			MCE_WARN("CreateFeedHelper::CreatePageFeedRecommendation Unknown Exception"); 
			return false;	
		}

		result_map[prefix + "id"] = pageid;
		result_map[prefix + "name"] = pagename;
		result_map[prefix + "maining"] = maining;
		result_map[prefix + "explain"] = explain;
		result_map[prefix + "logTag"] = tag;
		result_map[prefix + "link"] = page_link;
	}

	//try {
  //  if (!xce::feed::FeedCreatorAdapter::instance().Create(1210, 1, result_map)) {
  //      MCE_WARN("Push RecommendInfo Page RecommendItem Error : " << userid);
  //  } else {
  //      MCE_WARN("Push RecommendInfo Page RecommendItem Success : " << userid);
  //  }
  //} catch (Ice::Exception& e) {
  //  MCE_WARN("CreateFeedHelper::CreatePageFeedRecommendation Exception : " << e.what());
  //} catch (std::exception& e) {
  //  MCE_WARN("CreateFeedHelper::CreatePageFeedRecommendation Exception : " << e.what());
  //} catch (...) {
	//	MCE_WARN("CreateFeedHelper::CreatePageFeedRecommendation Unknown Exception"); 
  //}

	Print(result_map);
	return true;
}

bool CreateFeedHelper::CreateGuidePageRecommendation(int hostid, int friendid, const std::vector<int>& pageid_seq) {
	MCE_INFO("[CreateFeedHelper] CreateGuidePageRecommendation hostid:" << hostid 
					<< " friendid:" << friendid << " pageid_seq:" << pageid_seq.size());

	std::map<std::string, std::string> result_map;

	time_t nowtime = time(NULL);
	nowtime *= 1000;

	std::string strtime = "";
	std::string toid = "";
	std::string pagecount = "";
	std::string fromid = "";
	std::string fromname = "";
	std::string fromlink = "";
	std::string fromtinyurl = "";

	try {
		strtime = boost::lexical_cast<std::string>(nowtime);
		toid = boost::lexical_cast<std::string>(hostid);
		pagecount = boost::lexical_cast<std::string>(pageid_seq.size());

		fromid = boost::lexical_cast<std::string>(friendid);
		fromlink = "http://www.renren.com/" + boost::lexical_cast<std::string>(friendid) + "/profile";

		UserNameInfoPtr name_ptr = UserNameTripodAdapter::instance().Get(friendid); 
		fromname = name_ptr->name();

		UserUrlInfoPtr url_ptr = UserUrlTripodAdapter::instance().Get(friendid);
		fromtinyurl = url_ptr->tinyUrl();
		fromtinyurl = album_util::AlbumUtil::getPhotoURL(fromtinyurl.c_str());
	} catch (Ice::Exception& e) {
		MCE_WARN("[CreateFeedHelp] CreateGuidePageRecommendation Ice Exception : " << e.what());
		return false;
	} catch (...) {
		MCE_WARN("[CreateFeedHelp] CreateGuidePageRecommendation Unknown Exception");
		return false;
	}

	result_map["title.comment"] = "关注了这些公共主页";			//title detail
	result_map["more"] = "http://page.renren.com/renren";				//more link
	result_map["rcdtype"] = "page";				//type
	result_map["to.id"] = toid;																	//user recieve page recommendation
	result_map["to.count"] = pagecount;													//number of page recommendation
	result_map["from.id"] = fromid;															//fromid
	result_map["from.name"] = fromname;
	result_map["from.link"] = fromlink;	//link of fromid
	result_map["from.tinyimg"] = fromtinyurl;	//head of fromid
	result_map["time"] = strtime;						//timestamp

	for (int i = 0; i < pageid_seq.size(); ++i) {
		std::string prefix = "target." + boost::lexical_cast<std::string>(i) + ".";
		std::string pageidstr = "";
		std::string pagename = "";
		std::string maining = "";
		std::string explain = "";
		std::string tag = "";
		std::string page_link = "http://page.renren.com/";

		int pageid = pageid_seq.at(i);
		try {
			UserNameInfoPtr name_ptr = UserNameTripodAdapter::instance().Get(pageid); 
			UserUrlInfoPtr url_ptr = UserUrlTripodAdapter::instance().Get(pageid);

			pageidstr = boost::lexical_cast<std::string>(pageid);
			pagename = name_ptr->name();
			maining = url_ptr->mainUrl();
			maining = album_util::AlbumUtil::getPhotoURL(maining.c_str());
			explain = "NULL";
			tag = "RcdGuidePage";
			page_link += boost::lexical_cast<std::string>(pageid);
		} catch (Ice::Exception& e) {
			MCE_WARN("[CreateFeedHelp] CreateGuidePageRecommendation Ice Exception : " << e.what());
			continue;
		} catch (...) {
			MCE_WARN("[CreateFeedHelp] CreateGuidePageRecommendation Unknown Exception");
			continue;
		}

		result_map[prefix + "id"] = pageidstr;
		result_map[prefix + "name"] = pagename;
		result_map[prefix + "maining"] = maining;
		result_map[prefix + "explain"] = explain;
		result_map[prefix + "logTag"] = tag;
		result_map[prefix + "link"] = page_link;
	}
	try {
    if (!xce::feed::FeedCreatorAdapter::instance().Create(1211, 1, result_map)) {
        MCE_INFO("Push RecommendInfo Page RecommendItem Error : " << hostid);
    } else {
        MCE_INFO("Push RecommendInfo Page RecommendItem Success : " << hostid);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("CreateFeedHelper::CreateGuidePageRecommendation Exception : " << e.what());
  } catch (std::exception& e) {
    MCE_WARN("CreateFeedHelper::CreateGuidePageRecommendation Exception : " << e.what());
  } catch (...) {
		MCE_WARN("CreateFeedHelper::CreateGuidePageRecommendation Unknown Exception"); 
  }
	//Print(result_map);
	return true;
}

bool CreateFeedHelper::CreateGuideMinisiteRecommendation(int hostid, int friendid, const std::vector<int>& siteid_seq) {
	MCE_INFO("[CreateFeedHelper] CreateGuideMinisiteRecommendation hostid:" << hostid 
					<< " friendid:" << friendid << " siteid_seq:" << siteid_seq.size());

	std::map<std::string, std::string> result_map;

	time_t nowtime = time(NULL);
	nowtime *= 1000;

	std::string strtime = "";
	std::string toid = "";
	std::string sitecount = "";
	std::string fromid = "";
	std::string fromname = "";
	std::string fromlink = "";
	std::string fromtinyurl = "";

	try {
		strtime = boost::lexical_cast<std::string>(nowtime);
		toid = boost::lexical_cast<std::string>(hostid);
		sitecount = boost::lexical_cast<std::string>(siteid_seq.size());

		fromid = boost::lexical_cast<std::string>(friendid);
		fromlink = "http://www.renren.com/" + boost::lexical_cast<std::string>(friendid) + "/profile";

		UserNameInfoPtr name_ptr = UserNameTripodAdapter::instance().Get(friendid); 
		fromname = name_ptr->name();

		UserUrlInfoPtr url_ptr = UserUrlTripodAdapter::instance().Get(friendid);
		fromtinyurl = url_ptr->tinyUrl();
		fromtinyurl = album_util::AlbumUtil::getPhotoURL(fromtinyurl.c_str());
	} catch (Ice::Exception& e) {
		MCE_WARN("[CreateFeedHelp] CreateGuideMinisiteRecommendation Ice Exception : " << e.what());
		return false;
	} catch (...) {
		MCE_WARN("[CreateFeedHelp] CreateGuideMinisiteRecommendation Unknown Exception");
		return false;
	}

	result_map["title.comment"] = "关注了这些小站";			//title detail
	result_map["more"] = "http://zhan.renren.com/home";				//more link
	result_map["rcdtype"] = "site";				//type
	result_map["to.id"] = toid;																	//user recieve page recommendation
	result_map["to.count"] = sitecount;													//number of page recommendation
	result_map["from.id"] = fromid;															//fromid
	result_map["from.name"] = fromname;
	result_map["from.link"] = fromlink;	//link of fromid
	result_map["from.tinyimg"] = fromtinyurl;	//head of fromid
	result_map["time"] = strtime;						//timestamp

	for (int i = 0; i < siteid_seq.size(); ++i) {
		std::string prefix = "target." + boost::lexical_cast<std::string>(i) + ".";
		std::string siteidstr = "";
		std::string sitename = "";
		std::string maining = "";
		std::string explain = "";
		std::string tag = "";
		std::string site_link = "http://zhan.renren.com/zhan/";

		int siteid = siteid_seq.at(i);
		try {
			UserNameInfoPtr name_ptr = UserNameTripodAdapter::instance().Get(siteid); 
			UserUrlInfoPtr url_ptr = UserUrlTripodAdapter::instance().Get(siteid);

			siteidstr = boost::lexical_cast<std::string>(siteid);
			sitename = name_ptr->name();
			maining = url_ptr->mainUrl();
			maining = album_util::AlbumUtil::getPhotoURL(maining.c_str());
			explain = "NULL";
			tag = "RcdGuideSite";
			site_link += boost::lexical_cast<std::string>(siteid);
		} catch (Ice::Exception& e) {
			MCE_WARN("[CreateFeedHelp] CreateGuideMinisiteRecommendation Ice Exception : " << e.what());
			continue;
		} catch (...) {
			MCE_WARN("[CreateFeedHelp] CreateGuideMinisiteRecommendation Unknown Exception");
			continue;
		}
		result_map[prefix + "id"] = siteidstr;
		result_map[prefix + "name"] = sitename;
		result_map[prefix + "maining"] = maining;
		result_map[prefix + "explain"] = explain;
		result_map[prefix + "logTag"] = tag;
		result_map[prefix + "link"] = site_link;
	}
	try {
    if (!xce::feed::FeedCreatorAdapter::instance().Create(1211, 1, result_map)) {
        MCE_INFO("Push RecommendInfo Site RecommendItem Error : " << hostid);
    } else {
        MCE_INFO("Push RecommendInfo Site RecommendItem Success : " << hostid);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("CreateFeedHelper::CreateGuideMinisiteRecommendation Exception : " << e.what());
  } catch (std::exception& e) {
    MCE_WARN("CreateFeedHelper::CreateGuideMinisiteRecommendation Exception : " << e.what());
  } catch (...) {
		MCE_WARN("CreateFeedHelper::CreateGuideMinisiteRecommendation Unknown Exception"); 
  }
	//Print(result_map);
	return true;
}

bool CreateFeedHelper::CreateGuideMinisiteRecommendationWithUrl(int hostid, 
																											 					int friendid, 
																											 					const std::map<int, std::string>& siteid_map) {
	MCE_INFO("[CreateFeedHelper] CreateGuideMinisiteRecommendationWithUrl hostid:" << hostid 
					<< " friendid:" << friendid << " siteid_map :" << siteid_map.size());

	std::map<std::string, std::string> result_map;

	time_t nowtime = time(NULL);
	nowtime *= 1000;

	std::string strtime = "";
	std::string toid = "";
	std::string sitecount = "";
	std::string fromid = "";
	std::string fromname = "";
	std::string fromlink = "";
	std::string fromtinyurl = "";

	try {
		strtime = boost::lexical_cast<std::string>(nowtime);
		toid = boost::lexical_cast<std::string>(hostid);
		sitecount = boost::lexical_cast<std::string>(siteid_map.size());

		fromid = boost::lexical_cast<std::string>(friendid);
		fromlink = "http://www.renren.com/" + boost::lexical_cast<std::string>(friendid) + "/profile";

		UserNameInfoPtr name_ptr = UserNameTripodAdapter::instance().Get(friendid); 
		fromname = name_ptr->name();

		UserUrlInfoPtr url_ptr = UserUrlTripodAdapter::instance().Get(friendid);
		fromtinyurl = url_ptr->tinyUrl();
		fromtinyurl = album_util::AlbumUtil::getPhotoURL(fromtinyurl.c_str());
	} catch (Ice::Exception& e) {
		MCE_WARN("[CreateFeedHelp] CreateGuideMinisiteRecommendationWithUrl Ice Exception : " << e.what());
		return false;
	} catch (...) {
		MCE_WARN("[CreateFeedHelp] CreateGuideMinisiteRecommendationWithUrl Unknown Exception");
		return false;
	}

	result_map["title.comment"] = "关注了这些小站";			//title detail
	result_map["more"] = "http://zhan.renren.com/home";				//more link
	result_map["rcdtype"] = "site";				//type
	result_map["to.id"] = toid;																	//user recieve page recommendation
	result_map["to.count"] = sitecount;													//number of page recommendation
	result_map["from.id"] = fromid;															//fromid
	result_map["from.name"] = fromname;
	result_map["from.link"] = fromlink;	//link of fromid
	result_map["from.tinyimg"] = fromtinyurl;	//head of fromid
	result_map["time"] = strtime;						//timestamp

	int index = 0;
	for (std::map<int, std::string>::const_iterator iter = siteid_map.begin();
			iter != siteid_map.end(); ++iter) {
		std::string prefix = "target." + boost::lexical_cast<std::string>(index++) + ".";
		std::string siteidstr = "";
		std::string sitename = "";
		std::string maining = "";
		std::string explain = "";
		std::string tag = "";
		std::string site_link = "http://zhan.renren.com/";
		site_link += iter->second;

		int siteid = iter->first;
		try {
			UserNameInfoPtr name_ptr = UserNameTripodAdapter::instance().Get(siteid); 
			UserUrlInfoPtr url_ptr = UserUrlTripodAdapter::instance().Get(siteid);

			siteidstr = boost::lexical_cast<std::string>(siteid);
			sitename = name_ptr->name();
			maining = url_ptr->mainUrl();
			maining = album_util::AlbumUtil::getPhotoURL(maining.c_str());
			explain = "NULL";
			tag = "RcdGuideSite";
		} catch (Ice::Exception& e) {
			MCE_WARN("[CreateFeedHelp] CreateGuideMinisiteRecommendationWithUrl Ice Exception : " << e.what());
			continue;
		} catch (...) {
			MCE_WARN("[CreateFeedHelp] CreateGuideMinisiteRecommendationWithUrl Unknown Exception");
			continue;
		}
		result_map[prefix + "id"] = siteidstr;
		result_map[prefix + "name"] = sitename;
		result_map[prefix + "maining"] = maining;
		result_map[prefix + "explain"] = explain;
		result_map[prefix + "logTag"] = tag;
		result_map[prefix + "link"] = site_link;
	}
	try {
    if (!xce::feed::FeedCreatorAdapter::instance().Create(1211, 1, result_map)) {
        MCE_INFO("Push RecommendInfo Site RecommendItem Error : " << hostid);
    } else {
        MCE_INFO("Push RecommendInfo Site RecommendItem Success : " << hostid);
    }
  } catch (Ice::Exception& e) {
    MCE_WARN("CreateFeedHelper::CreateGuideMinisiteRecommendationWithUrl Exception : " << e.what());
  } catch (std::exception& e) {
    MCE_WARN("CreateFeedHelper::CreateGuideMinisiteRecommendationWithUrl Exception : " << e.what());
  } catch (...) {
		MCE_WARN("CreateFeedHelper::CreateGuideMinisiteRecommendationWithUrl Unknown Exception"); 
  }
	//Print(result_map);
	return true;
}

void CreateFeedHelper::Print(const std::map<std::string, std::string>& source) {
	for (std::map<std::string, std::string>::const_iterator iter = source.begin();
			iter != source.end(); ++iter) {
		//std::cout << iter->first << " : " << iter->second << std::endl;
		MCE_INFO(iter->first << " : " << iter->second);
	}
}

}
}
