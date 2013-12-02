
#ifndef _SERVICE_BASE_STATISTICS_H
#define _SERVICE_BASE_STATISTICS_H

#include "UtilCxx/src/LogWrapper.h"
#include "concurrency/Mutex.h"

#include <time.h>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <set>
#include <iostream>

#include <boost/enable_shared_from_this.hpp>

namespace renren { namespace xoa2 { namespace base { namespace statistics {

using apache::thrift::concurrency::Mutex;
using apache::thrift::concurrency::ReadWriteMutex;

using namespace boost;
using namespace std;

struct ReadWriteInt : ReadWriteMutex {int64_t value;ReadWriteInt(){value = 0;}};
struct ReadWriteFloat : ReadWriteMutex {float value;ReadWriteFloat(){value = 0.0;}};

struct ReadWriteIntCounterMap : ReadWriteMutex, std::map<std::string, boost::shared_ptr<ReadWriteInt> > {}; 

typedef std::vector<ReadWriteInt> ReadWriteIntArray;
typedef std::vector<ReadWriteFloat> ReadWriteFloatArray;

struct ReadWriteIntArrayCounterMap : ReadWriteMutex, std::map<std::string, boost::shared_ptr<ReadWriteIntArray> > {}; 
struct ReadWriteFloatArrayCounterMap : ReadWriteMutex, std::map<std::string, boost::shared_ptr<ReadWriteFloatArray> > {}; 


class ServiceStatistics : public boost::enable_shared_from_this<ServiceStatistics> {
public:
  ServiceStatistics(){}
    
  void setCallMethodInfo(const std::string& name, int64_t time);
  void init();
  std::string getMean(const std::string& name);
  std::string getMax(const std::string& name, int64_t time);
  void reallyStatistics();
  void reallyTestStatistics();
  int64_t getCounter(const std::string& name);
  void getCounters(std::map<std::string, int64_t>& _return);
  void initService();
private:
  //用来记录基类的函数名
  std::set<std::string> name_set_;
  Mutex name_set_lock_;

  //用来记录某个方法被调用了多少次
  ReadWriteIntCounterMap counters_;

  //用来记录某个函数在一天中的第N秒的调用花费最长的时间
  ReadWriteIntArrayCounterMap service_method_time_;

  //用来记录某个函数在一天中的第N秒的时候被调用了M次
  ReadWriteIntArrayCounterMap service_method_call_times_;

  //用来记录某个方法被调用的响应时间
  ReadWriteIntCounterMap service_method_call_time_;

  //1/5/15分钟平均调用时间
  ReadWriteFloatArrayCounterMap mean_call_time_;

  //1/5/15分钟平均调用次数
  ReadWriteFloatArrayCounterMap call_times_;

  ReadWriteIntCounterMap method_call_times_before_; 

  void initStatisticsThread();
  void initTestStatisticsThread();
  bool isMethodInClass(const std::string& name);
  void initMethodNameSet();
  void serviceMethodsInit(const std::string& name);
  std::string getCallTimes(const std::string& name);
  std::string getMeanTime(const std::string& name);

  //用来计算1分钟，5分钟和15分钟内的每次函数调用的平均响应时间
  void calcStatisticsCallTime(std::string name, int64_t time);

  //用来计算1分钟，5分钟和15分钟内的每次函数调用的平均调用次数
  void calcStatisticsCallTimes(std::string name, int64_t times);

  float calcBase(float load, int32_t exp, int32_t n);
  
  std::string getMaxConcurrentTimes(const std::string& name, int64_t time);
  
  std::string getMaxCallTime(const std::string& name, int64_t time);
  //用来清空map对象
  template<typename type1, typename type2>
  void clearMap(std::map<type1,type2>& map) {
    std::map<type1,type2> newMap;
    map.swap(newMap);
  }
  
  int32_t calcSecond() {
    int32_t rv= (int32_t)(time(NULL)+8*3600)%(24*60*60);
    return rv; 
  }

  int32_t getModule(int32_t time) {
    int index;
    if(time <= 0) {
      index = time + 86400;
    } else {
      index = time;
    }
    return index % 86400;
  } 
};

typedef boost::weak_ptr<ServiceStatistics> ServiceStatisticsPtr;

void startStatisticsThread(ServiceStatisticsPtr data);
void startTestStatisticsThread(ServiceStatisticsPtr data);

extern std::string g_Militia;
extern bool g_TestPhase;
}}}}

#endif
