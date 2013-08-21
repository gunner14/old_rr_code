
#include "net/spdy_session.h"

#include <iostream>
#include <boost/lexical_cast.hpp>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "base/logging.h"
#include "googleurl/src/gurl.h"
#include "net/spdy/spdy_protocol.h"

#if defined(__GNUC__)
#define DEBUG_TRAP __asm__("int $3")
#elif defined(_MSC_VER)
#define DEBUG_TRAP __asm int 3
#endif

namespace net {

void SpdyConnector::Start(SpdyStreamSink* sink) {
  sink_ = sink;

  spdy_framer_->set_visitor(this);
  response_.resize(512);

  DoRead();
}

// TODO: new_stream = true
void SpdyConnector::Request(const std::string& method, const std::string& url
                            , SpdyStreamSink* sink
                            , const char* data, std::size_t length) {
  spdy_framer_->set_visitor(this);
  response_.resize(512);

  // 1 check if Connected, if not call Connect
  // 2 CreateSynStream and put into queue
  
  spdy::SpdyHeaderBlock block;
  block["method"] = method;
  block["url"] = url;
  block["version"] = "HTTP/1.1";

  spdy::SpdyStreamId id = GetNewStreamId();

  spdy::SpdySynStreamControlFrame* syn =
    spdy_framer_->CreateSynStream(id
      , 0 // associated
      , 0 // priority
      , spdy::CONTROL_FLAG_FIN
      , true // compressed
      , &block);

  // TODO: lock
  //if (sink)
  //  streammap_.insert(std::make_pair(id, StreamClient(syn, sink)));

  if (connected_) {
    DoWrite(id, syn); // TODO: syn 的生存期，何时释放呢？
  }
  else {
    GURL gu(url);
    Connect(std::make_pair(gu.host(), gu.port()));

    // TODO: lock
    send_queue_.push_back(id);
  }
}

bool SpdyConnector::Connect(const Address& address) {
  tcp::resolver::query query(address.first, address.second);
  resolver_.async_resolve(query,
    boost::bind(&SpdyConnector::HandleResolve, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::iterator));
  return true;
}

void SpdyConnector::SinkHeaders(spdy::SpdyStreamId id, spdy::SpdyFrame * header_frame) {
  StreamMapType::iterator i = streammap_.find(id);
  if (i == streammap_.end()) {
    // copy headers data, better way?
    spdy::SpdyFrame * copy = new spdy::SpdyFrame(header_frame->length());
    memcpy(copy->data(), header_frame->data(), header_frame->length());
    StreamClient sc = StreamClient(copy, sink_);
    streammap_[id] = sc;
  }

  i = streammap_.find(id);
  DCHECK(i != streammap_.end());
  StreamClient & sc = i->second;
  if (sc.sink_) {

    spdy::SpdyHeaderBlock request_headers;
    bool parsed_headers = spdy_framer_->ParseHeaderBlock(sc.header_frame_, &request_headers);
    if (parsed_headers) {
//       LOG(INFO) << "   SYN_STREAM stream_id:" << id
//         << " associated stream_id:" << sc.header_frame_->associated_stream_id()
//         << " priority:" << (int)sc.header_frame_->priority()
//         << " request headers size:" << request_headers.size();

      sc.sink_->OnHeaders(id, &request_headers);
    }
  }
}

void SpdyConnector::SinkData(spdy::SpdyStreamId id, const char* data, size_t len) {
  StreamMapType::iterator i = streammap_.find(id);
  DCHECK(i != streammap_.end());
  if (i != streammap_.end()) {
    StreamClient & sc = i->second;
    if (sc.sink_)
      sc.sink_->OnData(id, data, len);
  }
}

void SpdyConnector::SinkRequest(spdy::SpdyStreamId id, spdy::SpdyHeaderBlock * headers) {
  StreamMapType::iterator i = streammap_.find(id);
  DCHECK(i != streammap_.end());
  if (i != streammap_.end()) {
    StreamClient & sc = i->second;
    if (sc.sink_)
      sc.sink_->OnRequest(id, headers, 0, 0);
  }
}

void SpdyConnector::HandleResolve(const boost::system::error_code& err,
  tcp::resolver::iterator endpoint_iterator) {
    if (!err) {
      tcp::endpoint endpoint = *endpoint_iterator;
      socket_.async_connect(endpoint,
        boost::bind(&SpdyConnector::HandleConnect, this,
        boost::asio::placeholders::error, ++endpoint_iterator));
      return;
    }
    
    // TODO: handle error
}

void SpdyConnector::HandleConnect(const boost::system::error_code& err,
  tcp::resolver::iterator endpoint_iterator) {
  if (!err) {
    // TODO: lock
    connected_ = true;
    for (StreamQueueType::const_iterator i = send_queue_.begin();
      i != send_queue_.end(); ++i) {
        spdy::SpdyStreamId id = *i;
        StreamMapType::const_iterator j = streammap_.find(id);
        if (j != streammap_.end() && j->second.frame_to_write_)
          DoWrite(id, j->second.frame_to_write_);
    }
  }
  else if (endpoint_iterator != tcp::resolver::iterator()) {
    // The connection failed. Try the next endpoint in the list.
    socket_.close();
    tcp::endpoint endpoint = *endpoint_iterator;
    socket_.async_connect(endpoint,
      boost::bind(&SpdyConnector::HandleConnect, this,
      boost::asio::placeholders::error, ++endpoint_iterator));
  }
  // TODO: handle error
  else {
    std::cout << "HandleConnect Error: " << err.message() << "\n";
  }
}

void SpdyConnector::HandleWriteRequest(spdy::SpdyStreamId id
                  , const boost::system::error_code& err
                  , size_t bytes_transferred) {
  // TODO: lock
  StreamMapType::iterator i = streammap_.find(id);
  DCHECK(i != streammap_.end());
  if (i != streammap_.end()) {
    // delete i->second.frame_;
    streammap_.erase(i);

    // TODO: 需要通知请求发送成功么?
  }

  if (!err) {
    DoRead();
    return;
  }

  // TODO: handle error
  std::cout << "Error: " << err.message() << "\n";
}

void SpdyConnector::HandleReadSome(const boost::system::error_code& err, size_t bytes_transferred) {
  // TODO: handle error
  if (err) {
    // __asm int 3;
    std::cout << "HandleReadSome Error: " << err.message() << "\n";
  }

  size_t bytes_processed = spdy_framer_->ProcessInput(&response_[0], bytes_transferred);
  if (!spdy_framer_->MessageFullyRead()) {
    DoRead();
  }
}

void SpdyConnector::DoRead() {
  // async_read_some is better then async_read
  // 有助于减少每个连接所用内存数
  response_.resize(512);

  socket_.async_read_some(boost::asio::buffer(response_),
  // boost::asio::async_read(socket_, boost::asio::buffer(response_), 
  //      boost::asio::transfer_at_least(spdy::SpdyFrame::size()),
        boost::bind(&SpdyConnector::HandleReadSome, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
}

void SpdyConnector::DoWrite(spdy::SpdyStreamId id, spdy::SpdyFrame* const frame) {
  boost::asio::async_write(socket_, 
      boost::asio::buffer(frame->data(), spdy::SpdyFrame::size() + frame->length()),
      boost::bind(&SpdyConnector::HandleWriteRequest, this,
          id,
          boost::asio::placeholders::error, 
          boost::asio::placeholders::bytes_transferred));
}

int SpdyConnector::GetNewStreamId() {
  int id = stream_id_;
  stream_id_ += 2;
  if (stream_id_ > 0x7fff)
    stream_id_ = 1;
  return id;
}

void SpdyConnector::OnError(spdy::SpdyFramer* framer) {
  LOG(INFO) << "OnError state:" << framer->state() << " " << framer->error_code();
}

void SpdyConnector::OnControl(const spdy::SpdyControlFrame* frame) {
  LOG(INFO) << "Got SpdyControlFrame"
    << " type:" << frame->type()
    << " version:" << frame->version()
    << " length:" << frame->length()
    << " flags:" << (int)frame->flags()
    ;

  if (spdy::SYN_STREAM == frame->type()) {
    spdy::SpdySynStreamControlFrame syn(frame->data(), false);

    SinkHeaders(syn.stream_id(), &syn);
#if 0
    spdy::SpdyHeaderBlock request_headers;
    bool parsed_headers = spdy_framer_->ParseHeaderBlock(frame, &request_headers);
    if (parsed_headers) {
      LOG(INFO) << "   SYN_STREAM stream_id:" << syn.stream_id()
        << " associated stream_id:" << syn.associated_stream_id()
        << " priority:" << (int)syn.priority()
        << " request headers size:" << request_headers.size();

      SinkHeaders(syn.stream_id(), &request_headers);
    }
#endif
  }
#if 0
      for (spdy::SpdyHeaderBlock::iterator i=request_headers.begin(); i!=request_headers.end();
        ++i) {
          LOG(INFO) << "  " << i->first << " " << i->second;
      }

      // TODO: sign event OnRequest(const spdy::SpdyHeaderBlock *, spdy::SpdySynStreamControlFrame);
    } else {
      // TODO: reset or ...
    }

    // TODO: save stream_id
    // prepare a SYN_REPLY
    spdy::SpdyHeaderBlock headers;
    headers["status"] = "200 OK";
    headers["version"] = "HTTP/1.1";
    headers["content-length"] = "0";
    spdy::SpdySynReplyControlFrame* reply = spdy_framer_->CreateSynReply(syn.stream_id()
      , spdy::CONTROL_FLAG_FIN
      , true // compressed
      , &headers);
    DoWrite(syn.stream_id(), reply);
  }
#endif
}

void SpdyConnector::OnStreamFrameData(spdy::SpdyStreamId stream_id,
                               const char* data,
                               size_t len) {
  LOG(INFO) << "OnStreamFrameData stream_id:" << stream_id << " " << " len:" << len;
  LOG_IF(INFO, len) << "  " << std::string(data, len);

  SinkData(stream_id, data, len);
}


//
void SpdyServer::Listen(const Address & address) {
  io_service_.reset(new boost::asio::io_service);
  // TODO: exception
  // TODO: address.first = ip
  int port = boost::lexical_cast<int>(address.second);
  acceptor_.reset(new tcp::acceptor(*io_service_, tcp::endpoint(tcp::v4(), port)));

  SpdyConnector* conn = new SpdyConnector(*io_service_);
  acceptor_->async_accept(conn->socket(), boost::bind(&SpdyServer::HandleAccept, this,
        boost::asio::placeholders::error, conn));
}


void SpdyServer::Run() {
  // TODO: use thread pool and io_service pool
  io_service_->run();
}

void SpdyServer::OnRequest(spdy::SpdyStreamId id, spdy::SpdyHeaderBlock * headers, const char* data, size_t length) {
  DEBUG_TRAP;
  
  spdy::SpdyHeaderBlock reply_headers;
  reply_headers["status"] = "200 OK";
  reply_headers["version"] = "HTTP/1.1";
  reply_headers["content-length"] = "0";
  spdy::SpdySynReplyControlFrame* reply = conn_->framer()->CreateSynReply(id
    , spdy::CONTROL_FLAG_FIN
    , true // compressed
    , &reply_headers);
  conn_->DoWrite(id, reply);
  delete reply; // TODO: is right?
}

void SpdyServer::HandleAccept(const boost::system::error_code& error, SpdyConnector* new_connection) {
  // TODO: check error
  if (!error) {
    printf("New Connecttion:%s,%s\n",new_connection->socket().local_endpoint().address().to_string().c_str(),
                                     new_connection->socket().remote_endpoint().address().to_string().c_str());
    //std::cout << "New Connecttion" << new_connection->socket().local_endpoint().address().to_string();
    new_connection->Start(this);

    conn_.reset(new SpdyConnector(*io_service_));
    acceptor_->async_accept(conn_->socket(), boost::bind(&SpdyServer::HandleAccept, this,
          boost::asio::placeholders::error, conn_.get()));
  } else {
    std::cout << "HandleAccept Error: " << error.message() << "\n";
  }
}




SpdyConnector * SpdySessionPool::GetConnector(const std::string & url) {
  // from url extract [hostname, port]
  // query map<Address, Connection*>
  // resolve
  // 
  return new SpdyConnector(ios_);
}

}
