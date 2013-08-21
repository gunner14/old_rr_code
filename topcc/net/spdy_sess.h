#ifndef NET_SPDY_SESSION_H__
#define NET_SPDY_SESSION_H__

//#include <boost/signals.hpp>
#include <list>
#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>

#include "net/asio_forward.hpp"
#include "net/spdy/spdy_framer.h"
#include "net/bufferarray.h"

namespace xoa {

const int kExistStreamId = 8881;
const int kNoExistStreamId = 8882;
const int kParseHeaderError = 8885;
const int kSendStreamTimeout = 8888;
const int kReceiveReplyTimeout = 8889;
const int kNoValidHost = 8892; // 当指定的服务端ip端口都访问不了时

class HttpRequestHeaders;
class HttpResponseInfo;
struct SpdyConnector;

using boost::asio::ip::tcp;

// Host Name(ip) and port
typedef std::pair<std::string, std::string> Address;

// 接收
struct SpdyStreamSink {
  // recv stream by request or response
  virtual void OnStream(spdy::SpdyStreamId, const spdy::SpdyHeaderBlock *, const char*, int) = 0;
  
  // send or recv data error
  virtual void OnError(bool send, spdy::SpdyStreamId id, int error, int mod) = 0; 

  // new child class instance,remenber delete it after used
  virtual SpdyStreamSink* OnNew(boost::asio::io_service &) = 0;

  // is match

  // init SpdyConnector
  virtual SpdyConnector* GetContext() = 0;
};

typedef std::pair<char*,int> BufferPair;
typedef std::list<BufferPair> BufferList;
struct StreamInfo {
  StreamInfo(spdy::SpdyHeaderBlock b) : block(b), buffer(new arch::BufferArray) {
  }
  spdy::SpdyHeaderBlock block;
  arch::BufferArray *buffer;
};

struct SpdyConnector : public spdy::SpdyFramerVisitorInterface {
  SpdyConnector(boost::asio::io_service & ios, SpdyStreamSink *sink, int first_id = 1)
    : stream_id_(first_id)
    //, ios_(ios)
    , socket_(ios)
    , spdy_(new spdy::SpdyFramer)
    , sink_(sink)
    , last_time_(0)
    , send_timeout_(10000)
    , spdy_error_(0) {
    spdy_->set_visitor(this);
  }

  bool Boost(int mod);

  void WillClose();

  spdy::SpdyStreamId PutFrame(spdy::SpdyStreamId id,
                              const std::string& host,
                              const std::string& method, // get, post, put, delete
                              const spdy::SpdyHeaderBlock *block,
                              const std::string& url,
                              const char* data,
                              int length,
                              bool async);

  bool DoWrite(spdy::SpdyStreamId id, const spdy::SpdyFrame *buf, bool async);
  bool DoWrite(spdy::SpdyStreamId id, const arch::BufferArray *buf, bool async);
  void HandleWrite(spdy::SpdyStreamId id, const boost::system::error_code& err, size_t bytes_transferred);
  void DoRead();
  void HandleRead(const boost::system::error_code& err, size_t bytes_transferred);
  bool SyncReadFrame(spdy::SpdyStreamId id, char** buf, int *len);

  void OnError(spdy::SpdyFramer* framer);
  void OnControl(const spdy::SpdyControlFrame* frame);
  void OnStreamFrameData(spdy::SpdyStreamId stream_id, const char* data, size_t len);
  
  void HandleError(bool send, spdy::SpdyStreamId id, int error);
  bool server_host() {
    return stream_id_ % 2 == 0;
  }

  int GetNewStreamId() {
    stream_id_ += 2;
    return stream_id_;
  }

  tcp::socket& socket() {
    return socket_;
  }

  int mod() {
    return mod_;
  }

 private:
  void Clear();
  int stream_id_; // The next stream id, initialize as 1
  
  tcp::socket socket_;
  boost::scoped_ptr<spdy::SpdyFramer> spdy_;

  // 接收的缓冲区
  std::map<spdy::SpdyStreamId, StreamInfo> recv_stream_map_;

  // 发送的缓冲区
  std::map<spdy::SpdyStreamId, time_t> send_stream_map_;

  //boost::signal <void (bool, spdy::SpdyStreamId, int)> sent_error_;
  SpdyStreamSink *sink_;
  char recv_buf_[512];
  int send_timeout_;  //发送后反馈的时间,涉及到切换备份机上
  time_t last_time_; // 连接的最后连接时间
  int spdy_error_;
  int mod_;
  DISALLOW_COPY_AND_ASSIGN(SpdyConnector);
};

class SpdyServer {
public:
  SpdyServer();
  bool Listen(const Address & address);
  void Run();
  void Set(SpdyStreamSink *sink); // add sink?
  boost::asio::io_service& io_service() {
    return ios_;
  }
  
private:
  void HandleAccept(const boost::system::error_code& error, SpdyStreamSink* new_connection);

  boost::scoped_ptr<boost::asio::io_service> io_service_;
  boost::scoped_ptr<tcp::acceptor> acceptor_;
  std::string allow_url_;
  boost::asio::io_service ios_;
  std::vector<SpdyStreamSink*> connets_;
  boost::scoped_ptr<SpdyStreamSink> sink_;
};
/*
class SpdySessionPool {
public:
  SpdyConnector * GetConnector(const std::string & url);

  SpdySessionPool(boost::asio::io_service &ios)
    : ios_(ios) {}

private:
  boost::asio::io_service & ios_;
  std::map<Address, SpdyConnector> map_;
};
*/
}
#endif
