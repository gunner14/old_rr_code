#include "Singleton.h"
#include "Common.h"
#include "tripod2/cc/core/tripod_client.h"
#include <boost/lexical_cast.hpp>
#include "OceCxxAdapter/src/FeedInterfaceReplicaAdapter.h"
#include "OceCxxAdapter/src/TimeLineFeedReplicaAdapter.h"
#include "TimeLineFeed.h"
#include "FriendFetcher.h"
#include "ugc_feed_builder.h"

namespace xce {
namespace socialgraph {
namespace guideugc {

using namespace MyUtil;
using namespace xce::tripod;
using namespace xce::feed::timeline;

/*
*load 2*3*31 usgc feeds for one guide user per month, for whose friends num is less than 10.
*/
void  GuideUserUGCFeed::LoadUGCFeeds(int limit){
      try {
             FriendFetcher fetcher;
             IntSeq        friends_list;
             fetcher.Read(_userid, friends_list);
             if (friends_list.size() < 10){// friends count must smaller than 10 
                 time_t  now; 
                 struct  tm* tm_now = NULL;
                 time(&now);
                 tm_now = localtime(&now);
                 if (!tm_now){
                     return;
                 }
                 int  year  = 1900 + tm_now->tm_year - 1;
                 int  month = tm_now->tm_mon + 1;
                 MyUtil::IntSeq  stypes;
                 stypes.push_back(601);
                 stypes.push_back(701);
                 stypes.push_back(709);
                 MyUtil::LongSeq  feeds;
                 for (IntSeq::const_iterator iter = friends_list.begin();
                      iter != friends_list.end(); iter++){
                      TimeLineFeedResult result = TimeLineFeedReplicaAdapter::instance().QueryFeedInfoByStype(*iter, year, month, stypes, 0, 100);
                      if (result.feeds.size() > 0){//add to feeds array
                          for (TimeLineFeedInfoSeq::const_iterator iter = result.feeds.begin();
                               iter != result.feeds.end(); iter++){
                               for (MyUtil::LongSeq::const_iterator iter2 = iter->feeds.begin();
                                    iter2 != iter->feeds.end(); iter2++){
                                  /*int  high = (*iter2)/1000000000;
                                    MCE_INFO("FEED high is "<< high << " fid " << *iter2);
                                    if (high > 28){*/
                                        feeds.push_back(*iter2); 
                                  //}
                               }
                          }  
                      }      
                 }//store those feeds
                 std::random_shuffle(feeds.begin(), feeds.end());
                 if (limit > 0 &&  feeds.size() > limit) {
                     feeds.resize(limit);
                 }
                 _feeds.insert(_feeds.begin(), feeds.begin(), feeds.end());
                 MCE_INFO("GuideUserUGCFeed::LoadUGCFeeds get " << feeds.size() <<" ugs feeds,uid:" << _userid << ",year:"<< year << ",month:" << month);
             }
      } catch (Ice::Exception& e) {
           MCE_WARN("GuideUserUGCFeed::LoadUGCFeeds get  feeds ice exception " << e.what());
      } catch (std::exception& e) {
           MCE_WARN("GuideUserUGCFeed::LoadUGCFeeds get  feeds std exception " << e.what());
      } catch (...) {
           MCE_WARN("GuideUserUGCFeed::LoadUGCFeeds get  feeds unknow exception");
      }
      _next = 0;//it's a tag to tell whether it has done loading action
}

/*
* select 3 feeds to push away, if not ever loaded ugc feeds,firstly load it, then change the tag about loading.
*/
int  GuideUserUGCFeed::PushUGCfeeds(int limit){
      int ret = 0;
      if (_next == -1){
          LoadUGCFeeds(MAX_UGC_FEED_SIZE);//2*3*31
      }
      MyUtil::LongSeq  feedids;
      int  count = SendStrategy(limit);
      ret  = count;
      try{
           if (count > 0){
               for (; count > 0; count--){
                   feedids.push_back(_feeds[_next++]);
                   MCE_INFO("GuideUserUGCFeed::PushUGCfeeds push feedsid " << _feeds[_next] << " for uid " << _userid); 
               }     
               xce::feed::FeedInterfaceReplicaAdapter::instance().addFriendFeedToGuide(_userid, feedids);
           }
      } catch (Ice::Exception& e) {
          MCE_WARN("GuideUserUGCFeed::PushUGCfeeds push feeds ice exception " << e.what());
      } catch (std::exception& e) {
          MCE_WARN("GuideUserUGCFeed::PushUGCfeeds push feeds std exception " << e.what());
      } catch (...) {
          MCE_WARN("GuideUserUGCFeed::PushUGCfeeds push feeds unknow exception");
      }
      return  ret;
}

/*
*send strategy about how many to send, normally less than 3.
*/
int   GuideUserUGCFeed::SendStrategy(int limit){
      int  ret = 0;
      time_t  now;
      struct  tm* tm_now;
      time(&now);
      tm_now = localtime(&now);
      if (!tm_now){
          return  ret;
      }
      if (_day != tm_now->tm_mday && _next >=0 && _next < _feeds.size()){
          _day = tm_now->tm_mday;
          ret  = (_feeds.size() - _next) >= 3 ? 3:(_feeds.size() - _next);
      }
      return  ret;
}

/*
* manager the guides' feeds,reset to clean feeds of very guide user's ugc feeds 
*/
int   GuideUserUGCFeedMgr::PushUGCFeeds(int userid, int limit){
      int  ret = 0;    
      if (IsNeedReset()){
          Update();
      }
      try{
           IceUtil::RWRecMutex::WLock  lock(_monitor);
           std::map<int, GuideUserUGCFeed>::iterator iter = _guideugcfeeds.find(userid);
           if (iter == _guideugcfeeds.end()){
              GuideUserUGCFeed  ugcfeeds(userid);
              pair<std::map<int, GuideUserUGCFeed >::iterator, bool> result;
              result = _guideugcfeeds.insert(make_pair<int, GuideUserUGCFeed >(userid,ugcfeeds));
              if(result.second){
                 iter = result.first;       
              }
           }
           if (iter != _guideugcfeeds.end()){
              ret = iter->second.PushUGCfeeds(3);
           }           
      } catch (Ice::Exception& e) {
           MCE_WARN("GuideUserUGCFeed::LoadUGCFeeds get  feeds ice exception " << e.what());
      } catch (std::exception& e) {
           MCE_WARN("GuideUserUGCFeed::LoadUGCFeeds get  feeds std exception " << e.what());
      } catch (...) {
           MCE_WARN("GuideUserUGCFeed::LoadUGCFeeds get  feeds unknow exception");
      }
      return  ret;
}

/*
*  check whether it needs to reset 
*/
bool  GuideUserUGCFeedMgr::IsNeedReset(){
      bool    ret = false;
      time_t  now        = time(&now);
      struct  tm* tm_now = localtime(&now);
      if (tm_now!=NULL){
         if(tm_now->tm_year != _year || tm_now->tm_mon != _month){
            ret = true;
            _year  = tm_now->tm_year;
            _month = tm_now->tm_mon;
         }         
      }
      return  ret;
}

/*
* celan action
*/
void  GuideUserUGCFeedMgr::Update(){
      IceUtil::RWRecMutex::WLock  lock(_monitor); 
      _guideugcfeeds.clear(); 
}

}}}

