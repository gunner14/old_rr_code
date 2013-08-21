#include "upload/download.h"
#include "base3/logging.h"

namespace net {
const int KRequestTimeout = 10;

Download::Download(boost::asio::io_service &ios)
: ios_(ios)
, will_exit_(false)
, max_reconnect_(10) {
}

void Download::Clear() {
  for (std::map<tcp::endpoint, ValidConnect*>::iterator it = cvalid_.begin(); it != cvalid_.end(); it++) {
    delete it->second->handler;
    delete it->second;
    LOG(INFO) << "free validconnect"; // TODO:test
  }
  cvalid_.clear();
}

bool Download::Connect(const tcp::endpoint &ep, tcp::socket *socket) {
  boost::system::error_code ec = boost::asio::error::host_not_found; // 连接/读写时的错误码
  if (!socket)
    return false;

  LOG_IF(INFO,kPhotoDetectLog) << "connect " << ep.address().to_string().c_str();
  socket->connect(ep, ec);
  if (!ec) {
    return true;
  } else {
    LOG(WARNING) <<"connect " << ep.address().to_string().c_str() << " is error:" <<  boost::system::system_error(ec).what();
    return false;
  }
}

void Download::RecheckTimeout() {

  time_t now = time(NULL);
  if (!cvalid_.empty()) {
    std::map<tcp::endpoint, ValidConnect*>::iterator it;
RECHECK:
    it= cvalid_.begin();
    for (; it != cvalid_.end(); ++it) {
      if (it->second->idle == false) {
        if (now - it->second->last_time > KRequestTimeout) { // 请求超时,按连接关闭算
          LOG(WARNING) <<"connect is timeout: " << it->second->endpoint.address().to_string().c_str() << ",now time: " << it->second->last_time << ", now close it";
          it->second->idle = true;
          //HandleError(true, it);
	  boost::system::error_code ec;
	  it->second->sock.close(ec);
          goto RECHECK;
	}
      }
    }
  }

}

bool Download::Idle(tcp::endpoint *ep, ValidConnect** vi) {
  std::map<tcp::endpoint, ValidConnect*>::iterator it = cvalid_.begin();
  for (; it != cvalid_.end(); ++it) {
    if (it->second->idle == true) {
      it->second->idle = false;
      if (vi)
        *vi = it->second;
      if (ep)
        *ep = it->first;
      return true;;
    }
  }
  RecheckTimeout();
  return false;
}

void Download::PrecessJob(const tcp::endpoint &ep, ValidConnect *vi) {
  
  if (!vi->handler->OnSendData(vi->endpoint.address().to_string().c_str(), &vi->buffer)) {
    LOG_IF(INFO,kPhotoDetectLog) << ("<> PrecessJob no job");
    vi->idle = true;
    return ;
  }
  LOG_IF(INFO,kPhotoDetectLog) << ("<> ProcessJob hav job");
  if (ConfirmConnect(vi)) {
    boost::asio::async_write(vi->sock,
                           boost::asio::buffer(vi->buffer.ptr(), vi->buffer.size()), // TODO:是否自己复制一份?
                           boost::bind(&Download::HandleWrite,
                             this,
                             ep,
                             boost::asio::placeholders::error,
                             boost::asio::placeholders::bytes_transferred)
                           );
  }
}

void Download::HandleError(bool closed, const std::map<tcp::endpoint, ValidConnect*>::iterator &it) {
  boost::system::error_code ec;
  it->second->sock.close(ec);
  it->second->handler->OnError();
  if (!closed) {
    delete it->second->handler;
    delete it->second; 
    cvalid_.erase(it);
  } else {
    it->second->buffer.Reset();
    it->second->idle = true;
    ios_.post(boost::bind(&Download::BoostJob, this));
  }
}

bool Download::ConfirmConnect(ValidConnect *vi) {
  if (!vi)
    return false;
  time_t now = time(NULL);
  if (now - vi->last_time > max_reconnect_) {
    if (vi->sock.is_open()) // 如果链接还活着的强制关闭,不知道会不会有问题
      vi->sock.close();
    LOG_IF(INFO,kPhotoDetectLog) << "<> connect long time no keeplive,reconnect it:" << vi->endpoint.address().to_string();
    //sock.remote_endpoint().address().to_string().c_str(
  }
	  
  if (!vi->sock.is_open()) {
    if( Connect(vi->endpoint, &vi->sock) ) {
      vi->last_time = now;
      return true;
    } else {
      return false;
    }
  } else {
    vi->last_time = now;
    return true;
  }
}

void Download::HandleWrite(const tcp::endpoint &ep, const boost::system::error_code& err, size_t bytes_transferred) {
  std::map<tcp::endpoint, ValidConnect*>::iterator it = cvalid_.find(ep);
  if (it == cvalid_.end())
    return;
  if (will_exit_)
    return;
  LOG_IF(INFO,kPhotoDetectLog) << ("<> HandleWrite");
  if (err) {
    if (boost::asio::error::broken_pipe == err) {
      LOG(WARNING) << "Write Net Data Error: Broken Pipe.";
      HandleError(true, it);
    } else if(boost::asio::error::operation_aborted == err) {
      LOG(WARNING) << "Write Net Data Error: Operation_aborted";
      HandleError(true, it);
    } else {
      LOG(WARNING) << "Write Net Data Error: " << "reason: " << boost::system::system_error(err).what();
      HandleError(true, it);
    }
    return;
  } else {
    it->second->last_time = time(NULL);
    boost::asio::async_read_until(it->second->sock,
                           it->second->recv,
                           "\r\n\r\n",
                           boost::bind(&Download::HandleReadHead,
                             this,
                             ep,
                             boost::asio::placeholders::error,
                             boost::asio::placeholders::bytes_transferred)
                           );
  }
}

void Download::HandleReadHead(const tcp::endpoint& ep, const boost::system::error_code& err, size_t bytes_transferred) {
  std::map<tcp::endpoint, ValidConnect*>::iterator it = cvalid_.find(ep);
  if (it == cvalid_.end())
    return;
  if (will_exit_)
    return;

  LOG_IF(INFO,kPhotoDetectLog) << ("<> HandleReadHead");
  if (err) {
    if (boost::asio::error::eof == err) {
      LOG(WARNING) << "Read Data Error: Connection is closed.";
      HandleError(true, it);
    } else if(boost::asio::error::operation_aborted == err) {
      LOG(WARNING) << "Read Data Error: Operation_aborted";
      HandleError(true, it);
    } else {
      LOG(WARNING) << "Read Data Error: " << "reason: " << boost::system::system_error(err).what();
      HandleError(true, it);
    }
  } else {
    it->second->last_time = time(NULL);
    std::istream response_stream(&it->second->recv);
    std::string header;
    std::vector<std::string> heads;
    it->second->chunk= false;
    
    int leftnumb = 0;
    while (std::getline(response_stream, header) && header != "\r") {
      if (header == "Transfer-Encoding: chunked\r") {
        it->second->chunk= true;
      } else if (header.find("Content-Length:") == 0) {
        std::string numstr = header.substr(16);
        leftnumb = atoi(numstr.c_str());
       // Content-Length:2973 
      } /*else if (header == "Content-Encoding:gzip\r") {
      } */
      heads.push_back(header);
      LOG_IF(INFO,kPhotoDetectLog) << "[ " << (header.c_str()) << " ]";
    }

    it->second->handler->OnGetHead(heads);
    it->second->buffer.Reset();
    if (ConfirmConnect(it->second)) {
      if (!leftnumb) {
        if (it->second->chunk == true)
          boost::asio::async_read_until(it->second->sock,
                           it->second->recv,
                           "0\r\n\r\n",
                           boost::bind(&Download::HandleReadResult,
                             this,
                             ep,
                             boost::asio::placeholders::error,
                             boost::asio::placeholders::bytes_transferred)
                           );
      } else {
        int left = it->second->recv.size();
        if (left < leftnumb) {
          boost::asio::async_read(it->second->sock,
                           it->second->recv,//boost::asio::buffer(buf, 148),
                           boost::asio::transfer_at_least(leftnumb - left),
                           boost::bind(&Download::HandleReadResult,
                             this,
                             ep,
                             boost::asio::placeholders::error,
                             boost::asio::placeholders::bytes_transferred)
                           );
       } else {
         HandleReadResult(ep, boost::system::error_code(), leftnumb);
       }
      }
    }
  }
}

void Download::HandleReadResult(const tcp::endpoint &ep, const boost::system::error_code& err, size_t bytes_transferred) {
  std::map<tcp::endpoint, ValidConnect*>::iterator it = cvalid_.find(ep);
  if (it == cvalid_.end())
    return;
  if (will_exit_)
    return;
  LOG_IF(INFO,kPhotoDetectLog) << ("<> HandleReadResult\n");

  if (err) {
    if (boost::asio::error::eof == err) {
      LOG(WARNING) << "Read Data Reslut Error: Connection is closed.";
      HandleError(true, it);
    } else if(boost::asio::error::operation_aborted == err) {
      LOG(WARNING) << "Read Data Result Error: Operation_aborted";
      HandleError(true, it);
    } else {
      LOG(WARNING) << "Read Data Reslut Error: " << "reason: " << boost::system::system_error(err).what();
      HandleError(true, it);
    }
  } else {
    if (it->second->chunk) {
      std::istream response_stream(&it->second->recv);
      std::string tmp;
      std::string body;
      if (!std::getline(response_stream, tmp)) // filter  chunked number
        return;
      while (std::getline(response_stream, tmp) && tmp != "\r") {
        if (tmp == "0\r") // filter chunked number
          continue;
        LOG_IF(INFO,kPhotoDetectLog) << tmp; // TODO: test
        if (tmp[tmp.size()-1] == '\r')
          tmp.erase(tmp.size()-1);
        body += tmp;
      }
      it->second->handler->OnGetResult(body.c_str(), body.size());
    } else {
      const char* buf = boost::asio::buffer_cast<const char*>(it->second->recv.data());
      //size_t size = it->second->recv.size();
      it->second->handler->OnGetResult(buf, bytes_transferred);
      it->second->recv.consume(bytes_transferred);
    }

    PrecessJob(ep, it->second);
  }
}

void Download::Start(WebHandlerFactory *wf) {
  webfactory_ = wf;
};

bool Download::BoostJob() {
  bool idle = true;
  ValidConnect *vi;
  tcp::endpoint ep;

  if (Idle(&ep, &vi) == false) {
    // boost::mutex::scoped_lock alock(mutex_);
    LOG_IF(INFO,kPhotoDetectLog) << ("<> Post but Budy");
  } else {
    LOG_IF(INFO,kPhotoDetectLog) << ("<> Post Idle");
    PrecessJob(ep, vi);
    idle = true;
  }
  return idle;
}

void Download::BoostRegister(const std::vector<tcp::endpoint> &curl) {
  DLOG(INFO) << ("<> ReRegister ip address");
  if (Register(curl))
    ;
}

bool Download::Register(const std::vector<tcp::endpoint> &curl) {
  bool theseccess = false;
  time_t now = time(NULL);
  ValidConnect *vi = new ValidConnect(ios_);
  //LOG_IF(INFO) << "new validconnect1"; // TODO:test
  for (std::vector<tcp::endpoint>::const_iterator it = curl.begin(); it != curl.end(); it++) {
    if (cvalid_.find(*it) == cvalid_.end()) {
      if (!vi) {
        vi = new ValidConnect(ios_);
   //     LOG(INFO) << "new validconnect2"; // TODO:test
      }
      tcp::endpoint ep;
      if (Connect(*it, &vi->sock)) {
        vi->idle = true;
        vi->endpoint = *it;
        vi->last_time = now;
        vi->handler = webfactory_->Create();//new PhotoUploadHander(this);
        cvalid_.insert(std::make_pair(*it, vi));
    //    LOG(INFO) << "insert validconnect"; // TODO:test
        vi = NULL;
        theseccess = true;
      }
    }
  }
  if (vi) {
  //  LOG(INFO) << "delete validconnect"; // TODO:test
    delete vi;
  }
  return theseccess;
}

void Download::Close() {
  will_exit_ = true;
}


} // namespace
