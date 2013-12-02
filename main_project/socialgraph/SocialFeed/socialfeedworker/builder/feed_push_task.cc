#include "commondbhelper.h"
#include "Singleton.h"
#include "tripod2/cc/core/tripod_client.h"
#include <boost/lexical_cast.hpp>
#include "UserNetwork.h"
#include "OceCxxAdapter/src/UserUrlInfo.h"
#include "OceCxxAdapter/src/FeedCreatorAdapter.h"
#include "OceCxxAdapter/src/UserNameTripodAdapter.h"
#include "OceCxxAdapter/src/UserWorkplaceTripodAdapter.h"
#include "OceCxxAdapter/src/UserUrlTripodAdapter.h"
#include "OceCxxAdapter/src/UserUniversityTripodAdapter.h"
#include "socialgraph/socialgraphd/headurl.h"
#include "feed_recommend_builder.h"
#include "feed_push_task.h"

using namespace  xce;
using namespace  xce::socialgraph;
using namespace  xce::adapter;
using namespace  xce::adapter::username;
using namespace  xce::adapter::userurl;

//call the bulider method to push the feed
void  FeedPushTask::PushFeed(xce::socialgraph::RecommendItemSeq& feedseq){
           //exchage the result to stand format and push to feed group
          std::map<std::string, std::string> data;
          MyUtil::Str2StrMap  props;   
          pair<map<std::string,std::string >::iterator, bool> result;
          std::string  key;
          std::string  value;
          int  tag = 1;
          int  seq = 0;
          int  srcfrom = 0;
          const int MAX_SRC_NUM = 5;
          std::string  strSeq;
          ostringstream   fo; 
          try{

             for (RecommendItemSeq::const_iterator it = feedseq.begin(); it != feedseq.end(); it++){
                  props.clear();
                  tag = 1;
                  seq = 0;
                  srcfrom = 0;
                  BOOST_FOREACH(int i, it->commons) { //splited by -1 in the array and no -1 in tail
                    if (-1==i){
                        tag ++;
                        seq = 0;
                        continue;
                    }else  if (1 == tag){//Fs
                      if(seq < MAX_SRC_NUM){
                         strSeq = boost::lexical_cast<std::string>(seq);
                         key    = "src." + strSeq + ".id";
                         value  = boost::lexical_cast<std::string>(i);
                         result = props.insert(make_pair<std::string, std::string>(key, value));
                         fo << key << " " << value << " ";
                         key    = "src." + strSeq + ".name";
                         UserNameInfoPtr namePtr = UserNameTripodAdapter::instance().Get(i);
                         value  = namePtr->name();
                         result = props.insert(make_pair<std::string, std::string>(key, value));
                         fo << key << " " << value << " ";
                         seq ++;
                         if (srcfrom <= 0){
                             srcfrom = i;                     
                         }
                      }
                   }else if (2 == tag){//FFs
                      strSeq = boost::lexical_cast<std::string>(seq);
                      key    = "target." + strSeq + ".id";
                      value  = boost::lexical_cast<std::string>(i);
                      result = props.insert(make_pair<std::string, std::string>(key, value));
                      fo << key << " " << value << " ";
                      key    = "target." + strSeq + ".name";
                      UserNameInfoPtr namePtr = UserNameTripodAdapter::instance().Get(i);
                      value  = namePtr->name();
                      result = props.insert(make_pair<std::string, std::string>(key, value));
                      fo << key << " " << value << " ";
                      key    = "target." + strSeq + ".mainimg";
                      UserUrlInfoPtr urlPtr = UserUrlTripodAdapter::instance().Get(i);
                      value  = xce::HeadUrl::FromPart(urlPtr->mainUrl());
                      result = props.insert(make_pair<std::string, std::string>(key, value));
                      fo << key << " " << value << " ";
                      key    = "target." + strSeq+ ".explain2";//univercity
                      mop::hi::svc::model::UniversityInfoSeq ptrme = UserUniversityTripodAdapter::instance().getUniversityInfoSeq(userid_);
                      mop::hi::svc::model::UniversityInfoSeq ptruser = UserUniversityTripodAdapter::instance().getUniversityInfoSeq(i);
                      int  max_enrollYear1 = 0;//try to get the name of same univercity, default is latest univercity
                      int  max_enrollYear2 = 0;
                      value = " ";//better not null
                      for (mop::hi::svc::model::UniversityInfoSeq::const_iterator it1 = ptruser.begin(); it1!= ptruser.end(); it1++){
                          if ((*it1)->enrollYear > max_enrollYear1){//default is name of latest univercity
                              max_enrollYear1 = (*it1)->enrollYear;
                              value = (*it1)->universityName;
                          }
                          for (mop::hi::svc::model::UniversityInfoSeq::const_iterator it2 = ptrme.begin(); it2 != ptrme.end(); it2++){
                              if (((*it1)->universityId == (*it2)->universityId) && ((*it2)->enrollYear > max_enrollYear2)){//some time in same univercity
                                  max_enrollYear2 = (*it2)->enrollYear;
                                  value = (*it2)->universityName; 
                              }
                          }
                      }
                      result = props.insert(make_pair<std::string, std::string>(key, value));
                      fo << key << " " << value << " ";
                      strSeq = boost::lexical_cast<std::string>(seq);
                      key    = "target." + strSeq + ".explain1";//workplace
                      mop::hi::svc::model::WorkplaceInfoSeq wk = UserWorkplaceTripodAdapter::instance().getWorkplaceInfoSeq(i);
                      int max_joinYear = 0;
                      value = " ";//try to get the latest workplace,default is empty but better not null
                      for (mop::hi::svc::model::WorkplaceInfoSeq::const_iterator it = wk.begin(); it != wk.end(); it++){
                          if ((*it)->joinYear > max_joinYear){ 
                              value = (*it)->workplaceName;
                              max_joinYear = (*it)->joinYear;
                          }  
                      }
                      result = props.insert(make_pair<std::string, std::string>(key, value));
                      fo << key << " " << value << " ";
                      seq ++;
                   }else if (3==tag){//CNs
                      strSeq = boost::lexical_cast<std::string>(seq);
                      key    = "target." + strSeq + ".explain3";//common friends
                      value  = boost::lexical_cast<std::string>(i) + "个共同好友";
                      result = props.insert(make_pair<std::string, std::string>(key, value));
                      fo << key << " " << value << " ";
                      seq ++;
                   }
                }
                key   = "time";
                value = boost::lexical_cast<std::string>(1000*time(NULL));
                result= props.insert(make_pair<std::string, std::string>(key, value));
                fo << key << " " << value << " ";
                key   = "from.tid";
                value = boost::lexical_cast<std::string>(userid_);
                result= props.insert(make_pair<std::string, std::string>(key, value));
                fo << key << " " << value << " ";
                key   = "from.id";
                value = boost::lexical_cast<std::string>(srcfrom);
                result= props.insert(make_pair<std::string, std::string>(key, value));
                fo << key << " " << value << " ";
                key   = "from.tinyimg";
                UserUrlInfoPtr urlPtr = UserUrlTripodAdapter::instance().Get(srcfrom);
                value  = xce::HeadUrl::FromPart(urlPtr->tinyUrl());
                result = props.insert(make_pair<std::string, std::string>(key, value));
                fo << key << " " << value << " ";
                key    = "from.count";//ff in feed.
                value  = boost::lexical_cast<std::string>(seq);
                result = props.insert(make_pair<std::string, std::string>(key, value));
                fo << key << " " << value << " ";
                MCE_INFO("FeedPushTask::PushFeed ->push one feed start:\n" << fo.str());
                fo.str("");
                if(seq >= 1){
                    if (!xce::feed::FeedCreatorAdapter::instance().Create(1207, 1, props)){
                        MCE_ERROR("FeedPushTask::PushFeed <-push one feed end:error!");
                    }else{
                        MCE_ERROR("FeedPushTask::PushFeed <-push one feed end:ok!");
                    }
                }else{
                    MCE_ERROR("FeedPushTask::PushFeed <-push one feed end:none!");
                }
            }
          }catch (Ice::Exception& e) {
                  MCE_WARN("FeedPushTask::PushFeed Set remained feeds ice exception " << e.what());
                } catch (std::exception& e) {
                  MCE_WARN("FeedPushTask::PushFeed Set remained feeds std exception " << e.what());
                } catch (...) {
                  MCE_WARN("FeedPushTask::PushFeed Set remained feeds unknow exception");
                }
} 
