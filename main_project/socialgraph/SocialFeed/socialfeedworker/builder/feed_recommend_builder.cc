#include "feed_recommend_builder.h"

#include "commondbhelper.h"
#include "item.pb.h"     
#include "socialgraph/socialgraphutil/mc_adapter.h"

#include "Singleton.h"
#include "tripod2/cc/core/tripod_client.h"
#include <boost/lexical_cast.hpp>
#include "OceCxxAdapter/src/SocialGraphCommonTripodAdapter.h"

namespace xce {
namespace socialgraph {

using namespace xce::tripod;
void  FeedRecommendBuilder::Preload() {
	//to-do	
	//prepare common data
}

RecommendItemSeq FeedRecommendBuilder::Prepare() {
	return GetFeedRecommend();
}

void FeedRecommendBuilder::Fliter(RecommendItemSeq& item_list) {
	//to-do
	//fliter bad case
}

void FeedRecommendBuilder::Rank(RecommendItemSeq& item_list) {
	////random recommenditem list
	//std::random_shuffle(item_list.begin(), item_list.end());
	//size limited
	if (limit_ > 0 && 
			item_list.size() > limit_) {		
		item_list.resize(limit_);
	}
}

void FeedRecommendBuilder::Explanation(RecommendItemSeq& item_list) {

}

int  FeedRecommendBuilder::SendCountStrategy(int timestamp, int count_remain){
     int count_send = 0;
     if (count_remain>0){
      /*  int     start = 0;
        int     end   = 12; 
        time_t  now   = time(&now);
        struct  tm* tm_now = localtime(&now);

        int   AP_count = count_sent/2;
        if ((tm_now->tm_hour>=0 && tm_now->tm_hour<12) && (AP_count%2==0)){ //now is morinig and data not sent
            count_send = (count_remain > 2) ? 2:count_remain;
        }else if((tm_now->tm_hour>=12 && tm_now->tm_hour<24) && (AP_count%2==1)){
            count_send = (count_remain > 2) ? 2:count_remain;
        }*/
        const int interver = 16*60*60;
        if((time(NULL) - timestamp) > interver){
            count_send = 1;
        }
     }
     return  count_send;
}

RecommendItemSeq FeedRecommendBuilder::GetFeedRecommend() {
	RecommendItemSeq  item_list;  //result of recommendation
	string value;
	string key;
        
	try {
		key = boost::lexical_cast<string>(userid_);
                key += "_sf";
	} catch (boost::bad_lexical_cast& e) {
                MCE_WARN("FeedRecommendBuilder::GetFeedRecommend boost::lexical_cast error: " << e.what() << "\n"); 
		return item_list;
	}
        try{
   	        SocialGraphCommonTripodAdapter::instance().Get(key, &value);
        }catch (Ice::Exception& e) {
                MCE_WARN("FeedRecommendBuilder::GetFeedRecommend Get feeds ice exception " << e.what());
        } catch (std::exception& e) {
                MCE_WARN("FeedRecommendBuilder::GetFeedRecommend Get feeds std exception " << e.what());
        } catch (...) {
                MCE_WARN("FeedRecommendBuilder::GetFeedRecommend Get feeds unknow exception");
        }
	if (!value.empty()) {
           ostringstream  ot;
	   ot << "FeedRecommendBuilder::GetFeedRecommend cachedfeed-key:" << key << " from cache,before proto-parse\n";
	   Items  sf_raw_items;
           Items  sf_remain_items;
           int    time_stamp = 0;
           int    count_send = 0;
	   if (sf_raw_items.ParseFromString(value)) {
              if (sf_raw_items.has_timestamp()){
                  time_stamp = sf_raw_items.timestamp();
                  count_send = SendCountStrategy(time_stamp, sf_raw_items.items_size());
              }
              if (count_send <= 0){
                  MCE_INFO("FeedRecommendBuilder::GetFeedRecommend count to send is zero for key " << key);
                  return  item_list;
              }
              int  counter = 0;
              ot << "FeedRecommendBuilder::GetFeedRecommend cachedfeed-key:" << key << " from cache,after proto-parse get feeds count:" << count_send << "\n"; 
	      BOOST_FOREACH(const Item& it, sf_raw_items.items()) {
	    	    if (counter++ < count_send) { //for feeds has to send 
	    		RecommendItem r;
                        r.commons.clear();
                        r.explanations.clear();
	    		r.id = it.id();
                        ot << "[sending]";
	    		BOOST_FOREACH(int i, it.fields()) {
	    		     r.commons.push_back(i);
                             ot << i << " ";
	    		}
                        ot << "mes:";
                        BOOST_FOREACH(const string& str, it.messages()){
                             r.explanations.push_back(str);
                             ot << " " << str;
                        }
	    		r.type = it.message();
	    		item_list.push_back(r);
                        ot << " type:" << r.type << "\n";
	    	     }else{//just copy 
                        Item* it2 = sf_remain_items.add_items();
                        it2->set_id(it.id());
                        ot << "[remained]";
                        BOOST_FOREACH(int i, it.fields()) {
                             it2->add_fields(i);
                             ot << i << " ";
                        }
                        ot << "mes:";
                        BOOST_FOREACH(const string& str, it.messages()){
                             it2->add_messages(str);
                             ot << " " << str;
                        }
                        it2->set_message(it.message());
                        ot << " type:" << it.message() <<"\n";
                    }               
	     }
             MCE_INFO(ot.str()); 
             ot.str("");
             //reset the remained feeds
             if (count_send > 0){
                try{
                   if (sf_remain_items.items_size() > 0){
                       value = "";
                       const int expired = 3*24*60*60;
                       int     byte_size = sf_remain_items.ByteSize();
                       if (byte_size > 0) {
                           sf_remain_items.set_timestamp(time(NULL));
                           sf_remain_items.SerializeToString(&value);
                           if (!SocialGraphCommonTripodAdapter::instance().Set(key, value, expired)){
                               MCE_INFO("FeedRecommendBuilder::GetFeedRecommend [remained] set remained feeds data cache for hostId: " << key << " failed!");
                           }
                       }
                   }else {
                       SocialGraphCommonTripodAdapter::instance().Remove(key);
                   }
                }catch (Ice::Exception& e) {
                  MCE_WARN("FeedRecommendBuilder::GetFeedRecommend Set remained feeds ice exception " << e.what());
                } catch (std::exception& e) {
                  MCE_WARN("FeedRecommendBuilder::GetFeedRecommend Set remained feeds std exception " << e.what());
                } catch (...) {
                  MCE_WARN("FeedRecommendBuilder::GetFeedRecommend Set remained feeds unknow exception");
                }
             }
          }
       }
       //std::random_shuffle(item_list.begin(), item_list.end());		//random strategy
       return  item_list;
}
}
}

