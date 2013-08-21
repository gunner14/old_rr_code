
#include "net/spdy_client.h"
#include "stdio.h"
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include "net/spdy_registry.h"

namespace xoa {
const std::string ACCEPT_TEXT_PLAIN = "text/plain";
const std::string ACCEPT_JSON = "application/json";
const std::string ACCEPT_SERIALIZABLE = "application/serializable";
const std::string ACCEPT_PROTOC_BUFF = "application/protoc-buff";
const int kMaxMod = 1; //暂时固定

XoaClient::XoaClient(XoaRegistry* reg)
  : ipreg_(reg)
  , t_(io_service_, boost::posix_time::seconds(2)) {
  send_timeout_ = 1000;
  work_.reset(new boost::thread(boost::bind(&XoaClient::Run, this)));
  for (int i = 0; i < kMaxMod; ++i) {
    connects_.push_back(std::make_pair(tcp::endpoint(), new SpdyConnector(io_service_, this, 1)));
  }
}

XoaClient::~XoaClient() {
  io_service_.post(boost::bind(&XoaClient::WillClose, this));
  std::vector<std::pair<tcp::endpoint, boost::shared_ptr<SpdyConnector> > >::iterator it = connects_.begin();
  for (; it != connects_.end(); ++it) {
    if (it->first.port() != 0)
      io_service_.post(boost::bind(&SpdyConnector::WillClose, it->second.get()));
  }
  //io_service_.stop();
  work_->join();
}

void XoaClient::WillClose() {
  t_.cancel();
}

bool Connect(const tcp::endpoint &ep, tcp::socket *socket) {
  boost::system::error_code ec = boost::asio::error::host_not_found; // 连接/读写时的错误码
  if (!socket)
    return false;

  socket->connect(ep, ec);
  if (!ec) {
    return true;
  } else {
    LOG(LS_WARNING) <<"connect " << ep.address().to_string().c_str() << " is error:" <<  boost::system::system_error(ec).what();
    return false;
  }
}

SpdyConnector* XoaClient::CheckConnect(int mod) {
  std::pair<tcp::endpoint, boost::shared_ptr<SpdyConnector> > &pr = connects_[mod];
  if (pr.first.port() == 0) {
    if (TryConnect(&pr.first, &pr.second->socket(), mod)) {
      pr.second->Boost(mod);
    } else {
      return NULL;
    }
  }
  return pr.second.get();
}

void XoaClient::OnSubmit(const RequestInfo &ri) {
  SpdyConnector * conn = CheckConnect(ri.method.mod());
  if (!conn) {
    XoaResponse rep;
    rep.status_ = 510;
    rep.error_ = kNoValidHost;
    if (ri.callback)
      ri.callback->Callback(rep);
    return;
  }
  const Method & method = ri.method;
  spdy::SpdyStreamId id = conn->PutFrame(0,
                                method.host(),
                                method.method(), &method.heads(), method.url(), 
                                method.content().c_str(), method.content().size(),
                                true);
  if (id > 0) {
    req_map_.insert(std::make_pair(id, ri));
  }
}

bool XoaClient::Submit(const Method &method, XoaResponseCallback *cb, int timeout) {
  RequestInfo ri = {method, cb, timeout};
  io_service_.post(boost::bind(&XoaClient::OnSubmit, this, ri));
  return true;
}

struct CallbackHandle : public  XoaResponseCallback{
  boost::mutex *lock_; 
  XoaResponse *resp_;
  void Callback(const XoaResponse &xoa) {
    if (resp_)
      *resp_ = xoa;
    lock_->unlock();
  }
};

bool XoaClient::Execute(const Method &method, XoaResponse *resp, int timeout) {
  CallbackHandle ch;
  boost::mutex lock;
 
  ch.lock_ = &lock;
  ch.resp_ = resp;
  lock.lock();
  Submit(method, &ch, timeout);
  lock.lock();
  lock.unlock();

  return resp->status_ == 200;
}

bool XoaClient::TryConnect(tcp::endpoint *endip, tcp::socket *sock, int mod) {
  if (!endip || !sock)
    return false;
  if (endip->port() !=0) {
    ipreg_->Trash(*endip, mod);
    endip->port(0);
  }
  for (int i; ipreg_->SelectIp(endip, mod); i++ ) {
 
    if (!Connect(*endip, sock)) {
      ipreg_->Trash(*endip, mod);
      endip->port(0);
    } else {
      return true;
    }
  }
  return false;
}

void XoaClient::ResendRequest(SpdyConnector& conn) {

  // 把未成功的重新发一次
  std::map<spdy::SpdyStreamId, RequestInfo> tmp;
  std::map<spdy::SpdyStreamId, RequestInfo>::iterator it = req_map_.begin();
  for(; it != req_map_.end(); ++it) {
    Method &met = it->second.method;
    spdy::SpdyStreamId id = conn.PutFrame(0,
                                        met.host(),
                                        met.method(), &met.heads(), met.url(), 
                                        met.content().c_str(), met.content().size(),
                                        true);
    tmp.insert(std::make_pair(id, it->second));
  }
  tmp.swap(req_map_);
}

void XoaClient::OnError(bool send, spdy::SpdyStreamId id, int error, int mod) {
  //if (error == kSendStreamTimeout || error == kReceiveReplyTimeout)
  //  return;
  std::pair<tcp::endpoint, boost::shared_ptr<SpdyConnector> > &pr = connects_[mod];
  if (TryConnect(&pr.first, &pr.second->socket(), mod)) {
    pr.second->Boost(mod);
    ResendRequest(*pr.second.get());
  } else {
    std::map<spdy::SpdyStreamId, RequestInfo>::iterator it = req_map_.begin();
    for(; it != req_map_.end(); ++it) {
      XoaResponse rep;
      rep.status_ = 501;
      rep.error_ = kNoValidHost;
      it->second.callback->Callback(rep);
    }
    req_map_.clear();
  }
  // LOG(LS_WARNING) << "OnError: spdy protocol error or net error :" << error;
  // base_.WillClose(); // base 已经willclose
}

void XoaClient::OnStream(spdy::SpdyStreamId id, const spdy::SpdyHeaderBlock *block, const char* buf, int sz) {
  std::map<spdy::SpdyStreamId, RequestInfo>::iterator it = req_map_.find(id);
  if (it != req_map_.end()) {
    if (it->second.callback) {
      XoaResponse rep;
      rep.status_ = 200;
      rep.content_.assign(buf, sz);
      if (block) {
        spdy::SpdyHeaderBlock::const_iterator it = block->find("Content-Type");
        if (it != block->end())
          rep.content_type_ = it->second;
      }
      it->second.callback->Callback(rep);
      req_map_.erase(it);
    } else {
      LOG(LS_ERROR) << "OnStream: no exist streamid";
    }
  }
}

void XoaClient::KeepTime(const boost::system::error_code &e) {
  if (e) {
    if (boost::asio::error::operation_aborted == e)
      LOG(LS_INFO) << "Keep Time normal exit.";
    else
      LOG(LS_WARNING) << "Keeptime Error reason: " << boost::system::system_error(e).what();
    return;
  }
  LOG(LS_VERBOSE) << ("<>KeepTime");
  t_.expires_at(t_.expires_at() + boost::posix_time::seconds(2));
  t_.async_wait(boost::bind(&XoaClient::KeepTime, this, boost::asio::placeholders::error));
}

void XoaClient::Run() {
  t_.async_wait(boost::bind(&XoaClient::KeepTime, this, boost::asio::placeholders::error));
  io_service_.run();
}

} //namespace

