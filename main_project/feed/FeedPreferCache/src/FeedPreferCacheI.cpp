/*
 * FeedQueueI.cpp
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#include "LogWrapper.h"
#include "QueryRunner.h"
#include "ServiceI.h"
#include "FeedPreferCacheI.h"
#include "ScoreCache.h"
#include "TaskManager.h"
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include "Date.h"
using namespace xce::feed;
using namespace std;
using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace boost;

//const static string data_path_ = "/data/xce/PreferScore/data/week1/";


class PrintHelper {
public:
  template<typename T>
  static string Print(const T & container,int limit = 30) {
    ostringstream ios;

    ios << "(";
    int idx = 0;
    typedef typename T::value_type vt;
    BOOST_FOREACH(const vt & v,container){
    	if(idx++ > 0) ios << ",";
    	ios << v;
      if(idx >= limit) break;
    }
    ios << ")";
    return ios.str();
  }
};
void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FeedPreferCacheI::instance(),
      service.createIdentity("M", ""));
  FeedPreferCacheI::instance().Init();
	TaskManager::instance().execute(new ReloadPreferTask);
}

int GetWeekCnt(int idx){
	switch (idx){
	case 0: return 1;
	case 1:	return 2;
	case 2: return 4;
	case 3: return 6;
	case 4: return 8;
	default: return 2;
	}
}
void FeedPreferCacheI::Init(){
  ServiceI& service = ServiceI::instance();
  Ice::PropertiesPtr props = service.getCommunicator()->getProperties();
  int index = props->getPropertyAsIntWithDefault("Service.ServerIndex", 0);
  weekcnt_ = GetWeekCnt(index);
  ostringstream os;
  os << "/data/xce/PreferScore/data/week" << weekcnt_ << "/";
  data_path_ = os.str();
  MCE_INFO("FeedPreferCacheI::Init. index:" << index << " weekcnt:" << weekcnt_ << " data_path:" << data_path_);
}

MyUtil::IntSeq  FeedPreferCacheI::Get(int uid,int weeks, const Ice::Current&){
	if(ready_ == false){
		MCE_INFO("FeedPreferCacheI::Get. uid:" << uid << " weeks:" << weeks << " not ready.");
		return vector<int>();
	}
	vector<int> res;
	PreferMap::iterator it = prefer_map_.find(uid);
	if(it  != prefer_map_.end()){
		res = it->second;
	}
	MCE_INFO("FeedPreferCacheI::Get. uid:" << uid << " weeks:" << weeks << " res:" << PrintHelper::Print(res));
	return res;
}

void FeedPreferCacheI::Reload(const Ice::Current& ){
	if(ready_ == false){
		MCE_WARN("FeedPreferCacheI::Reload. is already reloading");
		return;
	}
	ready_ = false;
	TaskManager::instance().execute(new ReloadPreferTask());
}

string AddZero(int i) {
  string s;
  if (i < 10) {
    s = "0";
  }
  s += lexical_cast<string> (i);
  return s;
}

string FeedPreferCacheI::GetScoreFileName(int i) {
	MyUtil::Date DeltaDays(i* 24 * 60 * 60);
	MyUtil::Date today = Date(time(NULL));
	string name;
	MyUtil::Date d = today - DeltaDays;
	ostringstream oss;
	oss << "clickreply.alps.week" << weekcnt_<< "." << d.year() << "-" << AddZero(d.month()) << "-"	<< AddZero(d.day());
	return oss.str();
}


bool FeedPreferCacheI::GetFileStream(ifstream & infile){
	bool suc = false;
	for (unsigned i = 0; i < 100; ++i) {
		string file = data_path_ + GetScoreFileName(i);
		if(filename_ == file){
			MCE_WARN("FeedPreferCacheI::GetFileStream. same file. filename:" << filename_);
			return suc;
		}
		infile.open(file.c_str(), ifstream::in);
		if (!infile) {
			MCE_INFO("FeedPreferCacheI::GetFileStream. openfile fail:" << file );
		} else {
			MCE_INFO("FeedPreferCacheI::GetFileStream. openfile OK:" << file );
			filename_ = file;
			suc = true;
			break;
		}
	}
	return suc;
}

void FeedPreferCacheI::ReloadPrefer(){
	//TODO 是否需要sleep
	TimeStat clear_ts,ts;
	ifstream infile;
	bool suc = GetFileStream(infile);
	if(!suc){
		MCE_WARN("FeedPreferCacheI::ReloadPrefer. openfile all fail");
	} else {
		prefer_map_.clear();
		float clear_cost = clear_ts.getTime();
		string line;
		while(getline(infile,line)){
			try{
				HandleScoreLine(line);
			}catch(std::exception &e){
				MCE_WARN("FeedPreferCacheI::ReloadPrefer. unknown error. line:" << __LINE__ << " exception:" << e.what());
			}
		}
		MCE_INFO("FeedPreferCacheI::ReloadPrefer. done. map size:" << prefer_map_.size()
				<< " clear_cost:" << clear_cost
				<< " cost:" << ts.getTime());
	}
	ready_ = true;
}
void FeedPreferCacheI::HandleScoreLine(const string & line){
//	MCE_INFO(__LINE__);
	static int lineno = 0;
	tokenizer<> tok(line);
	vector<int> friends;
	int uid;
	int idx =0;
	for(BOOST_AUTO(pos,tok.begin()); pos != tok.end(); ++pos){
		int value = lexical_cast<int>(*pos);
		if(idx == 0)
				uid = value;
		else
			friends.push_back(value);
		++idx;
	}
//	MCE_INFO("###.line no:"<< lineno << " uid:" << uid << " friends:" << PrintHelper::Print(friends) );
	++lineno;
	prefer_map_[uid] = friends;
//	MCE_INFO(__LINE__);

}


