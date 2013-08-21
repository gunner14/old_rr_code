#ifndef _CURL_DOWNLOAD_HEAD_
#define _CURL_DOWNLOAD_HEAD_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "bufferarray.h"

namespace net {
  
#ifndef kPhotoDetectLog
#define kPhotoDetectLog false
#endif

using boost::asio::ip::tcp;

class WebHandler {
 public:
  // 返回false,表示没有数据
  virtual bool OnSendData(const char* host, arch::BufferArray *ba) = 0;
  virtual void OnGetHead(std::vector<std::string> heads) = 0;
  virtual void OnGetResult(const char *buf, int size) = 0;
  virtual void OnError() = 0;
  virtual ~WebHandler(){};
};

class WebHandlerFactory {
 public:
  virtual WebHandler * Create() = 0;
  virtual ~WebHandlerFactory(){};
};

struct ValidConnect {
  ValidConnect(boost::asio::io_service &ios)
  : sock(ios)
  , chunk(false)
  , last_time(0) {
  }
  tcp::socket sock;
  bool idle;
  bool chunk;
  arch::BufferArray buffer;
  boost::asio::streambuf recv;
  tcp::endpoint endpoint;
  WebHandler *handler; // callback class
  time_t last_time;
};

class Download {
 public:
  Download(boost::asio::io_service &ios);
  void Start(WebHandlerFactory *wf);
  bool Register(const std::vector<tcp::endpoint> &curl);
  void Close();
  void Clear();
  // 返回true表示马上被处理,否则进入队列
  bool BoostJob();
  void BoostRegister(const std::vector<tcp::endpoint> &curl);
 private:
  void HandleReadResult(const tcp::endpoint &ep, const boost::system::error_code& err, size_t bytes_transferred);
  void HandleWrite(const tcp::endpoint &ep, const boost::system::error_code& err, size_t bytes_transferred);
  void HandleReadHead(const tcp::endpoint &ep, const boost::system::error_code& err, size_t bytes_transferred);
  bool Connect(const tcp::endpoint &ep, tcp::socket *socket);
  void RecheckTimeout();

  void PrecessJob(const tcp::endpoint &ep, ValidConnect *vi);
  bool Idle(tcp::endpoint *ep, ValidConnect** vi);

  void HandleError(bool closed, const std::map<tcp::endpoint, ValidConnect*>::iterator &it);
  bool ConfirmConnect(ValidConnect *vi);

  std::map<tcp::endpoint, ValidConnect*> cvalid_; // 有效及忙标记
  boost::asio::io_service &ios_;
  bool will_exit_;
  WebHandlerFactory *webfactory_;
  time_t max_reconnect_; // 超过n秒就要重连
};

} // namespace 
#endif // define 
