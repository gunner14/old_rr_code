#include "net/spdy_sess.h"
#include <stdio.h>
#include <map>

using namespace net;
class VldbClient : public SpdyStreamSink {
 public:
  VldbClient(boost::asio::io_service & ios) :
    base_(ios, 1) {
  }
  struct RequestInfo {
    std::string method; 
    std::string url;
    const char* data;
    size_t length;
  };

  bool Start(const Address &addr) {
    if (!base_.Connect(addr))
      return false;
    base_.Start(this);
    return true;
  }

  bool Request(const std::string& method, const std::string& url, const char* data = NULL, size_t length = 0, std::string *out = NULL) {
    RequestInfo ri= { method, url, data, length};
    spdy::SpdyStreamId id = base_.PutFrame(0, method, NULL, url, data, length);
    if (id > 0) {
      req_map_.insert(std::make_pair(id, ri));
    }
  }

  // 回调函数
  bool AsyncRequest(const std::string& method, const std::string& url, const char* data = NULL, size_t length = 0, ) {
    RequestInfo ri= { method, url, data, length};
    spdy::SpdyStreamId id = base_.PutFrame(0, method, NULL, url, data, length);
    if (id > 0) {
      req_map_.insert(std::make_pair(id, ri));
    }
  }
  
  // 轮询结果, 0 没有结果返回, > 0, 返回data字节数
  int AsyncGetResult(spdy::SydyStreamID id, char* data, size_t length) {
  }

  void OnStream(spdy::SpdyStreamId id, const spdy::SpdyHeaderBlock *block, const char* buf, int sz) {
    std::map<spdy::SpdyStreamId, RequestInfo>::iterator it = req_map_.begin();
    if (it != req_map_.end()) {
      // 服务端回调客户端
      printf("server call back myself:%d\n%s", id, buf);
    } else {


    }
      
    printf("fetch data secuess:%d\n%s", id, buf);
    base_.WillClose();
  }

  void OnError(bool send, spdy::SpdyStreamId id, int error) {
    if (send) {
      if (id)
        req_map_.erase(id);
    }
    printf("fetch data error:%d", error);
    base_.WillClose();
  } 

  SpdyStreamSink* OnNew(boost::asio::io_service & ios) {
    return new VldbClient(ios);
  }

  SpdyConnector* GetContext() {
    return &base_;
  }

 private:
  SpdyConnector base_;
  std::map<spdy::SpdyStreamId, RequestInfo> req_map_;

private:

  void HandleResolve(const boost::system::error_code& err,
                      tcp::resolver::iterator endpoint_iterator);

  void HandleConnect(const boost::system::error_code& err,
                      tcp::resolver::iterator endpoint_iterator);

};

int main() {
  boost::asio::io_service io_service;

  using namespace net;
  std::string value1("{value:\'this is a test\'}");
  std::string value2("{value:\'that is a test\'}");
  VldbClient conn(io_service);
  conn.Start(std::make_pair("10.22.206.180", "3001"), 300);
  conn.Request("PUT", "http://xoa.xiaonei.com/storage/open?dbid=1");
  conn.Request("PUT", "http://xoa.xiaonei.com/storage/add?dbid=1&pageid=200435454&itemid=101", value1.c_str(), value1.size());
  conn.Request("PUT", "http://xoa.xiaonei.com/storage/add?dbid=1&pageid=200435454&itemid=102", value2.c_str(), value2.size());
  conn.Request("GET", "http://xoa.xiaonei.com/storage/get-page-size?dbid=1&pageid=200435454", NULL, 0);
  conn.Request("GET", "http://xoa.xiaonei.com/storage/get-page?dbid=1&pageid=200435454&index=0", NULL, 0);
  conn.Request("GET", "http://xoa.xiaonei.com/storage/get?dbid=1&pageid=200435454&itemid=101", NULL, 0);
  conn.Request("GET", "http://xoa.xiaonei.com/storage/get-size?dbid=1", NULL, 0);
  conn.Request("PUT", "http://xoa.xiaonei.com/storage/close?dbid=1");

  io_service.run();

  // GET, url
  return 0;
}
