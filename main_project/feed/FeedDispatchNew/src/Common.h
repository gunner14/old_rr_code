/*
 * Common.h
 *
 *  Created on: Jan 14, 2011
 *      Author: yejingwei
 */

#ifndef COMMON_H_
#define COMMON_H_
//#include "RFeed.h"
#include <vector>
#include <string>
#include "RFeed.h"
#include "XceLoggerAdapter.h"
#include "boost/foreach.hpp"
using namespace com::xiaonei::service;
using namespace std;

/*class FeedConfig;
 class ReplyData;
 class FeedReply;
 typedef IceUtil::Handle<FeedConfig> FeedConfigPtr;
 typedef IceUtil::Handle<FeedReply> FeedReplyPtr;
 typedef IceUtil::Handle<ReplyData> ReplyDataPtr;*/

namespace xce {
namespace feed {
class FeedConfigBuilder {
public:
  static FeedConfigPtr Build(const map<string, string>& props);
};
class FeedConfigLocator {
public:
  static FeedConfigPtr Locate(long feedid);
};
class ReplyDataBuilder {
public:
  static ReplyDataPtr Build(long source, int stype, int actor,
      FeedReplyPtr & reply);
};
class MemcHandler {
public:
  static bool UpdateReply(const vector<long>& fids, const FeedReplyPtr & reply);
};
class ReplyLogger: public Singleton<ReplyLogger>{
public:
  void Init();
  bool AddReply(Ice::Long fid,Ice::Long source, Ice::Int stype,
      Ice::Int actor, const FeedReplyPtr& reply);
private:
  XceLoggerPtr logger_;
  vector<string> path_;
};
class ReplyXmlParser: public Singleton<ReplyXmlParser>{
public:
	int ParseReplier(const string & xml);
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
  float getTime(bool clear = false){
    gettimeofday(&_end, NULL);
    float timeuse=1000000*(_end.tv_sec-_begin.tv_sec)+_end.tv_usec
    -_begin.tv_usec;
    timeuse/=1000;
    if(clear) {
      reset();
    }
    return timeuse;
  }
  
};
struct DispatchCost {
  DispatchCost() :
    send_cfg_(0.0f), expr_(0.0f), recv_cfg_(0.0f), friend_(0.0f), mini_(0.0f),
        log_(0.0f), wtran_(0.0f), news_(0.0f), interaction_(0.0f),
        photo_(0.0f), total_(0.0f) ,range_(0.0f){
  }
  float send_cfg_, expr_, recv_cfg_, friend_, mini_, log_, wtran_, news_,
      interaction_, photo_, total_,range_;
  string ToString();
};

class PrintHelper {
public:
  template<typename T>
  static string Print(const T & container,int limit = 30) {
    ostringstream ios;

    ios << "(";
    int idx = 0;
    typedef typename T::value_type vt;
    using namespace boost;
    BOOST_FOREACH(const vt & v,container){
    	if(idx++ > 0) ios << ",";

    	ios << v;

      if(idx >= limit) break;
    }
    ios << ")";

    return ios.str();
  }
};
struct Record {
  Record() :
    count_(0), target_(0), time_(0.0f) {
  }
  int count_;
  long target_;
  float time_;
};



}
}
#endif /* COMMON_H_ */
