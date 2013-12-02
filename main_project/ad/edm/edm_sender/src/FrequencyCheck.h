/*
 * FrequencyCheck.h
 *
 * module role : 用户发送edm信息记录及频率检测
 * Created on: 2012-2-14
 * Author: shuangshuang
 */

#ifndef __EDM_FREQUENCY_CHECK_H__
#define __EDM_FREQUENCY_CHECK_H__

#include "Singleton.h"
#include <ctime>
#include <map>
#include "TaskManager.h"
#include <IceUtil/RWRecMutex.h>
#include <Ice/Ice.h>

namespace xce {
namespace ad {

using namespace  MyUtil;
using namespace  std;

typedef map<int,time_t> UidTimeMap;
enum SendType { kWeekly = 1, kDaily};

class FrequencyCache : public MyUtil::Singleton<FrequencyCache> {
  public:
    FrequencyCache();
    bool LoadFromDb();
    void UpdateCache();
    int CheckFrequencyLimit(int user_id);
    bool Add(int user_id, SendType send_type = kWeekly, time_t update_time = time(NULL));
    inline bool daily_load_sign() { return daily_load_sign_;}
    inline bool weekly_load_sign() { return weekly_load_sign_;}
    inline bool load_sign() { return weekly_load_sign_ && daily_load_sign_;}
    inline void set_daily_load_sign(bool daily_load_sign = false) { daily_load_sign_ = daily_load_sign;}
    inline void set_weekly_load_sign(bool weekly_load_sign = false) { weekly_load_sign_ = weekly_load_sign;}
  private:
    bool LoadDailyData();
    bool LoadWeeklyData();
    bool WeeklySendValidate(int user_id);
    bool DailySendValidate(int user_id);
    //void GetNDaysAgoTime(int n_day,string &need_time);  // 获取n天前的时间 样式 ： 2012-02-14 05:21:21
  private:
    bool weekly_load_sign_;
    bool daily_load_sign_;
    UidTimeMap user_daily_send_time_;
    UidTimeMap user_weekly_send_time_;
    IceUtil::RWRecMutex mutex_;
  private:
    class CacheCleaner : public Timer {
      public:
        CacheCleaner(int seconds = 60) : Timer(seconds*1000) {}
        virtual void handle();
    };

};

class FrequencyCacheLoadTask : public Task {
public:
  FrequencyCacheLoadTask():Task(TASK_LEVEL_PRELOAD) {}
  virtual void handle();
};

}
}
#endif
