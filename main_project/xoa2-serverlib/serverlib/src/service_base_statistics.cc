#include "service_base_statistics.h"

#include <iostream>
#include <sys/time.h> 

#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>

namespace renren { namespace xoa2 { namespace base { namespace statistics {

using apache::thrift::concurrency::Guard;

using namespace boost; 

#define    SHIFTINDEX    11
#define    FIXED_1       (1 << SHIFTINDEX)
#define    EXP_1         1884
#define    EXP_5         2014
#define    EXP_15        2037

std::string g_Militia = "";
bool g_TestPhase = false;

void ServiceStatistics::setCallMethodInfo(const std::string& name, int64_t time) {
  if(!isMethodInClass(name)){
    return ;
  }

  counters_.acquireWrite();
  ReadWriteIntCounterMap::iterator it = counters_.find(name);
  if(it == counters_.end()) {
    MCE_DEBUG("ServiceStatistics::setCallMethodInfo 添加了对" << name << "方法的统计!");
    serviceMethodsInit(name);
    boost::shared_ptr<ReadWriteInt> read_write_int_ptr;
    read_write_int_ptr = boost::shared_ptr<ReadWriteInt>(new ReadWriteInt);
    counters_[name] = read_write_int_ptr;
  }
  counters_[name]->value += 1;

  if(g_TestPhase) {
    int32_t second_of_day = calcSecond();

    service_method_call_times_.acquireWrite(); 
    service_method_call_times_[name]->at(second_of_day).value = counters_[name]->value;
    service_method_call_times_.release();

    service_method_time_.acquireWrite();
    if(time > service_method_time_[name]->at(second_of_day).value) {
      service_method_time_[name]->at(second_of_day).value = time;
    }
    service_method_time_.release();
  }

  counters_.release();

  service_method_call_time_.acquireWrite();
  boost::shared_ptr<ReadWriteInt> read_write_int_ptr;
  read_write_int_ptr = boost::shared_ptr<ReadWriteInt>(new ReadWriteInt);
  read_write_int_ptr->value = time;
  service_method_call_time_[name] = read_write_int_ptr;
  service_method_call_time_.release();
}

void ServiceStatistics::init() {
  initMethodNameSet();
  initStatisticsThread();
  if(g_TestPhase) {
    cout<<"test model"<<endl;
    initTestStatisticsThread();  
  }
}

std::string ServiceStatistics::getMean(const std::string& name) {
  std::stringstream ss;
  if(name == "") {
    MCE_WARN("ServiceStatistics::getMean() error! 传入的参数不合法");
    ss << "ServiceStatistics::getMean() 传入的参数不合法"; 
    return ss.str(); 
  }
  counters_.acquireRead();
  ReadWriteIntCounterMap::iterator it = counters_.find(name); 
  if(it == counters_.end()) {
    MCE_WARN("ServiceStatistics::getMean() 传入的参数有误");
    ss << "ServiceStatistics::getMean() " << name << " method you have no call, please check this name!";
    counters_.release();
    return ss.str();
  }
  counters_.release();
  ss << getCallTimes(name) << "\r\n" << getMeanTime(name);
  return ss.str();
}

std::string ServiceStatistics::getMax(const std::string& name, int64_t time) {
  std::stringstream ss;
  if(name == "" || time <= 0) {
    MCE_WARN("ServiceStatistics::getMax() error! 传入的参数不合法");
    ss << "ServiceStatistics::getMax() 传入的参数不合法";  
    return ss.str();
  }

  counters_.acquireRead();
  ReadWriteIntCounterMap::iterator it = counters_.find(name);
  if(it == counters_.end()) {
    MCE_WARN("ServiceStatistics::getMax() 传入的参数有误");
    ss << "ServiceStatistics::getMax()" << name << " method you have no call, please check this name!";
    counters_.release();
    return ss.str();
  }
  counters_.release(); 
  if(g_TestPhase) {
    ss << getMaxConcurrentTimes(name, time) << "\r\n" << getMaxCallTime(name, time);
  } else {
    ss << "no test phase! we have no these max data! if you want these data, please start server with -t";
  }
  return ss.str();
}
  

void ServiceStatistics::initStatisticsThread() {
  boost::thread st(boost::bind(&startStatisticsThread, shared_from_this())); 
  st.detach();
}

void ServiceStatistics::initTestStatisticsThread() {
  boost::thread st(boost::bind(&startTestStatisticsThread, shared_from_this())); 
  st.detach();
} 

void ServiceStatistics::reallyStatistics() {
  counters_.acquireRead();
  for(ReadWriteIntCounterMap::iterator it = counters_.begin(); it != counters_.end(); it++) {
    std::string key = it->first;
    service_method_call_time_.acquireRead();
    calcStatisticsCallTime(key, service_method_call_time_[key]->value);
    service_method_call_time_.release();
    
    method_call_times_before_.acquireRead();
    calcStatisticsCallTimes(key, (counters_[key]->value - method_call_times_before_[key]->value));
    method_call_times_before_[key]->value = counters_[key]->value;
    method_call_times_before_.release();
  }
  counters_.release();
}

void ServiceStatistics::reallyTestStatistics() {
  int32_t second_of_day = calcSecond();
  int32_t index = getModule(second_of_day-1);
  service_method_call_times_.acquireWrite();
  for(ReadWriteIntArrayCounterMap::iterator it = service_method_call_times_.begin(); it != service_method_call_times_.end(); it++) {
    std::string key = it->first;
    int64_t long_value = service_method_call_times_[key]->at(index).value;
    service_method_call_times_[key]->at(second_of_day).value = long_value;
  }
  service_method_call_times_.release();
}

bool ServiceStatistics::isMethodInClass(const std::string& name) {
  if(name == "") {
    MCE_WARN("ServiceStatistics::isMethodInClass error!  传入的参数有误!");
    return false; 
  }
  counters_.acquireRead();
  ReadWriteIntCounterMap::iterator it = counters_.find(name);
  if(it != counters_.end()) {
    counters_.release();
    return true;
  }
  counters_.release();

  Guard g(name_set_lock_);
  std::set<std::string>::iterator set_it = name_set_.find(name);
  if(set_it == name_set_.end()) {
    return true;
  }
  return false;
}

int64_t ServiceStatistics::getCounter(const std::string& name){ 
  int64_t rv = 0;
  counters_.acquireRead();
  ReadWriteIntCounterMap::iterator it = counters_.find(name);
  if (it != counters_.end()) {
    it->second->acquireRead();
    rv = it->second->value;
    it->second->release();
  }
  counters_.release();
  return rv; 
}

void ServiceStatistics::getCounters(std::map<std::string, int64_t>& _return) {
  counters_.acquireRead();
  for(ReadWriteIntCounterMap::iterator it = counters_.begin();
      it != counters_.end(); it++)
  {
    _return[it->first] = it->second->value;
  }
  counters_.release();
}

void ServiceStatistics::initService() {
  counters_.acquireWrite();
  clearMap(counters_);
  counters_.release();

  service_method_call_time_.acquireWrite();
  clearMap(service_method_call_time_);
  service_method_call_time_.release();

  mean_call_time_.acquireWrite();
  clearMap(mean_call_time_);
  mean_call_time_.release();

  call_times_.acquireWrite();
  clearMap(call_times_);
  call_times_.release();

  method_call_times_before_.acquireWrite();
  clearMap(method_call_times_before_);
  method_call_times_before_.release();

  if(g_TestPhase) {
    service_method_time_.acquireWrite();
    clearMap(service_method_time_);
    service_method_time_.release();

    service_method_call_times_.acquireWrite();
    clearMap(service_method_call_times_);
    service_method_call_times_.release();
  }
}


void ServiceStatistics::initMethodNameSet() {
  Guard g(name_set_lock_);
  name_set_.insert("getName");
  name_set_.insert("getVersion");
  name_set_.insert("aliveSince");
  name_set_.insert("getStatus");
  name_set_.insert("getStatusDetails");
  name_set_.insert("getCounters");
  name_set_.insert("getCounter");
  name_set_.insert("setOption");
  name_set_.insert("getOption");
  name_set_.insert("getOptions");
  name_set_.insert("getCpuProfile");
  name_set_.insert("getMean");
  name_set_.insert("getMax");
  name_set_.insert("reinitialize");
  name_set_.insert("shutdown");
  name_set_.insert("startMilitiaService");
}

void ServiceStatistics::serviceMethodsInit(const std::string& name) {
  boost::shared_ptr<ReadWriteFloatArray> read_write_float_ptr1;
  read_write_float_ptr1 = boost::shared_ptr<ReadWriteFloatArray>(new ReadWriteFloatArray(3));
  mean_call_time_[name] = read_write_float_ptr1;

  boost::shared_ptr<ReadWriteFloatArray> read_write_float_ptr2;
  read_write_float_ptr2 = boost::shared_ptr<ReadWriteFloatArray>(new ReadWriteFloatArray(3));
  call_times_[name] = read_write_float_ptr2;

  boost::shared_ptr<ReadWriteInt> read_write_int_ptr;
  read_write_int_ptr = boost::shared_ptr<ReadWriteInt>(new ReadWriteInt);
  method_call_times_before_[name] = read_write_int_ptr;
  
  if(g_TestPhase) {
     boost::shared_ptr<ReadWriteIntArray> read_write_int_ptr1;
     read_write_int_ptr1 = boost::shared_ptr<ReadWriteIntArray>(new ReadWriteIntArray(86401));
     service_method_time_[name] = read_write_int_ptr1;

     boost::shared_ptr<ReadWriteIntArray> read_write_int_ptr2;
     read_write_int_ptr2 = boost::shared_ptr<ReadWriteIntArray>(new ReadWriteIntArray(86401));
     service_method_call_times_[name] = read_write_int_ptr2;
  }
}

std::string ServiceStatistics::getCallTimes(const std::string& name) {
  std::stringstream ss;
  call_times_.acquireRead();
  ss << name << " method call times is " << ((int64_t)call_times_[name]->at(0).value*12) << "  " <<
    ((int64_t)call_times_[name]->at(1).value*60) << "  " << ((int64_t)call_times_[name]->at(2).value*180) << " in 1/5/15 min";
  call_times_.release();
  return ss.str();
}

std::string ServiceStatistics::getMeanTime(const std::string& name) {
  std::stringstream ss;
  mean_call_time_.acquireRead();
  ss << name << " method of average call time is " << ((int64_t)mean_call_time_[name]->at(0).value) << "  " <<
    ((int64_t)mean_call_time_[name]->at(1).value) << "  " << ((int64_t)mean_call_time_[name]->at(2).value) <<" in 1/5/15 min";
  mean_call_time_.release();
  return ss.str();
}

void ServiceStatistics::calcStatisticsCallTime(std::string name, int64_t callTime) {
  mean_call_time_.acquireWrite();
  mean_call_time_[name]->at(0).value = calcBase(mean_call_time_[name]->at(0).value, EXP_1, callTime);
  mean_call_time_[name]->at(1).value = calcBase(mean_call_time_[name]->at(1).value, EXP_5, callTime);
  mean_call_time_[name]->at(2).value = calcBase(mean_call_time_[name]->at(2).value, EXP_15, callTime);
  mean_call_time_.release();
}

void ServiceStatistics::calcStatisticsCallTimes(std::string name, int64_t callTimes) {
  call_times_.acquireWrite();
  call_times_[name]->at(0).value = calcBase(call_times_[name]->at(0).value, EXP_1, callTimes);
  call_times_[name]->at(1).value = calcBase(call_times_[name]->at(1).value, EXP_5, callTimes);
  call_times_[name]->at(2).value = calcBase(call_times_[name]->at(2).value, EXP_15, callTimes);
  call_times_.release();
}

std::string ServiceStatistics::getMaxConcurrentTimes(const std::string& name, int64_t time) {
  service_method_call_times_.acquireRead();
  int32_t second_of_day = calcSecond();
  int64_t max_concurrent_times = 0;
  for(int32_t i = second_of_day; i >= second_of_day-time; --i) {
    if(max_concurrent_times < service_method_call_times_[name]->at(getModule(i)).value - service_method_call_times_[name]->at(getModule(i-1)).value) {
      max_concurrent_times = service_method_call_times_[name]->at(getModule(i)).value - service_method_call_times_[name]->at(getModule(i-1)).value;
    }
  }
  service_method_call_times_.release();
  std::stringstream ss;
  ss << name << " max concurrent times is " << max_concurrent_times << " in " << time  <<" second ";
  return ss.str();
}
  
std::string ServiceStatistics::getMaxCallTime(const std::string& name, int64_t time) {
  service_method_time_.acquireRead();
  int32_t second_of_day = calcSecond();
  int64_t max_call_time = 0;
  for(int32_t i = second_of_day; i >= second_of_day-time; --i) {
    if(max_call_time < service_method_time_[name]->at(getModule(i)).value) {
      max_call_time = service_method_time_[name]->at(getModule(i)).value;
    }
  }
  service_method_time_.release();
  std::stringstream ss;
  ss << name << " max call time is " << max_call_time << " in " << time << " ms second ";
  return ss.str();
}

float ServiceStatistics::calcBase(float load, int32_t exp, int32_t n) {
  load *= exp;
  load += n*(FIXED_1-exp);
  load = load /(1 << SHIFTINDEX);
  return load;
}

void startStatisticsThread(ServiceStatisticsPtr data) {
  MCE_DEBUG("启动统计线程完成统计工作! 每隔五秒统计一次");
  boost::shared_ptr<ServiceStatistics> serviceStatistics = data.lock();
  pthread_cond_t cond;
  pthread_mutex_t mutex;
   
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);
   
  struct timespec out_time;
  struct timeval now;
  
  pthread_mutex_lock(&mutex);

  while(true) {
    serviceStatistics->reallyStatistics();
    gettimeofday(&now,NULL);
    out_time.tv_sec = now.tv_sec + 5;
    out_time.tv_nsec = now.tv_usec * 1000;
    pthread_cond_timedwait(&cond, &mutex, &out_time);
  }
  pthread_mutex_unlock(&mutex);
}

void startTestStatisticsThread(ServiceStatisticsPtr data) {
  MCE_DEBUG("启动测试统计线程完成统计工作! 每秒统计一次");
  boost::shared_ptr<ServiceStatistics> serviceStatistics = data.lock();
  pthread_cond_t cond;
  pthread_mutex_t mutex;
   
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);
   
  struct timespec out_time;
  struct timeval now;
  
  pthread_mutex_lock(&mutex);

  while(true) {
    serviceStatistics->reallyTestStatistics();
    gettimeofday(&now,NULL);
    out_time.tv_sec = now.tv_sec + 1;
    out_time.tv_nsec = now.tv_usec * 1000;
    pthread_cond_timedwait(&cond, &mutex, &out_time);
  }
  pthread_mutex_unlock(&mutex);

}

}}}}
