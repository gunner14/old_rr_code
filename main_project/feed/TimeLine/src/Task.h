#ifndef _TIMELINEFEED_TASK_H_
#define _TIMELINEFEED_TASK_H_

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "TimeLineFeed.h"
#include <string>
#include <vector>

namespace xce {
namespace feed {
namespace timeline {

class TimeLineStaticsTask: public IceUtil::TimerTask 
{
public:
    virtual void runTimerTask();
    virtual ~TimeLineStaticsTask(){};
};

class LoadCacheByFileTask: public MyUtil::Task 
{
public:
    LoadCacheByFileTask(const std::string & s,const std::string & key,int _quotient=0,int _remainder=0):file_name(s),progress_name(key),
    quotient(_quotient),remainder(_remainder){};
    virtual void handle();
    virtual ~LoadCacheByFileTask(){};
private:
    std::string file_name;
    std::string progress_name;
    int quotient;
    int remainder;
};

class LoadCacheTask: public MyUtil::Task   
{  
public:   
    LoadCacheTask(const std::vector<int> & ids):uids(ids){};   
    virtual void handle();  
    virtual ~LoadCacheTask(){};
private:   
    std::vector<int> uids;
};


}
}
}



#endif /*_TIMELINEFEED_TASK_H_*/
