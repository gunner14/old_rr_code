#include "net/spdy_server.h"

using namespace xoa;
class EchoSession : public SpdySession {
 public:
  EchoSession(boost::asio::io_service & ios)
  : SpdySession(ios) {

  }

  void OnError(bool send, spdy::SpdyStreamId id, int error) {
    printf("client stream(%d) have error:%d\n", id, error);
  }

  SpdyStreamSink* OnNew(boost::asio::io_service & ios) {
    return new EchoSession(ios);
  }

  virtual void OnRequest(SpdyConnector &context,
                 spdy::SpdyStreamId id, const spdy::SpdyHeaderBlock &block, 
                 const std::string &path, const std::map<std::string,std::string> &param, 
                 const char* data, int length) {
    PutBson(id, "200 OK", "a:{id=31,key=\'this is a test\'}");
  }

};

int main() {

  using namespace net;
  
  printf("start run server\n");
  SpdyServer server;
  EchoSession ss(server.io_service()); // only used for clone
  server.Set(&ss);
  server.Listen(std::make_pair("0.0.0.0", "4000"));
  server.Run();
  return 0;
}
