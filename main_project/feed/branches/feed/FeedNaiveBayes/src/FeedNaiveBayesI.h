/*
 * FeedQueueI.h
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#ifndef FEEDNAIVEBAYESI_H_
#define FEEDNAIVEBAYESI_H_

#include <queue>
#include "Singleton.h"
#include "TaskManager.h"
#include "FeedNaiveBayes.h"
#include "ScoreCache.h"
#include "ServiceI.h"
#include "QueryRunner.h"

#include <FeedFocusInvertReplicaAdapter.h>
#include <FeedFocusAdapter.h>
//#include "UserPool.h"
#include <boost/foreach.hpp>
//#include "MyQueue.h"
using namespace std;
using namespace MyUtil;
using namespace xce::feed;
namespace xce {
namespace feed {



//------------------------------------

class PrintHelper {
public:
  template<typename T>
  static string Print(const T & container,int limit = 30) {
    ostringstream ios;

    ios << "(";
    int idx = 0;
    typedef typename T::value_type vt;
    BOOST_FOREACH(const vt & v,container ){
    	if(idx++ > 0) ios << ",";
    	ios << v;
      if(idx >= limit) break;
    }
    ios << ")";
    return ios.str();
  }
};


//rong.tang  加载白名单
class WhiteListHandler : public MyUtil::Singleton<WhiteListHandler>{
public:
	void LoadWhitelist() {
		MCE_INFO("@@@@ WhiteListHandler   load  BEGIN");
		set<int> testers;
		Statement sql;
		sql << "select * from feednews_whitelist";
	//	MCE_INFO("@@@@ " << __LINE__);
		mysqlpp::StoreQueryResult res;
		try {
			res = QueryRunner("feed_db", CDbRServer, "feednews_whitelist").store(sql);
		} catch (Ice::Exception& e) {
			MCE_WARN("FeedNews50Handler::LoadWhitelist --> error:" << e);
		} catch (std::exception& e) {
			MCE_WARN("FeedNews50Handler::LoadWhitelist --> error:" << e.what());
		}

		if(!res){
			MCE_WARN("FeedNews50Handler::LoadWhitelist. res null");
		} else {
			for (size_t i = 0; i<res.num_rows(); ++i) {
				mysqlpp::Row row = res.at(i);
				int uid = (int)row["uid"];
//				int view = (int)row["view"];
				testers.insert(uid);
			}
		}
		IceUtil::Mutex::Lock lock(mutex_);
		MCE_INFO("WhiteListHandler::Load. testers size:" << testers.size() << " testers:"
				<< PrintHelper::Print(testers,1000) << " old testers:" << PrintHelper::Print(testers_,1000));
		testers_.swap(testers);
	}
//	Int2IntMap GetTester(const MyUtil::IntSeq & ids);
	bool IsTester(int uid) {
		IceUtil::Mutex::Lock lock(mutex_);
		return testers_.count(uid);
	}

	set<int> testers_;
	IceUtil::Mutex	mutex_;
};
//-------------------------------------------------------------------------------


//rong.tang 12.03.05 将共用的分离出来
class CaculateHelper  {
public:
	CaculateHelper(int id, bool reverse) :
			total_score_(0.0f),
			exist_total_score_(0.0f),
			exist_cnt_(0),
			count_ (0),
			reverse_ (reverse) {
		focusers_ = GetFocusSource(id);
	}

	int CutScore(float score) {
		score = score * 8;
		int temp = (int)score;
		if (temp >= 7) {
			return 7;
		} else {
			return temp;
		}
	}

	int Caculate(int fromid, int toid, int stype) {
		count_++;
		bool focusing = false;
		float from_score = ScoreCache::instance().GetScore(toid, fromid);
		float stype_score = ScoreCache::instance().GetScore(toid, -stype);
		if(focusers_.count(toid)){
			from_score = 1.0f;
			usedfocusers_.insert(toid);
			focusing = true;
		}

		//---------  build friendrank
		int from_stype_score;
		if (WhiteListHandler::instance().IsTester(toid)) { // for the FeedNewsBlock
			int score = 0;
			int tint = CutScore(from_score);
			score |= (tint<<4);

			tint = CutScore(stype_score);
			score |= tint;

			from_stype_score = score;
		} else {  // regular use
			float score = 0.0f;
//			if (IsTest(toid)) {
				score = float(from_score * stype_score * 110);
//			} else {
//				score = float(from_score * stype_score * 250);
//			}
			score = score > 99.0? 99.0:score;
			if(score > 1.0f){
				exist_total_score_ += float(score);
				++exist_cnt_;
				exist_scores_[score] = toid;
			}
			from_stype_score = (int)score;
		}

		int combined;
		if(focusing){
			combined = 0x80 | from_stype_score;
		}else{
			combined = from_stype_score;
		}

		//dict_[fromid] = int(combined);
		if (IsTest(toid) ) {
			MCE_INFO("@@@@ toid:" << toid << " fromid:" << fromid << " stype:" << stype
				<< " from_score:" << from_score << " stype_score:" << stype_score //<< " score:" << score
				<<" from_stype_score:" << from_stype_score << " focusing:" << focusing << " combined:" << combined);
		}

		total_score_ += from_stype_score;
		return combined;
	}

	bool IsTest(int uid) {//是否打log
		  if (//uid/100%100 == 69 ||// 使用新公式
					 uid == 220678672 //jw
					|| uid == 258081713//jin.zhang
					|| uid == 238489851 //fei.yuan
					|| uid == 239556923 //jicheng.zhao
					|| uid == 263734453 //tang.rong
								) {
			return true;
		}
		return false;
	}

	string ShowInfo() {
		ostringstream os;
		ostringstream ostring;
		for(map<float,int>::reverse_iterator ri = exist_scores_.rbegin(); ri != exist_scores_.rend(); ++ri){
				os << "  " << ri->first << ":" << ri->second;
		}
		ostring <<"  CaculateHelper::ShowInfo   reverse?:" << reverse_
				<< "  average:" << float(total_score_) / count_
				<< "  exist cnt:" << exist_cnt_
				<< "  exist average:" << exist_total_score_/exist_cnt_
				<< "  exist scores:" << os.str()
				<< "  focusers:" << PrintHelper::Print(focusers_)
				<< "  usedfocusers:" << PrintHelper::Print(usedfocusers_);

		return ostring.str();
	}

private:
	set<int> GetFocusSource(int target){
		try{
			IntSeq res;
			if (reverse_) {
				res = FeedFocusAdapter::instance().GetFocuses(target);
			} else {
				res = FeedFocusInvertReplicaAdapter::instance().GetSources(target);
			}
//			MCE_INFO("CaculateHelper::GetFocusSource. "<<"reverse?"<<reverse_<<" target:" << target  << " size:  "<<res.size());
			return set<int>(res.begin(), res.end());
		}catch (Ice::Exception& e) {
			MCE_WARN("CaculateHelper::GetFocusSource.   reverse?:" << reverse_ <<" target:" << target  << " error:  "<< e);
		}catch(...){
			MCE_WARN("CaculateHelper::GetFocusSource.   reverse?:" << reverse_ <<" target:" << target  << " unknown error" );
		}
		return set<int>();
	}

private:
	//map<int,int> dict_;
	int count_;
	float total_score_;

	float exist_total_score_;
	int exist_cnt_;
	map<float,int> exist_scores_;

	bool reverse_;
	set<int> focusers_;
	set<int> usedfocusers_;
};



//-----------------------------
class FeedNaiveBayesI: public FeedNaiveBayes, public MyUtil::Singleton<FeedNaiveBayesI> {
public:
//  FeedNaiveBayesI():registered_(false){}
  virtual map<int,int>  GetScoreDict(int fromid,const vector<int> & toids, int stype, const Ice::Current& = Ice::Current());
  virtual map<int,int>  GetScoreDictReverse(int to,const vector<int> & fromids, int stype, const Ice::Current& = Ice::Current());
  virtual int GetSendFrequency(int fromid, const Ice::Current& = Ice::Current());

  virtual bool Registered( const Ice::Current& = Ice::Current()){
  	return true;
//	return registered_;
  }
  virtual void MergeNew(const string & new_score,const string & dump,const Ice::Current& = Ice::Current());
  virtual void ReloadWhitelist(const Ice::Current& = Ice::Current()) {
	  WhiteListHandler::instance().LoadWhitelist();
  }

  virtual void Reload();
  void RegisterController();

  //  void IsTester(int uid){
//  }
private:
//  set<int> GetFocusSource(int target);
//  set<int> testers_;
//  bool registered_;
};

//-----------------------------------

class ReloadTask: public MyUtil::Task, public MyUtil::Singleton<ReloadTask> {
public:
	ReloadTask() :
		Task() {
	}
	virtual void handle() {
//		MCE_INFO("ReloadTask::handle. BEGIN");
//		FrequencyCache::instance().Reload();
//		MCE_INFO("ReloadTask::handle. END");
//		xce::feed::FeedNaiveBayesI::instance().RegisterController();
		MCE_INFO("----------------ReloadTask::handle. BEGIN");
		if (ScoreCache::instance().Reload()) {
			MCE_INFO("----------------ReloadTask::handle. Sucess      Begin Set Register Status");
			xce::feed::FeedNaiveBayesI::instance().RegisterController();
			return ;
		}
		MCE_INFO("----------------ReloadTask::handle. Fail");
	}
private:

};

class TimeStat{
  timeval _begin, _end;
  bool _log;
public:
  TimeStat(bool log=false){
    _log = log;
    reset();
  }
  ~TimeStat(){
    if(_log){
      MCE_INFO("######### " << getTime());
    }
  }
  void reset(){
    gettimeofday(&_begin, NULL);
  }
  float getTime(){
    gettimeofday(&_end, NULL);
    float timeuse=1000000*(_end.tv_sec-_begin.tv_sec)+_end.tv_usec
    -_begin.tv_usec;
    timeuse/=1000;
    return timeuse;
  }


};

}
}
#endif /* FEEDQUEUEI_H_ */
