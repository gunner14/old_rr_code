#include "friend_recommend_builder.h"

#include "commondbhelper.h"
#include "item.pb.h"     
#include "socialgraph/socialgraphutil/mc_adapter.h"

#include "Singleton.h"
#include "tripod2/cc/core/tripod_client.h"
#include <boost/lexical_cast.hpp>
#include "OceCxxAdapter/src/SocialGraphRcdTripodAdapter.h"

namespace xce {
namespace socialgraph {

using namespace xce::tripod;
void FriendRecommendBuilder::Preload() {
	//to-do	
	//prepare common data
}

RecommendItemSeq FriendRecommendBuilder::Prepare() {
	return GetRcdFriendRecommend();
}

void FriendRecommendBuilder::Fliter(RecommendItemSeq& item_list) {
	//to-do
	//fliter bad case
}

void FriendRecommendBuilder::Rank(RecommendItemSeq& item_list) {
	////random recommenditem list
	//std::random_shuffle(item_list.begin(), item_list.end());

	//size limited
	if (limit_ > 0 && 
			item_list.size() > limit_) {		
		item_list.resize(limit_);
	}
}

void FriendRecommendBuilder::Explanation(RecommendItemSeq& item_list) {
	//to-do
	//add explanation
}

RecommendItemSeq FriendRecommendBuilder::GetRcdFriendRecommend() {
	RecommendItemSeq item_list;							//result of recommendation
	srand(unsigned (time (NULL)));
	std::string value;
	string key;
	try {
		key = boost::lexical_cast<std::string>(userid_);
	} catch (boost::bad_lexical_cast& e) {
        MCE_WARN("boost::lexical_cast error: " << e.what() << "\n"); 
		return item_list;
	}
	SocialGraphRcdTripodAdapter::instance().Get(key, &value);
	if (!value.empty()) {
	  MCE_INFO("hostId: " << key << " Friends recommendation from tripod FOF source");
	  BinaryData fof_raw_data(value.begin(), value.end());
	  Items fof_raw_items;
	  if (fof_raw_items.ParseFromArray(fof_raw_data.data(), fof_raw_data.size())) {
	    BOOST_FOREACH(const Item& it, fof_raw_items.items()) {
	    	if (it.message() == "FOF") {		//new fof friends
	    		RecommendItem r;
	    		r.id = it.id();	
	    		r.count = it.field();
	    		BOOST_FOREACH(int i, it.fields()) {
	    			r.commons.push_back(i);
	    		}
	    		//r.weight = (Ice::Long)0.0l;
	    		try {
	    			r.weight = boost::lexical_cast<float>(it.messages(0));
	    		} catch (boost::bad_lexical_cast& e) {
	    			MCE_WARN("boost::lexical_cast error: " << e.what() << "\n"); 
	    		}
	    		r.type = it.message();
	    		item_list.push_back(r);
	    	}
	    }
	  }
    } else {
	    MCE_INFO("hostId: " << key << " Friends recommendation from memcached FFW or FFUCR source");
		const std::string biz_rcd = "RCD";
    	RecommendItemSeq tmp_list;
    	BinaryData rcd_raw_data;
    	bool fetch_rcd = BusinessCacheAdapter::GetInstance().Get(biz_rcd, userid_, rcd_raw_data);
    	if (!fetch_rcd) {
    		return tmp_list; 
    	}
    	Items rcd_raw_items;
    	if (!rcd_raw_items.ParseFromArray(rcd_raw_data.data(), rcd_raw_data.size())) {
    		return tmp_list; 
    	}
    	BOOST_FOREACH(const Item& it, rcd_raw_items.items()) {
		  if (it.message() == "FFW" ||			//common friends
    				it.message() == "FFUCR") {		//cluster friends
    			RecommendItem r;
    			r.id = it.id();	
    			r.count = it.field();
    			BOOST_FOREACH(int i, it.fields()) {
    				r.commons.push_back(i);
    			}
    			r.weight = -1.0f;
    			r.type = it.message();

    			tmp_list.push_back(r);
    		}
    	}
    	//std::random_shuffle(tmp_list.begin(), tmp_list.end());		//random strategy
    	item_list.insert(item_list.end(), tmp_list.begin(), tmp_list.end()); 
	}
    std::random_shuffle(item_list.begin(), item_list.end());		//random strategy
	return item_list;
}

}}
