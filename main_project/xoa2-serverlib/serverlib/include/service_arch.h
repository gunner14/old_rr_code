#ifndef _SERVICE_ARCH_H_
#define _SERVICE_ARCH_H_ 

#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <iostream>
#include <map>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>

#include "UtilCxx/src/LogWrapper.h"

#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <thrift/Thrift.h>

#include <thrift/transport/TServerTransport.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <thrift/protocol/TProtocol.h>
#include <thrift/protocol/TBinaryProtocol.h>

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TNonblockingServer.h>

#include "xoa_processor.h"
#include "service_registry.h"
#include "service_config.h"

using namespace std;
using namespace boost;
using namespace apache::thrift;
using namespace xoa2::processor;
using namespace renren::xoa2::registry;
using namespace xoa2::base;

using apache::thrift::server::TNonblockingServer;
namespace xoa2 { namespace service {

class AbstractProcessorFactory {
public:
  virtual shared_ptr<TProcessor> Create(std::string name, std::string version) = 0;
};

template <class HandlerType, class ProcessorType>
class DefaultProcessorFactory : public AbstractProcessorFactory {
public:
  static DefaultProcessorFactory & Instance() {
    static DefaultProcessorFactory instance;
    return instance;
  }

  shared_ptr<TProcessor> Create(std::string name, std::string version) {
    if(name == "" || version == "" ) {
      MCE_WARN("ProcessorFactory::Create error: "
               << "必须输入服务名和服务版本号");
      shared_ptr<TProcessor> processor;
      return processor; 
    }
    shared_ptr<HandlerType> handler(new HandlerType(name, version));
    handler_ = handler;
    shared_ptr<TProcessor> processor(new ProcessorType(handler));
    return processor;
  }

  shared_ptr<HandlerType> getHandler() {
    return handler_;
  }

private:
  shared_ptr<HandlerType> handler_;
 
};

template <class HandlerType, class ProcessorType>
class ThriftService {
 private:
  // TODO: use nb-server
  shared_ptr<server::TServer> server_;
  std::string name_;
  std::string version_;
  int port_;
  bool has_base_;
 public:
  ThriftService():has_base_(false){}

  void Start(const std::string& config, const std::string& xoa_path, bool is_registry) {
    xoa2::Xoa2ServiceConfig conf;
    if(conf.Init(xoa_path) != 0) {
      MCE_ERROR("init xoa.xml file fail! please check xoa.xml file!");
      return ;
    }

    if(!GetConfInfo(conf)) {
      MCE_ERROR("get xoa.xml info fail! please check xoa.xml file!");
      return ;
    }
    
    if (server_ == NULL) {
      boost::shared_ptr<TProcessor> real_processor = DefaultProcessorFactory<HandlerType, ProcessorType>::Instance().Create(name_, version_);
      if(real_processor == NULL) {
        MCE_ERROR("create real processor error, start server fail!");
        return ;
      }
      if(dynamic_pointer_cast<ServiceBaseImpl>(DefaultProcessorFactory<HandlerType, ProcessorType>::Instance().getHandler()) != NULL) {
        has_base_ = true;
      }
      //boost::shared_ptr<protocol::TProtocolFactory> protocol_factory(new protocol::TBinaryProtocolFactory());
      // buffered tranport 和 framed transport 的区别何在?
      //shared_ptr<transport::TTransportFactory> transport_factory(new transport::TBufferedTransportFactory());
      
      boost::shared_ptr<transport::TTransportFactory> transport_factory(new transport::TFramedTransportFactory());
      boost::shared_ptr<TProtocolFactory> protocol_factory(new TBinaryProtocolFactory(0, 0, false, false));

      boost::shared_ptr<XoaProcessor<HandlerType> > xoa_processor(new XoaProcessor<HandlerType>(real_processor));
      xoa_processor->setHandler(DefaultProcessorFactory<HandlerType, ProcessorType>::Instance().getHandler()); 
      
      if(has_base_) {
        xoa_processor->setHasBase(true);
      }
      boost::shared_ptr<transport::TServerTransport> server_transport(new transport::TServerSocket(port_));
      int thread_pool_core_size = conf.thread_pool_core_size();
      if(thread_pool_core_size <= 0) {
        MCE_WARN("thread pool core size error! please check it! thread_pool_core_size will be set 4");
        thread_pool_core_size = 4;
      }
      boost::shared_ptr<concurrency::ThreadManager> thread_manager = concurrency::ThreadManager::newSimpleThreadManager(thread_pool_core_size);
      boost::shared_ptr<concurrency::PlatformThreadFactory> thread_factory = shared_ptr<concurrency::PlatformThreadFactory>(new concurrency::PlatformThreadFactory());

#ifndef USE_BOOST_THREAD
      thread_factory->setPriority(concurrency::PosixThreadFactory::HIGHEST);
#endif
      thread_manager->threadFactory(thread_factory);
      thread_manager->start();
      server_ = shared_ptr<server::TServer>(new TNonblockingServer(xoa_processor, protocol_factory, port_, thread_manager));
      //server_ = new server::TThreadPoolServer(xoa_processor, server_transport, transport_factory, protocol_factory, thread_manager);      
    }
    if(has_base_) {
      dynamic_pointer_cast<ServiceBaseImpl>(DefaultProcessorFactory<HandlerType, ProcessorType>::Instance().getHandler())->setServer(server_);
    }
    if(is_registry) {   
      startRegistry(config);
    } else {
      MCE_WARN("Skip regist service to xcs, binding on port : " <<  port_);
    }
    server_->serve();
  }

private:
  void startRegistry(const std::string& config) {
    if(config == "") {
      cout << "传入的参数为空，所以不在XCS上进行注册!" << endl;
      MCE_WARN("传入的参数为空，所以不在XCS上进行注册!");
      return ;
    }
    xcs::ServiceNode node;
    std::string ip_and_port = GetIpAndPort();
    node.name_ = ip_and_port;
    node.content_ = "disabled=false\nhealthy=true\nstate=0";
    shared_ptr<renren::xoa2::registry::ServiceRegistry> registry = renren::xoa2::registry::DefaultRegistryFactory::Instance().Create(config, node);
    if(registry == NULL) {
      cout << "registry create error!" <<endl;
      MCE_ERROR("registry create error!");
      return ;
    }
    cout<<"正在XCS上注册服务..."<<endl;
    sleep(5);
    int rc = registry->registerNode(name_, version_, "0");
    if(rc) {
      cout << "启动注册失败 rc=" << rc <<endl;
      MCE_ERROR("启动注册失败 rc=" << rc );
      return ; 
    }
    if(has_base_) {
      dynamic_pointer_cast<ServiceBaseImpl>(DefaultProcessorFactory<HandlerType, ProcessorType>::Instance().getHandler())->setRegistry(registry);
    }
  }

  std::string GetIpAndPort()
  {
    char host_name[100];
    char ip[32];
    struct hostent* host = NULL;
  
    if(gethostname(host_name, 100)) { 
      MCE_WARN("GetIpAndPort gethostname() fail!");
      return ""; 
    }

    host = gethostbyname(host_name);
    if(host == NULL) {
      MCE_WARN("GetIpAndPort gethostbyname() fail!");
      return "";
    }

    std::string ip_str = std::string(inet_ntoa(*((struct in_addr *)host->h_addr_list[0])));
    std::stringstream ss;
    ss << ":" << port_;
    std::string port_str = ss.str();
    std::string ip_and_port = ip_str + port_str;
    return ip_and_port;
  }

  bool GetConfInfo(xoa2::Xoa2ServiceConfig& conf) {
    name_ = conf.service_id();
    if(name_ == "") { 
      MCE_ERROR("server name error! please check it. start server fail!");
      return false;
    }
   
    version_ = conf.version();
    if(version_ == "") {
      MCE_WARN("version error! please check it! version_ will be set 1");
      version_ = "1";
    }

    port_ = conf.listen_port(); 
    if(port_ < 0) {
      MCE_ERROR("port error! please check it. start server fail!");
      return false;
    }

    return true;
  }
};

}}

#endif
