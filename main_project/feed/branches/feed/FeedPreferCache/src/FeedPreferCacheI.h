/*
 * FeedQueueI.h
 *
 *  Created on: Feb 17, 2011
 *      Author: yejingwei
 */

#ifndef FEED_PREFER_CACHI_H_
#define FEED_PREFER_CACHI_H_

#include <queue>
#include "Singleton.h"
#include "TaskManager.h"
#include "FeedNaiveBayes.h"
#include "ScoreCache.h"
#include "ServiceI.h"
#include "FeedPreferCache.h"
#include <boost/typeof/typeof.hpp>
//#include "UserPool.h"

//#include "MyQueue.h"
using namespace std;
using namespace MyUtil;
using namespace xce::feed;
namespace xce {
namespace feed {


class FeedPreferCacheI: public FeedPreferCache, public MyUtil::Singleton<FeedPreferCacheI> {
public:
	FeedPreferCacheI(){
		ready_ = false;
		weekcnt_ = 1;
	}
	void Init();
	virtual MyUtil::IntSeq  Get(int uid,int weeks, const Ice::Current& = Ice::Current());
	virtual void Reload(const Ice::Current& = Ice::Current());
	void ReloadPrefer();
private:
	void HandleScoreLine(const string & line);
	string GetScoreFileName(int i);

	bool GetFileStream(ifstream & infile);
	typedef hash_map<int,vector<int> > PreferMap;
	PreferMap prefer_map_;
	bool ready_;
	int weekcnt_;
	string data_path_;
	string filename_;
};


class ReloadPreferTask: public MyUtil::Task {
public:
	ReloadPreferTask() : Task() { }
	virtual void handle() {
		FeedPreferCacheI::instance().ReloadPrefer();
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
