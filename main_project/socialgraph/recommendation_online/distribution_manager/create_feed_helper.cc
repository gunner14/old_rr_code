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
	MCE_INFO("[CreateFeedHelper] CreatePageFeedRecommendation userid:" << userid);
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

	result_map["title.comment"] = "猜您可能感兴趣的公共主页";			//title detail
	result_map["more"] = "http://page.renren.com";				//more link
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
		std::string explain = "not set";
		//std::string tag = item_seq.at(i).experiment;
		std::string tag = "E1";
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
		result_map[prefix + "mainimg"] = maining;
		result_map[prefix + "explain"] = explain;
		result_map[prefix + "logTag"] = tag;
		result_map[prefix + "link"] = page_link;
	}

	try {
    		if (!xce::feed::FeedCreatorAdapter::instance().Create(1210, 1, result_map)) {
        		MCE_INFO("Push RecommendInfo Page RecommendItem Error : " << userid);
    		} else {
        		MCE_INFO("Push RecommendInfo Page RecommendItem Success : " << userid);
    		}
  	} catch (Ice::Exception& e) {
    		MCE_WARN("CreateFeedHelper::CreatePageFeedRecommendation Exception : " << e.what());
  	} catch (std::exception& e) {
    		MCE_WARN("CreateFeedHelper::CreatePageFeedRecommendation Exception : " << e.what());
  	} catch (...) {
      		MCE_WARN("CreateFeedHelper::CreatePageFeedRecommendation Unknown Exception"); 
  	}
	//Print(result_map);
	return true;
}

void CreateFeedHelper::Print(const std::map<std::string, std::string>& source) {
	for (std::map<std::string, std::string>::const_iterator iter = source.begin();
			iter != source.end(); ++iter) {
	//	std::cout << iter->first << " : " << iter->second << std::endl;
		MCE_INFO(iter->first << " : " << iter->second);	
	}
}

}
}
