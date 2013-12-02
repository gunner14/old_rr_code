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
//	return GetRcdFriendRecommend();
	return GetFinalFriendRecommend();
   
}

RecommendItemSeq FriendRecommendBuilder::GetFinalFriendRecommend() {
//      return GetRcdFriendRecommend();

        RecommendItemSeq resultFFucr = GetFFucrFriendRecommend();
				RecommendItemSeq resultRcd = GetRcdFriendRecommend();

        if (resultRcd.empty() && resultFFucr.empty()) {
	 				 return resultFFucr;
        } 

        if (resultRcd.empty() && !resultFFucr.empty()) {
           std::random_shuffle(resultFFucr.begin(), resultFFucr.end());
	 				 return resultFFucr;
        }

        if (!resultRcd.empty() && resultFFucr.empty()) {
           std::random_shuffle(resultRcd.begin(), resultRcd.end());
         	 return resultRcd;
        }

				if (!resultRcd.empty() && !resultFFucr.empty()) {
				  // remove the same items
						BOOST_FOREACH(const RecommendItem& r_ffucr, resultFFucr) {
							std::vector<RecommendItem>::iterator iter;
							for (iter = resultRcd.begin(); iter != resultRcd.end(); ++iter) {
								if (r_ffucr.id == (*iter).id) {
									resultRcd.erase(iter);
									break;
								}
							}
						}
          // add FFucr result before Rcd result 
          	int ffucrEndPos = resultFFucr.size();
         	 	resultRcd.insert(resultRcd.begin(), resultFFucr.begin(), resultFFucr.end());
          	std::random_shuffle(resultRcd.begin(), resultRcd.begin() + ffucrEndPos);
				  	std::random_shuffle(resultRcd.begin() + ffucrEndPos, resultRcd.end());
          	return resultRcd;
				}
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


RecommendItemSeq FriendRecommendBuilder::GetFFucrFriendRecommend() {
//        const std::string biz = "RCD";
        const std::string biz = "FFUCR";
        RecommendItemSeq item_list;                                                     //result of recommendation
        BinaryData raw_data;
		   	int rank = 0;

//				int64_t userid_code = userid_ + 100000000000000;
//				bool fetch = BusinessCacheAdapter::GetInstance().Get(biz, userid_code, raw_data);		
				
				bool fetch = BusinessCacheAdapter::GetInstance().Get(biz, userid_, raw_data);
        if (!fetch) {
                return item_list;
        }

        Items raw_items;
				try {
       		if (!raw_items.ParseFromArray(raw_data.data(), raw_data.size())) {
                return item_list;
       		}
				} catch(...) {
					return item_list;
				}

        BOOST_FOREACH(const Item& it, raw_items.items()) {
                        RecommendItem r;
                        r.id = it.id();
                        r.count = it.field();
                        BOOST_FOREACH(int i, it.fields()) {
                                r.commons.push_back(i);
                        }
                        r.weight = (Ice::Long)0.0l;
//                        r.type = it.message();
                        r.type = "FFUCR";
									//			for(int i = 0; i < it.messages_size(); i++) {
									//				r.explanations.push_back(it.messages(i));
									//			}
                        item_list.push_back(r);
																					
        }

        return item_list;
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
//    std::random_shuffle(item_list.begin(), item_list.end());		//random strategy
	return item_list;
}

}}
