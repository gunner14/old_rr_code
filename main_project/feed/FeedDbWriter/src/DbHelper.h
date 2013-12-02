/*
 * DBHelper.h
 *
 *  Created on: Dec 24, 2009
 *      Author: antonio
 */

#ifndef DBADDHELPER_H_
#define DBADDHELPER_H_

#include "QueryRunner.h"
#include "Singleton.h"
#include "RFeed.h"
#include "IceLogger.h"
namespace xce {
namespace feed {
const int DB_TASK_LEVEL = 1004;

const int DB_CONTENT_ZONE=1000000000;
const static int DB_ZONE_SPLIT=18;

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
  float getTime(bool rset = false){
    gettimeofday(&_end, NULL);
    float timeuse=1000000*(_end.tv_sec-_begin.tv_sec)+_end.tv_usec
    -_begin.tv_usec;
    timeuse/=1000;
    if (rset) {
      reset();
    }
    return timeuse;
  }
  
};

using namespace com::xiaonei::xce;
using namespace MyUtil;

static string DB_INSTANCE = "feed_db";
static string DB_INSTANCE_INDEX_BY_SOURCE = "feed_index_bysource";
static string DB_ZONE_CONTENT_INSTANCE = "feed_content_db";

const string GROUP_DB_INSTANCE = "feed_minigroup";
const string TAB_ID_SEQ = "feed_id_seq";
const int kWideScreenBit=24;// 0-based
const unsigned int kWideFlag = 1<<kWideScreenBit;
//for timeline
static const string kYear="year";
static const string kMonth="month";
static const string kDay="day";

#define COL_FEED "feed"
#define COL_SOURCE "source"
#define COL_SMALLTYPE "small_type"
#define COL_PSOURCE "psource"
#define COL_PTYPE "ptype"
#define COL_BIG_TYPE "big_type"
#define COL_ACTOR "actor"
#define COL_NEWS_MERGE "news_merge"
#define COL_MINI_MERGE "mini_merge"
#define COL_TIME "time"

const int LOAD_TIMEOUT = 1000; //200ms
const int ID_SEQ_STEP=1000;

class DbHelper: public MyUtil::Singleton<DbHelper> {
public:

  DbHelper():_curIndex(-1), _topIndex(-1) {};

  void addFeedIndex(const FeedIndexPtr& itemPtr);
  void addFeedIndexBysource(const FeedIndexPtr& itemPtr);
  void addFeedContent(const FeedSeedPtr& content);
  void addZoneFeedContent(const FeedSeedPtr& content);
  void addFeedMini(const FeedSeedPtr& content);
  void addFeedSchool(int schoolid,const FeedSeedPtr & seed);
  void addFeedGroup(int groupid, const FeedSeedPtr& );
  void removeFeedIndex(const MyUtil::LongSeq& feeds);
  void removeFeedIndexBysource(Ice::Long source,int stype,int actor = -1);
  void removeFeedContent(const MyUtil::LongSeq& feeds);
  void removeFeedZoneContent(const MyUtil::LongSeq& feeds);
  void removeAllFeedMini(int user);
  void removeFeedMini(int user, int stype, Ice::Long miniMerge);
  void ReplaceXML(const  MyUtil::LongSeq & fids , const string & xml );
  void ReplaceZoneXML(const  MyUtil::LongSeq & fids , const string & xml );
  void UpdateReply(const map<Ice::Long, FeedReplyPtr>& buffer);
  void UpdateZoneReply(const map<Ice::Long, FeedReplyPtr>& buffer);
  bool isNewFeedidForDb(long feedid) {
    return (feedid / DB_CONTENT_ZONE) >= DB_ZONE_SPLIT || feedid < 0;
  }

  //拆分新旧新鲜事feedid列表
  void SplitFeedidsForDb(const MyUtil::LongSeq & feedids, MyUtil::LongSeq & oldids, MyUtil::LongSeq & newids) {
    for(MyUtil::LongSeq::const_iterator it = feedids.begin(); it != feedids.end(); ++it) {
      if (isNewFeedidForDb(*it)) {
        newids.push_back(*it);
      } else {
        oldids.push_back(*it);
      }
    }
  }

  string getIndexTab(Ice::Long feed) {
    ostringstream tab;
    tab << "feed_index_" << feed % 10;
    return tab.str();
  }
  string getIndexBysourceTab(Ice::Long source) {
    ostringstream tab;
    tab <<"feed_index_bysource_"<<source % 100;
    return tab.str();
  }
  string getContentTab(Ice::Long feed) {
    ostringstream tab;
    tab << "feed_content_" << feed % 100;
    return tab.str();
  }
  string getZoneContentTab(Ice::Long feed) {
    ostringstream tab;
    //tab << "feed_content_" << feed % 100 << "_" << feed/DB_CONTENT_ZONE;
    if (feed > 0) {
                    tab << "feed_content_" << feed % 100 << "_" << feed/DB_CONTENT_ZONE;
                } else {
                    tab << "feed_content_bc_" << abs(feed)%100 << "_" << abs(feed)/DB_CONTENT_ZONE;
                }
                MCE_DEBUG("DbHelper::getZoneContentTable --> feed:" << feed << " tab:"<< tab.str());
    return tab.str();
  }
  string getZoneContentTab(int mod, int zone) {
    ostringstream tab;
    //tab << "feed_content_" << mod << "_" << zone;
    if (zone > 0) {
                     tab << "feed_content_" << mod << "_" << zone;
                } else {
                     tab << "feed_content_bc_" << abs(mod) << "_" << abs(zone);
                }
                MCE_DEBUG("DbHelper::getZoneContentTable --> mod:" << mod << " zone:" << zone << " tab:"<< tab.str());
    return tab.str();
  }
  string getMiniTab(int user,bool after_2009) {
    ostringstream tab;
    tab << "feed_mini_";
                if(!after_2009) {
                    tab<<"bc_";
                }
                tab<< user % 100;
    return tab.str();
  }
        string getMiniDB(bool after_2009) {
                if(after_2009)
                    return "feed_db";
                else
                    return "feed_content_db";
        }
  string getSchoolTab(int schoolid) {
    ostringstream tab;
    tab << "feed_school_" << schoolid % 10;
    return tab.str();
  }
  string getGroupTab(int groupid) {
    ostringstream tab;
    tab << "feed_group_" << groupid % 100;
    return tab.str();
  }
private:
  Ice::Long _curIndex;
  Ice::Long _topIndex;
  IceUtil::Mutex _mutex;
};

class IndexLoader;
typedef IceUtil::Handle<IndexLoader> IndexLoaderPtr;

class IndexLoader: public IceUtil::Shared, public IceUtil::Monitor<
    IceUtil::Mutex> {
public:
  IndexLoader() :
    _count(0) {
  }
  ;

  FeedIndexSeq query();

  void addQuery(const string& dbInstance, const string& sql,
      const string& pattern, bool isSelect = true);

  void finish(FeedIndexSeq& indexSeq) {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    _indexSeq.insert(_indexSeq.end(), indexSeq.begin(), indexSeq.end());
    if (--_count == 0) {
      //MCE_INFO("IndexLoader --> finish  res.size:"<<_indexSeq.size());
      notify();
    };
  }
  void finish() {
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(*this);
    if (--_count == 0) {
      notify();
    };
  }
private:
  int _count;
  vector<MyUtil::TaskPtr> _taskSeq;
  FeedIndexSeq _indexSeq;

  class SqlExecuteTask: public MyUtil::Task {
  public:
    SqlExecuteTask(const IndexLoaderPtr& loader, bool isSelect) :
      Task(DB_TASK_LEVEL), _loader(loader), _isSelect(isSelect) {
    }
    ;

    void init(const string& dbInstance, const string& sql,
        const string& pattern) {
      _sql << sql;
      _dbInstance = dbInstance;
      _pattern = pattern;
    }
    ;

    virtual void handle();

  private:
    IndexLoaderPtr _loader;
    bool _isSelect;
    Statement _sql;
    string _dbInstance;
    string _pattern;
  };
};
static time_t mktime_bj(struct tm *t)
{
    return mktime(t);
}


inline time_t getTime(int year,int month,int day) 
{
    struct tm t;
    t.tm_year=year-1900;
    t.tm_mon=month-1;//-1,0-based
    t.tm_mday=day;
    t.tm_hour=0;t.tm_min=0;
    t.tm_sec=0;t.tm_isdst=0;
    return mktime_bj(&t);
}
inline void getYearMonthDay(time_t t,int & year,int & month, int &day)
{
    struct tm * temp=localtime(&t);
    year=temp->tm_year+1900;
    month=temp->tm_mon+1;
    day=temp->tm_mday;
}


//  02/31/2000 -> 02/29/2000
//  04/31/2000 -> 04/30/2000
//  and can't be future;
inline void fix_date(int & year,int & month,int &day)
{
    int max_date=31;
    if(month==4||month==6||month==9||month==11) {
        max_date=30;
    } else if( month==2) {
        if((year%400==0) || (year%100!=0) && (year%4==0))
            max_date=29;
        else
            max_date=28;
    }
    if( day>max_date )
        day=max_date;
    if( getTime(year,month,day) > time(NULL) ) {
        getYearMonthDay(time(NULL),year,month,day);
    }
}
inline int type2stype(const int & type) {
    return type&0xFFFF;
}
inline int makeWide(int orignal ,bool wide) {
    if(wide)
        return orignal | kWideFlag ;
    else
        return orignal & (~kWideFlag);
}




}
;
}
;

#endif /* DBHELPER_H_ */
