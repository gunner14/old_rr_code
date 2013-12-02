#include "SecondDegreeFriendSearchManagerI.h"
#include <cmath>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include "IceExt/src/Channel.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

using namespace std;
using namespace MyUtil;
using namespace xce::serverstate;
using namespace com::xiaonei::xce;
using namespace xce::seconddegreefriendsearch;

// =============== MyUtil::initialize ====================================
void MyUtil::initialize() {
    MCE_INFO("init dict");
    Dictionary::instance().init();
    ServiceI& service = ServiceI::instance();
    service.getAdapter()->add(&SecondDegreeFriendSearchManagerI::instance(), service.createIdentity("M",""));
    MCE_INFO("init HA");
    //ServerStateSubscriber::instance().initialize("ControllerSecondDegreeFriendSuggestion",&SecondDegreeFriendSearchManagerI::instance(), 10, 5, new MyUtil::SearchChannel);
//    ServerStateSubscriber::instance().initialize("ControllerSecondDegreeFriendSuggestion",&SecondDegreeFriendSearchManagerI::instance(), 10, 5, new MyUtil::St0SearchChannel);
    ServerStateSubscriber::instance().initialize("ControllerSecondDegreeFriendSuggestion",&SecondDegreeFriendSearchManagerI::instance(), 10, 5, new MyUtil:: GadgetsChannel);
    MCE_INFO("init done");
}

//================ PinYinResultHandler ===================================
bool PinYinResultHandler::handle(mysqlpp::Row& res) const {
#ifndef NEWARCH
	string name=res["hanzi"].get_string();
	string pinyin=res["pinyin"].get_string();
#else
	string name=res["hanzi"].c_str();
	string pinyin=res["pinyin"].c_str();
#endif
	if(pinyin=="")
	  return false;
	pinyin_.insert(pinyin);
	map<string,StringSeqPtr>::iterator it=dict_.find(name);
	if(it!=dict_.end()){
		(it->second)->seq_.push_back(pinyin);
	}else{
		StringSeqPtr p = new StringSeq;	
		p->seq_.push_back(pinyin);
		dict_[name] = p;
	}
	return true;
}

// =============== SecondDegreeFriendSearchManagerI ===================================

SuggestionResultPtr SecondDegreeFriendSearchManagerI::search(int userid,const string& query,int begin,int limit, const StrSeq&, const Ice::Current& crt) {
    TimeCost tc = TimeCost::create("SearchQuery: " + query, 1);
    SuggestionResultPtr result = new SuggestionResult;
    string key = SecondDegreeFriendSearchDataIUtil::getKey(SECOND_DEGREE_FRIEND, userid);
    MCE_INFO("Search " + key);
    SearchDataIPtr sp = getCachedData(key);
    if (!sp) {
      load(SECOND_DEGREE_FRIEND, userid);
      sp = getCachedData(key);
    }
    if(sp){
      IndexItemPtr p = new IndexItem;
      p->type = SECOND_DEGREE_FRIEND;
      TimeCost tc2 = TimeCost::create("SearchType: [" + SECOND_DEGREE_FRIEND + "] Query: " + query, 1);
      p->docs = sp->search(query,begin,limit);
      tc2.step("Time Usage:");
      result->results.push_back(p);
      if(sp->timeout()) {
        reload(SECOND_DEGREE_FRIEND, userid);
      }
    }else{
      result->missIndexs.push_back(SECOND_DEGREE_FRIEND);
    }
    tc.step("Time Usage:");
    return result;
};

void SecondDegreeFriendSearchManagerI::load(int userid,const IndexItemSeq& indexItems, const Str2IntMap& config,const Ice::Current& crt) {
    if (indexItems.empty() || config.empty()) {
        ostringstream os;
        os << "load id: " << userid;
        TimeCost tc = TimeCost::create(os.str(), 1);
        load(SECOND_DEGREE_FRIEND, userid);
        tc.step("Time Usage:");
        return;
    }
    for(vector<IndexItemPtr>::const_iterator it = indexItems.begin(); it != indexItems.end(); ++it){
        string type = (*it)->type;
        string key = SecondDegreeFriendSearchDataIUtil::getKey(type, userid);
        int sizeT = (*it)->docs.size();
        MCE_INFO("[SecondDegreeFriendSearchManagerI::load] load:" << key << " size:" << sizeT << " Type: " << type);

        SearchDataIPtr p = SecondDegreeFriendSearchDataIUtil::getSearchDataIPtr(type);
        if (!p) {
            MCE_INFO("[SecondDegreeFriendSearchManagerI::load] Invalide Type: " << type);
            return;
        }
        p->add((*it)->docs,config, DYNAMIC_DATA_TIMEOUT);
        if (SECOND_DEGREE_FRIEND == type) {
            dynamic_cache_.add(p,key);
        }
    }
}


void SecondDegreeFriendSearchManagerI::load(string type, int id) {
    string key = SecondDegreeFriendSearchDataIUtil::getKey(type, id);
    MCE_INFO("[SecondDegreeFriendSearchManagerI::load] type:" << type << " id:" << id << " key:" << key);
    SearchDataIPtr sp = getCachedData(key);
    if (sp) {
        if (sp->timeout()) {
            reload(type, id);
        }
        return;
    }
    //MCE_INFO("===========11111111111");
    if (type == SECOND_DEGREE_FRIEND) {    //2011-12-27 加入二度好友
        sp = SecondDegreeFriendSearchDataIUtil::getSearchDataIPtr(type);
    } else {
        MCE_INFO("[SecondDegreeFriendSearchManagerI::load] Invalide Type: " << type);
        return;
    }
    //MCE_INFO("===========2222222222");
    if (sp) {
        sp->load(id);
        dynamic_cache_.add(sp, key);
    }
    //MCE_INFO("===========3333333333333");
}

void SecondDegreeFriendSearchManagerI::reload(string type, int id) {
    MCE_INFO("[SecondDegreeFriendSearchManagerI::reload] Begin LoadThread... " << type);
    SecondDegreeFriendLoadThreadPtr thread = new SecondDegreeFriendLoadThread(type, id);
    thread->start().detach();
}

