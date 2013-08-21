#ifndef NET_SPDY_SESSION_H__
#define NET_SPDY_SESSION_H__

#include <deque>

#include "net/asio_forward.hpp"
#include "net/spdy/spdy_framer.h"

namespace net {

class HttpRequestHeaders;
class HttpResponseInfo;

using boost::asio::ip::tcp;

// Host Name(ip) and port
typedef std::pair<std::string, std::string> Address;

struct SpdyStreamSink {
  // c/s
  virtual void OnHeaders(spdy::SpdyStreamId, spdy::SpdyHeaderBlock *) = 0;
  // c/s
  virtual void OnData(spdy::SpdyStreamId, const char*, size_t) = 0;

  // c
  virtual void OnResponse(spdy::SpdyStreamId, spdy::SpdyHeaderBlock *
              , const char*, size_t) = 0;

  // c/s for normal get
  // cache Header SpdyFrame, call in OnData
  // 
  virtual void OnRequest(spdy::SpdyStreamId, spdy::SpdyHeaderBlock *, const char*, size_t) = 0;
};

class SpdyConnector : public spdy::SpdyFramerVisitorInterface {
public:
  SpdyConnector(boost::asio::io_service & ios) : 
      stream_id_(1),
      resolver_(ios),
      socket_(ios),
      spdy_framer_(new spdy::SpdyFramer),
      connected_(false),
      sink_(NULL) {}

  void Start(SpdyStreamSink* sink);
  void Request(const std::string& method, const std::string& url
          , SpdyStreamSink* sink = NULL
          , const char* data = NULL, std::size_t length = 0);

  // SpdyFramerVisitorInterface
  // Called if an error is detected in the SpdyFrame protocol.
  virtual void OnError(spdy::SpdyFramer* framer);

  // Called when a Control Frame is received.
  virtual void OnControl(const spdy::SpdyControlFrame* frame);

  // Called when data is received.
  // |stream_id| The stream receiving data.
  // |data| A buffer containing the data received.
  // |len| The length of the data buffer.
  // When the other side has finished sending data on this stream,
  // this method will be called with a zero-length buffer.
  virtual void OnStreamFrameData(spdy::SpdyStreamId stream_id,
                                 const char* data,
                                 size_t len);

protected:
  friend class SpdyServer;

  tcp::socket & socket() {
    return socket_;
  }

  // TODO: very bad
  spdy::SpdyFramer* framer() {
    return spdy_framer_.get();
  }

  void DoRead();
  void DoWrite(spdy::SpdyStreamId, spdy::SpdyFrame * const);

  int GetNewStreamId();

  bool Connect(const Address& address);

private:
  void SinkHeaders(spdy::SpdyStreamId, spdy::SpdyFrame * const);
  void SinkData(spdy::SpdyStreamId, const char*, size_t);
  void SinkRequest(spdy::SpdyStreamId, spdy::SpdyHeaderBlock *);

  void HandleResolve(const boost::system::error_code& err,
                      tcp::resolver::iterator endpoint_iterator);

  void HandleConnect(const boost::system::error_code& err,
                      tcp::resolver::iterator endpoint_iterator);

  void HandleWriteRequest(spdy::SpdyStreamId, const boost::system::error_code& err, size_t bytes_transferred);
  void HandleReadSome(const boost::system::error_code& err, size_t bytes_transferred);

  int stream_id_; // The next stream id, initialize as 1
  
  tcp::resolver resolver_;
  tcp::socket socket_;

  std::vector<char> response_;

  boost::scoped_ptr<spdy::SpdyFramer> spdy_framer_;

  struct StreamClient {
    spdy::SpdyFrame * header_frame_; // TODO: scoped_ptr?
    SpdyStreamSink * sink_;
    spdy::SpdyFrame * frame_to_write_;

    StreamClient(spdy::SpdyFrame* frame = NULL, SpdyStreamSink* sink = NULL) 
          : header_frame_(frame), sink_(sink), frame_to_write_(NULL) {}
    ~StreamClient() {
      //delete header_frame_;
      //delete frame_to_write_;
    }
  };

  typedef std::map<spdy::SpdyStreamId, StreamClient> StreamMapType;
  StreamMapType streammap_;
  typedef std::deque<spdy::SpdyStreamId> StreamQueueType;
  StreamQueueType send_queue_; // ?

  bool connected_;
  SpdyStreamSink* sink_;
};


class SpdyServer : public SpdyStreamSink {
public:
  void Listen(const Address & address);
  void Run();

  // 
  virtual void OnHeaders(spdy::SpdyStreamId, spdy::SpdyHeaderBlock *) {}
  virtual void OnData(spdy::SpdyStreamId, const char*, size_t) {}
  virtual void OnResponse(spdy::SpdyStreamId, spdy::SpdyHeaderBlock *
        , const char*, size_t) {}

  virtual void OnRequest(spdy::SpdyStreamId, spdy::SpdyHeaderBlock *, const char*, size_t);

private:
  void HandleAccept(const boost::system::error_code& error, SpdyConnector* new_connection);

  boost::scoped_ptr<boost::asio::io_service> io_service_;
  boost::scoped_ptr<tcp::acceptor> acceptor_;
  boost::scoped_ptr<SpdyConnector> conn_;
};


class SpdySessionPool {
public:
  SpdyConnector * GetConnector(const std::string & url);

  SpdySessionPool(boost::asio::io_service &ios)
    : ios_(ios) {}

private:
  boost::asio::io_service & ios_;
  std::map<Address, SpdyConnector> map_;
};

}
#endif
