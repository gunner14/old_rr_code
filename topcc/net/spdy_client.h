#ifndef _NET_SPDY_CLIENT_H
#define _NET_SPDY_CLIENT_H

#include "net/spdy_sess.h"
//#include <stdio.h>
#include <map>
#include <set>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "net/spdy_method.h"
#include "net/spdy_registry.h"
#include "net/spdy_response.h"

/*
  Xoa的C++客户端 
  v0.8版 基本功能实现 2011-3-8
  v0.9版 兼容java,暂时去掉压缩头,双线程调用,简单取模处理, 2011-3-25
  v1.0版 增强错误处理,增加json的解析, 2011-4-1

*/

namespace xoa {
using boost::asio::ip::tcp;

//typedef void (*XoaResponseCallback)(const XoaResponse &);
class XoaResponseCallback {
 public:
  virtual void Callback(const XoaResponse &) = 0; 
};

class XoaClient : public SpdyStreamSink {
 public:
  XoaClient(XoaRegistry* reg);
  ~XoaClient();
  struct RequestInfo {
    Method method;
    XoaResponseCallback *callback;
    int timeout;
    //boost::mutex mutex;
  };

  bool Execute(const Method &method, XoaResponse *resp, int timeout);
  bool Submit(const Method &method, XoaResponseCallback *cb, int timeout); //boost::function<void (const char*, int)> callback;

 private:
  void Run();
  void OnStream(spdy::SpdyStreamId id, const spdy::SpdyHeaderBlock *block, const char* buf, int sz);
  void OnError(bool send, spdy::SpdyStreamId id, int error, int mod);

  // 以下2客户端不会被调用
  SpdyStreamSink* OnNew(boost::asio::io_service & ios) {
    return NULL;//new XoaClient(NULL);
  }
  SpdyConnector* GetContext() {
    return NULL;
  }

  void OnSubmit(const RequestInfo &ri);
  void KeepTime(const boost::system::error_code &e);

  SpdyConnector* CheckConnect(int mod = 0);
  bool TryConnect(tcp::endpoint *endip, tcp::socket *sock, int mod);
  bool Register(const std::string &host, const std::vector<std::pair<std::string, int> > &ips);
  bool Obtain(const Address &addr);
  void ResendRequest(SpdyConnector& conn);
  void WillClose();

 private:
  boost::asio::io_service io_service_; // 别人依赖它,他要第一个
 // SpdyConnector base_;
  XoaRegistry* ipreg_;
  std::map<spdy::SpdyStreamId, RequestInfo> req_map_;
  // 发送的缓冲区
  BufferList send_stream_list_;
  int send_timeout_;
  boost::shared_ptr<boost::thread> work_;
  boost::asio::deadline_timer t_;
  std::vector<std::pair<tcp::endpoint, boost::shared_ptr<SpdyConnector> > > connects_;

 private: // 连接没必要异步
  void HandleResolve(const boost::system::error_code& err,
                      tcp::resolver::iterator endpoint_iterator);
  void HandleConnect(const boost::system::error_code& err,
                      tcp::resolver::iterator endpoint_iterator);
};

} //namespace
#endif //define
