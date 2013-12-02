#include "feed_recommend_builder.h"


#include "socialgraph/socialgraphutil/mcclient.h"
#include "socialgraph/socialgraphutil/algostuff.h"
#include "socialgraph/socialgraphutil/clogging.h"

#include "item.pb.h"     
#include "socialgraph/socialgraphutil/mc_adapter.h"
#include <time.h>
#include "Singleton.h"
#include "tripod2/cc/core/tripod_client.h"
#include <boost/lexical_cast.hpp>
#include "OceCxxAdapter/src/SocialGraphRcdTripodAdapter.h"
#include "OceCxxAdapter/src/SocialGraphRcdPageCfTripodAdapter.h"

namespace xce {
namespace socialgraph {

//using namespace xce::tripod;
void FeedRecommendBuilder::Preload() {
	//to-do	
	//prepare common data
}

RecommendItemSeq FeedRecommendBuilder::Prepare() {
  RecommendItemSeq item_list;
  srand(unsigned (time (NULL)));
  std::string value;
  std::string key;
  const int recommend_time_per_day = 2;
  uint32_t interval_time = 86400 / recommend_time_per_day;
  time_t timenow = time(NULL);
  key = xce::Comb("RCDPAGECF_", userid_);
  try {                                                                  
    SocialGraphRcdPageCfTripodAdapter::instance().Get(key, &value);
  } catch(Ice::Exception& e) {
    MCE_WARN("SocialGraphRcdPageCfTripodAdapter::instance().Get() Ice exception " << e.what() << "user_id : " << userid_ << "\n");
  } catch(...) {
    MCE_WARN("SocialGraphRcdPageCfTripodAdapter::instance().Get() other exception, user_id : " << userid_ << "\n");
  }
  if (!value.empty()) {
    MCE_INFO("hostId: " << key << " Page recommendation from tripod PageCF source");
    BinaryData page_data(value.begin(), value.end());
    Items page_items;
    if (page_items.ParseFromArray(page_data.data(), page_data.size())) {
      time_t timestamp = page_items.timestamp(); //读取Item的时间戳，为上一次推荐的时间
      if (timestamp == 0) { //时间戳为0，代表第一次推荐
        page_items.set_timestamp(timenow); //将当前第一次推荐时间保存到items中
        MCE_INFO("user_id : " << userid_ << " first time for recommendation, set the timestamp:" << timenow << "\n");
      } else if (timestamp + interval_time > timenow) { //当前时间比上一次推荐时间+时间间隔小，不进行推荐
        MCE_INFO("user_id : " << userid_ << " last recommendation time : " << timestamp << ", cancel recommend again\n");
        return item_list;
      } else if (timestamp + interval_time < timenow) { //当前时间比上一次推荐+时间间隔大，继续进行推荐，并保存当前时间
	MCE_INFO("user_id : " << userid_ << " last recommendation time : " << timestamp << ", recommend again, set the timestamp = " 
	  << timenow << "\n");
	page_items.set_timestamp(timenow);
      }
    }
    BOOST_FOREACH(const Item& it, page_items.items()) {
      if (it.message() == "PAGECF") {		//CF PAGE
        RecommendItem r;
        r.id = it.id();	
        r.count = it.field();
 	BOOST_FOREACH(int i, it.fields()) {
          r.commons.push_back(i);
        }
        //r.weight = (Ice::Long)0.0l;
        r.type = it.message();
        item_list.push_back(r);
      }
    }
    int byte_size = page_items.ByteSize();
    char ch[byte_size];
    page_items.SerializeToArray(ch, byte_size);
    std::string data;
    data.reserve(byte_size);
    data.assign(ch, ch + byte_size);
    MCE_INFO("userid : " << userid_ << " set back for change timestamp, item Size() : " << byte_size << "\n");
    try {
      SocialGraphRcdPageCfTripodAdapter::instance().Set(key, data, 60*60*24*7);
    } catch(Ice::Exception& e) {
      MCE_WARN("SocialGraphRcdPageCfTripodAdapter::instance().Get() Ice exception " << e.what() << "user_id : " << userid_ << "\n");
    } catch(...) {
      MCE_WARN("SocialGraphRcdPageCfTripodAdapter::instance().Get() other exception, " <<  "user_id : " << userid_ << "\n");
    }
    std::random_shuffle(item_list.begin(), item_list.end());
    return item_list;
  }   
  return item_list;
}  
   
void FeedRecommendBuilder::Fliter(RecommendItemSeq& item_list) {
          //to-do
          //fliter bad case
}  
   
void FeedRecommendBuilder::Rank(RecommendItemSeq& item_list) {
          ////random recommenditem list
    int item_size = item_list.size();
    if (limit_ > 0) {
      if (item_size > limit_) {
        item_size = limit_;
      }
    }
    if (item_size > MAX_SIZE) {
      item_list.resize(MAX_SIZE);
    } else {
      item_list.resize(item_size);
    } 
}  

void FeedRecommendBuilder::Explanation(RecommendItemSeq& item_list) {
	//to-do
	//add explanation
}

}
}
