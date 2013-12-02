/*=====================================================
 *
 *    FileName:SecondDegreeFriendSearchManagerI.cpp
 *
 *    Created:2012年11月23日
 *
 *    Author:min.chen1@renren-inc.com
 *
 *=====================================================
 * */
#include "BuddyByIdReplicaAdapter.h"
#include "SecondDegreeFriendSearchManagerI.h"
#include <cmath>
#include <boost/lexical_cast.hpp>
#include "IceExt/src/Channel.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include <boost/foreach.hpp>

using namespace xce::serverstate;
using namespace xce::seconddegreefriendsearch;
using namespace com::renren::secondfrienddata;
using namespace xce::buddy::adapter;


// =============== MyUtil::initialize ====================================
void MyUtil::initialize() {
    ServiceI& service = ServiceI::instance();
    service.registerObjectCache(SecondDegreeFriendSearchManagerI::instance().SECONDFRIENDSUGGEST_DATA,"SF");
    service.getAdapter()->add(&SecondDegreeFriendSearchManagerI::instance(), service.createIdentity("M",""));
    ServerStateSubscriber::instance().initialize("ControllerSecondDegreeFriendSuggestion",&SecondDegreeFriendSearchManagerI::instance(), 10, 5, new MyUtil::St0SearchChannel);

    string zk = service.getCommunicator()->getProperties()->getProperty("SecondDegreeFriendsSuggestion.ZK");
    string loaderpath = service.getCommunicator()->getProperties()->getProperty("SecondDegreeFriendsSuggestion.LoadPath");
    int dataNum = service.getCommunicator()->getProperties()->getPropertyAsInt("SecondDegreeFriendsSuggestion.DataNum");
    SecondDegreeFriendSearchManagerI::instance().init(zk,loaderpath,dataNum);
}


//============SecondDegreeFriendSearchManagerI==============
SecondDegreeFriendSearchManagerI::SecondDegreeFriendSearchManagerI():SECONDFRIENDSUGGEST_DATA(0),TIMEOUT_CACHE(2*60*60),SECOND_DEGREE_FRIEND("second_degree_friend_name"),DB_FRIENDINPUT_PINYIN("friendinput_pinyin") {
  adapter.Init(zk_,loaderpath_);

  Statement sql;
  sql << "SELECT hanzi, pinyin FROM search_pinyin";
  PinYinResultHandler handler(dict_);
  QueryRunner(DB_FRIENDINPUT_PINYIN, CDbRServer).query(sql, handler);
}

void SecondDegreeFriendSearchManagerI::init(string& zk,string& loaderpath,int dataNum) {
  zk_ = zk;
  loaderpath_ = loaderpath;
  dataNum_ = dataNum;
};

SuggestionResultPtr SecondDegreeFriendSearchManagerI::search(int userid,const string& query,int begin,int limit, const xce::generalsuggestion::StrSeq&, const Ice::Current& crt) {
  MCE_INFO("[SecondDegreeFriendSearchManagerI],search for userid="<<userid<<",query="<<query<<",limit="<<limit<<",query size:"<<query.size());

    SuggestionResultPtr result = new SuggestionResult;
    if(limit <= 0) {
      MCE_WARN("limit is lower than zero!");
      return result;
     }
    int len = FriendSuggestUtil::getStrLen(query);
    MCE_DEBUG("query len is :"<<len);
    if(len > 15 ||len == 0) {
      MCE_WARN("query len is zero or large than 15~");
      return result;
    } else {
      SecondDegreeFriendSearchDataIPtr server = ServiceI::instance().findObject<SecondDegreeFriendSearchDataIPtr>(SECONDFRIENDSUGGEST_DATA, (long)userid);
      if(server != NULL){
        time_t old = server->getTime();
        int duration = time(NULL) - old;
        MCE_DEBUG("duration is:"<<duration);
        if(duration >= TIMEOUT_CACHE)  {//remove and add
          MCE_DEBUG("cache timeout~~");
          ServiceI::instance().removeObject(SECONDFRIENDSUGGEST_DATA, (long)userid);
          createObject(userid,server);
        }
      } else {//add
        createObject(userid,server);
      }
      MyUtil::IntSeq pids;
      if(server == NULL) {
        MCE_DEBUG("the server is null,createObject failure~");
        return result;
      }
      pids = server->search(query,limit);


      if(pids.size() == 0) {
        MCE_INFO("getSecondDegreeFriendSearchManagerI result is null~");
        return result;
      } else {
        MCE_INFO("res is not null~ and res.size()="<<pids.size()<<"FriendInputManagerI::getFriendInput done hostID= "<<userid<<" query="<<query<<"        limit="<<limit << " res:"  << PrintHelper::Print(pids));
      }

      initComNum(userid);

      IndexItemPtr p = new IndexItem;
      p->type = SECOND_DEGREE_FRIEND;
      MyUtil::Str2StrMapSeq docs_; 

      string id= "",commonNum = "";
      map<int,short>::iterator comNumIt;
      for(vector<int>::iterator it = pids.begin();it != pids.end();it++) {
        id = boost::lexical_cast<string>(*it);
        comNumIt = id2ComNum.find(*it);
        if(comNumIt != id2ComNum.end()) {
          commonNum = boost::lexical_cast<string>(comNumIt->second);
        }
        map<string,string> resItem;
        resItem["id"] = id;
        resItem["name"] = "";
        resItem["commonNum"] = commonNum;
        docs_.push_back(resItem);
      }
      p->docs = docs_;
      result->results.push_back(p);
    }
    return result;
};


void SecondDegreeFriendSearchManagerI::load(int userid,const IndexItemSeq& indexItems, const Str2IntMap& config,const Ice::Current& crt) {
  return ;
}

//==============================================================
void SecondDegreeFriendSearchManagerI::initComNum(int hostID) {
  map<int, map<int,short> > res;
  try {
    res = adapter.GetRelationV2(hostID,2,"SecondDegreeFriendsSuggestion");
  } catch(...) {
    MCE_WARN("test zk. error,adapter.GetRelateionV2 error~");
  }

  map<int, map<int,short> >::iterator it = res.begin();
  if(res.size() != 1 || it->first != 2) {
    MCE_WARN("get result from adapter.GetRelationV2 is wrong~");
    return;
  }
  if((res[2].size() == 0)) {
    MCE_WARN("id:"<<hostID<<" has none friend~");
    return;
  }
  id2ComNum = res[2];
}

void SecondDegreeFriendSearchManagerI::createObject(int hostID,SecondDegreeFriendSearchDataIPtr& server) {
    server = new SecondDegreeFriendSearchDataI(hostID,dict_,adapter);
    server->buildIndex(dataNum_);
    ServiceI::instance().addObject(server, SECONDFRIENDSUGGEST_DATA, (long)hostID);
}
