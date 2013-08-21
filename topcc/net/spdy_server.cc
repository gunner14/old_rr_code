#include "net/spdy_server.h"
#include "net/url_parse.h"

//#include <boost/bind.hpp>

namespace xoa {
SpdySession::SpdySession(boost::asio::io_service & ios) : 
  base_(ios, this, 2) {}

void SpdySession::OnStream(spdy::SpdyStreamId id, const spdy::SpdyHeaderBlock * block, const char* buf, int sz) {
  if (buf) {
    char pp[1024]="";
    const int len = sizeof(pp) - 1;
    int cpsz = len > sz ? sz : len;
    memcpy(pp, buf, cpsz);
    pp[cpsz] = 0;
    printf("OnStream :%d\nbuf:%s", id, buf);
  } else {
    printf("OnStream :%d\n", id);
  }
  spdy::SpdyHeaderBlock::const_iterator it = block->find(std::string("url"));
  if (it != block->end()) {
    arch::UrlParse up(it->second);
    if (up.valid()) {
      OnRequest(base_, id, *block, up.path(), up.param(), buf, sz);
    } else {
      // TODO: 
    }
  }
}

  void SpdySession::OnError(bool send, spdy::SpdyStreamId id, int error, int mod) {

  } 

  SpdyStreamSink* SpdySession::OnNew(boost::asio::io_service & ios) {
    return new SpdySession(ios);
  }

  SpdyConnector* SpdySession::GetContext() {
    return &base_;
  }

  spdy::SpdyStreamId SpdySession::PutBson(spdy::SpdyStreamId id, const std::string &code, const std::string &bson) {
    char slen[20];
    spdy::SpdyHeaderBlock reply_headers;
    if (!code.empty())
      reply_headers["status"] = code; // "200 OK";
    if (!bson.empty()) {
      sprintf(slen, "%d", bson.size());
      reply_headers["content-length"] = slen;
    }
    return  base_.PutFrame(id,
                      "",
                      "",
                      &reply_headers,
                      "",
                      bson.c_str(),
                      bson.size(),
                      true);
  }

  void SpdySession::OnRequest(SpdyConnector &context,
                 spdy::SpdyStreamId id, const spdy::SpdyHeaderBlock &block, 
                 const std::string &path, const std::map<std::string, std::string> &param, 
                 const char* data, int length) {
  }

} // namespace

