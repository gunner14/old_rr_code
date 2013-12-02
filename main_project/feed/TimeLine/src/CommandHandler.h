#ifndef _TIMELINE_COMMAND_HANDLER_
#define _TIMELINE_COMMAND_HANDLER_

#include <vector>
#include <string>

namespace xce {
namespace feed {
namespace timeline {
 
//enum {kLeNum=0,kInvokeNum,kQNum,kQsbsNum,kQfbsNum,kIwNum,kSwNum,kAddNum,kModifyNum,kSrbiNum,kLoadNum,kExeNum,
//  kUserMiss,kMonthMiss,kMonthHit,k0_2ms,k2_50ms,k50_500ms,k500ms,kDB0_50ms,kDB50_500ms,kDB500ms,kDBExcepn };

enum {kLeNum=0,kInvokeNum,kQNum,kQsbsNum,kQfbsNum,kIwNum,kSwNum,kAddNum,kModifyNum,kSrbiNum,kLoadNum,kExeNum,/**/
  kUserMiss,kMonthMiss,kMonthHit, /**/
k0_2ms,k2_50ms,k50_300ms,k300ms,   /* all */
kq0_2ms,kq2_50ms,kq50_300ms,kq300ms,  /* query */
kqfbs0_2ms,kqfbs2_50ms,kqfbs50_300ms,kqfbs300ms,  /* queryfeedinfobystype */
kadd0_2ms,kadd2_50ms,kadd50_300ms,kadd300ms,  /* addfeedmininotify */
ktmlDB0_50ms,ktmlDB50_300ms,ktmlDB300ms,ktmlDBExcepn, /* timeline DB */ 
kminiDB0_50ms,kminiDB50_300ms,kminiDB300ms,kminiDBExcepn, /* feed_mini DB */ 
kotherDBExcepn
};



const int kStatMax = kotherDBExcepn + 1;

class TimeLineStatistics
{
public:
    TimeLineStatistics();
    
    ::IceUtil::Mutex mutex_;
    std::map<std::string,std::string> table_;
    
    unsigned int time_;//2012071223 = 2012/07/12 23:00:00
    AtomicWord values_[kStatMax];
    string names_[kStatMax];
    
};

class TimeLineFeedI;

/// \brief CommandHandler \see TimeLineFeed_service_command.html
class CommandHandler
{
public:
    CommandHandler(){};
    std::string Help(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string LogLevel(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string LoadCache(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string LoadTimeLine(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string LoadFile(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string QueryFeedInCache(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string QueryMonthInCache(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string QueryFeedInDB(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string QueryMonthInDB(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string ClearUserCache(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string QueryTimeLineInCache(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string QueryTimeLineInDB(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string RebuildTimeLine(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string QueryCacheSize(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string SetCacheSize(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string QueryFeedSize(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string Show(int uid,const std::vector<std::string> & args,const std::string & cmd);  
    std::string Stat(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string StatFile(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string Lru(int uid,const std::vector<std::string> & args,const std::string & cmd);
    //std::string LruTimeout(int uid,const std::vector<std::string> & args,const std::string & cmd);
    //std::string MergeStypes(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string SettingReload(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string SettingShow(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string Setting(int uid,const std::vector<std::string> & args,const std::string & cmd);
    std::string Test(int uid,const std::vector<std::string> & args,const std::string & cmd);
    //std::string Temp(int uid,const std::vector<std::string> & args,const std::string & cmd);

};
 
}
}
}
 

#endif /*_TIMELINE_COMMAND_HANDLER_*/
