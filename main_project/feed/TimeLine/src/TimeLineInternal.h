//
//       Title: TimeLineInternal.h
//       Athor: Feed
//  CreateTime: 07/05/2012
//       Email: zhi.lin@renren-inc.com
// Description: 
//              implement of internal inline function
//              declaration of internal global function
//              declaration of internal class 
//
//
// 
//
#ifndef _TIMELINEFEEDINTERAL_H_
#define _TIMELINEFEEDINTERAL_H_

#include "boost/function.hpp"

namespace xce {
namespace feed { 
namespace timeline {
//gcc's __PRETTY_FUNCTION__ is too long
//gcc's __FUNCTION__ doesn't include the class name
#define FEEDI_TRACE(uid,x) MCE_TRACE("TimeLineFeedI::"<<__FUNCTION__<<" --> uid:"<<uid<<x)
#define FEEDI_DEBUG(uid,x) MCE_DEBUG("TimeLineFeedI::"<<__FUNCTION__<<" --> uid:"<<uid<<x)
#define FEEDI_INFO(uid,x)  MCE_INFO("TimeLineFeedI::"<<__FUNCTION__<<" --> uid:"<<uid<<x)
#define FEEDI_WARN(uid,x)  MCE_WARN("TimeLineFeedI::"<<__FUNCTION__<<" --> uid:"<<uid<<x)
#define FEEDI_FATAL(uid,x) MCE_FATAL("TimeLineFeedI::"<<__FUNCTION__<<" --> uid:"<<uid<<x)

#define SETTING_DEBUG(x) MCE_DEBUG("TimeLineSetting::"<<__FUNCTION__<<" --> "<<x)
#define SETTING_INFO(x)  MCE_INFO("TimeLineSetting::"<<__FUNCTION__<<" --> "<<x)

//stype
const int kStypeCoverEvent=1206;
const int kStypeLifeEvent=1205;
//merge
const int kStypeNewFriend=1201;
const int kStypeMovieToSee=9801;
const int kStypeMovieSeen=9802;

//year range
const int kMinYearAllow=1600;
const int kMaxYearAllow=2400;

//DB
static const string kTimeLineDB="feed_timeline";
static const char* kA09FeedDB="feed_db";
static const char* kB09FeedDB="feed_content_db";
static const int    kDBCluster = 100;
static const int kWideScreenBit=24;// 0-based
static const unsigned int kWideFlag = 1<<kWideScreenBit;

//date property in seed
static const string kYear="year";
static const string kMonth="month";
static const string kDay="day";

//classes

//for feed sorting see TimeLineFeedI::rearrangeFeeds
//class TimeLineFeedInfoGreater
//{
//public:
//    bool operator()(const xce::feed::timeline::TimeLineFeedInfo & n1,const xce::feed::timeline::TimeLineFeedInfo & n2)
//    {
//        return n1.feeds[0] > n2.feeds[0];
//    }
//};
/// \brief Helper class to record time cost \n
/// can automatic log when decontruct \n
/// can get current timecost by elapsed() \n
/// can set a hook function to do custom time record when decontruct \n
/// by function(boost::function< void(TimeHelper*) >)
/// can get current time value(ms) by current() \n
/// can reset timer by reset() \n
/// and other features.
class TimeHelper
{
public:
    const static int LOG_LEVEL_DEBUG = 0;
    const static int LOG_LEVEL_INFO = 1;
    const static int LOG_LEVEL_WARN = 2;
    const static int LOG_LEVEL_ERROR = 3;
    TimeHelper(bool _log = true,int level = LOG_LEVEL_DEBUG ) : needLog_(_log),logLevel_(level)
    {
        startTime_ = current();
    }
    TimeHelper(const string & preLog, bool _log = true,int level = LOG_LEVEL_DEBUG ) : preString_(preLog),needLog_(_log),logLevel_(level)
    {
        startTime_ = current();
    }
    TimeHelper(const char* preLog, bool _log = true,int level = LOG_LEVEL_DEBUG ) :preString_(preLog),needLog_(_log),logLevel_(level)
    {//for char * will convert to bool rather than string, so we need explicit define this.
        startTime_ = current();
    }
    ~TimeHelper()
    { 
        if(func_)
           func_(this);
        if(!needLog_) {
           return;
        }
        stringstream ss;
        ss<<"TimeHelper: "<<preString_<<" Time cost:"<<elapsed()<<" us";
        log(ss.str());
    }
    void log(const string & str)
    {
        switch (logLevel_) {
	  case LOG_LEVEL_DEBUG:
            MCE_DEBUG(str);
	    break;
	  case LOG_LEVEL_INFO:
	    MCE_INFO(str);
	    break;
       	  case LOG_LEVEL_WARN:
            MCE_WARN(str);
            break;
          case LOG_LEVEL_ERROR:
            MCE_ERROR(str);
            break;
          default:
            MCE_DEBUG(str);
        }
    }
    void function(const boost::function< void(TimeHelper*) > & f)
    {
        func_=f;
    }
    void preString(const string & preLog)
    {
        preString_=preLog;
    }
    long reset()
    {
        long result = abs( current() - startTime_ );
        startTime_ = current();
        return result;
    }
    long current(){
	struct timeval tv;
	memset(&tv, 0, sizeof(tv));
	gettimeofday(&tv, NULL);
	return tv.tv_sec*1000*1000 + tv.tv_usec;
    }
    long elapsed()
    {
        return abs( current() - startTime_ );
    }
    void markStone(int id)
    {
        time_map_[id]=current();
    }
    long elapsed(int id)
    {
        return abs(current()-time_map_[id]);
    }
    boost::function<void(TimeHelper*)> func_;
    std::map<int,long> time_map_;
    std::string preString_;
    bool needLog_;
    int logLevel_;
    long startTime_;
};

typedef std::map<int , std::set<int> > YearMonthMap;



//inline function
//Tool functions
//  year >  2009, we use feed_mini
//  year <= 2009, we use feed_mini_bc

/// \brief > 2009 return feed_db \n
///       <= 2009 return feed_content_db
/// \param[in] after_2009 if feed is after 2009
inline const char* GetFeedDBInstance(bool after_2009)
{
    if(after_2009)
        return kA09FeedDB;
    else
        return kB09FeedDB;
}
/// \brief > 2009 return feed_mini_id%100 \n
///       <= 2009 return feed_mini_bc_id%100
/// \param[in] uid id of the user
/// \param[out] name the result table name
/// \param[in] after_2009 if feed is after 2009
inline void GetFeedMiniTable(int uid, string & name, bool after_2009) 
{
    ostringstream os;
    if( after_2009 ) {
        os << "feed_mini_";
    } else {
        os << "feed_mini_bc_";
    }
    os<< uid % kDBCluster;
    name=os.str();
}
/// \brief return feed_timeline_id%100 \n
/// \param[in] id id of the user
/// \param[out] name the result table name
inline void GetTimeLineTable(int id, string & name)
{
    ostringstream os;
    os << "feed_timeline_" << id % kDBCluster;
    name=os.str();
}
/// \brief retrieve stype from type, type&0xFFFF
inline int type2stype(const int & type) {
    return type&0xFFFF;
}
/// \brief return whether wide style bit is set \n
/// wide style bit
inline bool isWide(int type) {
    return type & kWideFlag;
}
/// \brief set or clear wide style bit
/// \param[in] orignal type value
/// \param[in] wide set or clear the bit
/// \return the result type value
inline int makeWide(int orignal ,bool wide) {
    if(wide)
        return orignal | kWideFlag ;
    else
        return orignal & (~kWideFlag);
}
/// \brief calculate next month,e.g 2010-12 -> 2011-01
/// \param[in] year year
/// \param[in] month month
/// \param[out] year_n the result year
/// \param[out]  month_n the result month
inline void getNextMonth(int year,int month,int & year_n,int & month_n) 
{
    if(month!=12) {
        year_n=year;month_n=month+1;
    } else {
        year_n=year+1;month_n=1;
    }
}

//global tool function
time_t mktime_bj(struct tm *t);
time_t getTime(int year,int month,int day,int hour=0,int min=0,int sec=0);
void getYearMonthDay(time_t t,int & year,int & month, int &day);
void getDayTime(time_t t,int &year,int &month,int & day,int &hour,int &min,int &sec);
void getMonthRange(time_t time,time_t & start,time_t & end);
void makeTimeStr(time_t t,std::string & s);
void fix_date(int & year,int & month,int &day);
bool is_future(int year,int month);
bool checkDigit(const string & str,size_t begin, size_t len);
bool inTimeLine(const string & str, int year,int month);
void timeLineToUidYear(string & str, xce::feed::timeline::UidYear& result);
void timeLineToMap(string & str, YearMonthMap & result);
void mapToTimeLine(YearMonthMap & year_month, string & str);
void show(xce::feed::timeline::TimeLineFeedResult res,int uid);
void show(xce::feed::timeline::UidYear & m,int uid);

}
}
}



#endif /* _TIMELINEFEEDINTERAL_H_*/
