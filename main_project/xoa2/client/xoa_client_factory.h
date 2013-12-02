#ifndef _XOA2_CLIENT_FACTORY_H_
#define _XOA2_CLIENT_FACTORY_H_

#include "xoa2/client/client_factory.h"

#include <iostream>

#include <boost/lexical_cast.hpp>

#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>

#include "LogWrapper.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

namespace xoa2 {
namespace client {

template <typename T>
class XoaClientWrap : public ClientWrap<T> {
  boost::shared_ptr<TSocket> socket_;
  boost::shared_ptr<TTransport> transport_;
  boost::shared_ptr<TProtocol> protocol_;
 public:
  XoaClientWrap(const char * host, int port, const char * lb_key, const int timeout) 
    : ClientWrap<T>(NULL, lb_key)
    , socket_(new TSocket(host, port))
    , transport_(new TFramedTransport(socket_))
    , protocol_(new TBinaryProtocol(transport_))
  {
    socket_->setConnTimeout(timeout);
    socket_->setSendTimeout(timeout);
    socket_->setRecvTimeout(timeout);

    socket_->setNoDelay(true);
    socket_->setLinger(false, 0);
  }

  bool Init() {
    ClientWrap<T>::client_ = new T(protocol_);

    try {
      transport_->open();
    } catch (TTransportException & e) {
      MCE_WARN("Thrift conn open error addr=" << socket_->getHost() 
          << ":" << socket_->getPort() << " " << e.what());
      return false;
    } catch (...) {
      MCE_WARN("Thrift conn open unknown error. addr=" << socket_->getHost() 
          << ":" << socket_->getPort());
      return false;
    }

    return true;
  }

  virtual ~XoaClientWrap() {
    transport_->close();
  }
};

template <typename ClientType>
class XoaClientFactory : public ClientFactory<ClientType> {
 private:
  int timeout_;
  bool ParseAddr(const std::string & addr, std::string * host, unsigned short * port) const {
    size_t pos = addr.find_first_of(':');
    if (pos == std::string::npos) {
      return false;
    }
    *host = addr.substr(0, pos);

    try {
      *port = boost::lexical_cast<short>(addr.substr(pos + 1));
    } catch (boost::bad_lexical_cast &) {
      return false;
    }

    return true;
  }

 public:
  XoaClientFactory(int timeout) : timeout_(timeout) {
  }

  virtual ClientWrap<ClientType> * Create(const char * addr) {
    std::string host;
    unsigned short port = 0;
    XoaClientWrap<ClientType> * wrap = NULL;

    if (ParseAddr(addr, &host, &port)) {
      try {
        wrap = new XoaClientWrap<ClientType>(host.c_str(), port, addr, timeout_);
      } catch (...) {
        MCE_WARN("Create client wrap new() error. addr=" << host << ":" << port);
        wrap = NULL;
      }
      if (wrap && !wrap->Init()) {
        MCE_WARN("Create client wrap Init() error. addr=" << host << ":" << port);
        delete wrap;
        wrap = NULL;
      }
    }
    return wrap;
  }
};

}
}

#endif // _XOA2_CLIENT_FACTORY_H_

