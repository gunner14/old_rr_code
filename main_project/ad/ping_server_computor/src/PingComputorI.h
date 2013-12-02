#ifndef __AD_PING_COMPUTOR_H__
#define __AD_PING_COMPUTOR_H__

#include "PingComputor.h"
#include <string>
#include <set>
#include <map>
#include <set>
#include "TaskManager.h"
#include "Singleton.h"
#include "Date.h"
#include "Util.h"

using namespace std;
using namespace MyUtil;

namespace xce{
namespace ad{

class PingComputorI : public PingComputor, public MyUtil::Singleton<PingComputorI> {
public:
  void Init(const string& DbInstance);
  void Compute();
  void Reset(const string& time, const int interval, const Ice::Current&);
  double StatServerMethod(const string& serverName, const string& methodName,
          const string& beginTime, const string& endTime, const int type, const Ice::Current&);
private:
  void Load(const string& begin_time, const string& end_time, map<string, multiset<double> >& pool);
  double ComputeNN(const multiset<double>& pool); //计算.99
  double ComputeNZ(const multiset<double>& pool); //.90
  double ComputeAvg(const multiset<double>& pool); //平均时间
  double ComputeStdDev(const multiset<double>& pool); //标准差
  
  string db_source_;
  string begin_time_;
  int interval_; //单位minute
};
//每隔一分钟进行一次判断，距上次统计时间>interval_，
//则加载、计算、写入server_ping_stat
class ComputeTimer : public MyUtil::Timer {
public:
  ComputeTimer(int time = ONE_MINUTE) : Timer(time){}
  virtual void handle();
};

}
}
#endif
