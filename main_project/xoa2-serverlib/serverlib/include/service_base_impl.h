/**
 * 以后所有的服务类都要继承于这个基类，这个基类保持与java版本的基类相同的功能，
 * 保证了能够进行统计一些数据和控制服务的运行状况（停止服务，重新初始化服务）。
 * 同时提供了一个切换民兵服务的接口，来实现实时的服务切换功能。
 * 
 * @author zhe.long
 * @email zhe.long@renren-inc.com
 * @date 2012.11.23
 *
 */

#ifndef _SERVICE_BASE_IMPL_H_
#define _SERVICE_BASE_IMPL_H_

#include "RenrenService.h"
#include "service_base_statistics.h"
#include "service_registry.h"

#include "server/TServer.h"
#include "concurrency/Mutex.h"

#include <time.h>
#include <vector>
#include <string>
#include <map>
#include <sstream>

namespace xoa2 { namespace base {
using apache::thrift::concurrency::Mutex;
using apache::thrift::concurrency::ReadWriteMutex;
using apache::thrift::server::TServer;

using namespace apache::thrift;
using namespace renren::xoa2::base::statistics;

class ServiceBaseImpl : virtual public RenrenServiceIf {
 protected:
   //构造函数分别是服务名和对应的版本号，用来完成在zk上进行自注册
   ServiceBaseImpl(std::string name, std::string version);
   virtual ~ServiceBaseImpl() {}

 public:
   //thrift文件中定义的方法
   // 获得服务的id         
   virtual void getName(std::string& _return){
     _return = name_;
   }   
   //获得服务的version
   virtual void getVersion(std::string& _return){
     _return = version_;
   }
   //用于返回服务启动的秒数
   virtual int64_t aliveSince();

   //获得服务的status 
   virtual RenrenBaseStatus::type getStatus() { 
     return service_status_;
   }
  
   //获得服务的status的描述
   virtual void getStatusDetails(std::string& _return);

   //返回服务key方法的调用次数
   virtual void getCounters(std::map<std::string, int64_t>& _return);
   virtual int64_t getCounter(const std::string& key);
 
   virtual void getCpuProfile(std::string& _return, const int32_t secs);
   //返回服务method方法最近1/5/15分钟的平均调用次数
   virtual void getMean(std::string& _return, const std::string& key);

   //返回服务method方法最近1/5/15分钟的最大调用次数
   virtual void getMax(std::string& _return, const std::string& key, const int32_t secs);

   virtual void reinitialize();

   virtual void shutdown();

   virtual void startMilitiaService(const std::string& serviceId);
   //自己定义的方法
   void setServer(shared_ptr<TServer> server) {
     server_ = server;
   }

   void setRegistry(shared_ptr<renren::xoa2::registry::ServiceRegistry> registry) {
     registry_ = registry;
   }

   //每次调用函数的时候记性统计时的调用
   void setCallMethodInfo(const std::string& name, const int64_t time);

private:
  std::string name_;
  std::string version_;
  RenrenBaseStatus::type service_status_;
  int64_t alive_since_;
  Mutex service_status_lock_;
  shared_ptr<TServer> server_;
  boost::shared_ptr<ServiceStatistics> service_statistics_; 
  shared_ptr<renren::xoa2::registry::ServiceRegistry> registry_;
 
  void setStatus(RenrenBaseStatus::type status);

};

}}

#endif
