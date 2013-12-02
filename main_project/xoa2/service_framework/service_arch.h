#include <stdio.h>
#include <iostream>
#include <map>

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
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>

// #include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;
using namespace apache::thrift;

namespace xoa2 {

class AbstractProcessorFactory {
 public:
  virtual shared_ptr<TProcessor> Create() = 0;
};

template <class HandlerType, class ProcessorType>
class DefaultProcessorFactory : public AbstractProcessorFactory {
 public:
  static DefaultProcessorFactory & Instance() {
    static DefaultProcessorFactory instance; 
    return instance;
  }

  shared_ptr<TProcessor> Create() {
    shared_ptr<HandlerType> handler(new HandlerType());
    shared_ptr<TProcessor> processor(new ProcessorType(handler));
    return processor;
  }
};

template <class HandlerType, class ProcessorType>
class ThriftService {
 public:
  enum {
    STOPED,
    STARTING,
    READY,
  };
 private:
  server::TThreadPoolServer * server_;
  // server::TSimpleServer * server_;
  int status_;
 public:
  ThriftService() : server_(NULL), status_(STOPED) {
  }

  void Start(int port) {
    if (server_ == NULL) {
      shared_ptr<TProcessor> processor = DefaultProcessorFactory<HandlerType, ProcessorType>::Instance().Create();

      shared_ptr<protocol::TProtocolFactory> protocolFactory(new protocol::TBinaryProtocolFactory());
      // buffered tranport 和 framed transport 的区别何在?
      // shared_ptr<transport::TTransportFactory> transportFactory(new transport::TBufferedTransportFactory());
      shared_ptr<transport::TTransportFactory> transportFactory(new transport::TFramedTransportFactory());
      shared_ptr<transport::TServerTransport> serverTransport(new transport::TServerSocket(port));

      shared_ptr<concurrency::ThreadManager> threadManager = concurrency::ThreadManager::newSimpleThreadManager(10, 20);
      shared_ptr<concurrency::PlatformThreadFactory> threadFactory = shared_ptr<concurrency::PlatformThreadFactory>(new concurrency::PlatformThreadFactory());

#ifndef USE_BOOST_THREAD
      threadFactory->setPriority(concurrency::PosixThreadFactory::HIGHEST);
#endif
      threadManager->threadFactory(threadFactory);
      threadManager->start();

      server_ = new server::TThreadPoolServer(processor, serverTransport, transportFactory, protocolFactory, threadManager);
      /*
      server_ = new server::TSimpleServer(processor, serverTransport, transportFactory, protocolFactory);
      */
    }
    server_->serve();
  }
  bool Stop() {
    delete server_->stop();
    delete server_;
    return true;
  }
};


}

