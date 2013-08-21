#include "net/spdy_sess.h"

#include <iostream>
#include <boost/lexical_cast.hpp>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "base/logging.h"
//#include "googleurl/src/gurl.h"
#include "net/spdy/spdy_protocol.h"

namespace xoa {

const int kServerSendTimeout = 5000;

void SpdyConnector::OnError(spdy::SpdyFramer* framer) {
  //sink_->OnStream(0, NULL, NULL, framer->error_code());
  LOG(LS_WARNING) << "Parse Spdy Framer Error:"
               << "Reson: " << framer->ErrorCodeToString(framer->error_code());
  HandleError(false, 0,framer->error_code());
}

void SpdyConnector::Clear() {
  sink_ = NULL;
  last_time_ = 0;
  std::map<spdy::SpdyStreamId, StreamInfo>::iterator recv_it = recv_stream_map_.begin();
  for (; recv_it != recv_stream_map_.end(); ++recv_it) {
    delete recv_it->second.buffer;
  }
  recv_stream_map_.clear();
  send_stream_map_.clear();
}

void SpdyConnector::WillClose() {
  socket_.shutdown(boost::asio::socket_base::shutdown_both);
  socket_.close();
  Clear();
}

bool SpdyConnector::Boost(int mod) {
  mod_= mod;
  last_time_ = time(NULL);
  DoRead();
}

spdy::SpdyStreamId SpdyConnector::PutFrame(spdy::SpdyStreamId id, 
                              const std::string& host,
                              const std::string& method,
                              const spdy::SpdyHeaderBlock *bk,
                              const std::string& url,
                              const char* data,
                              int length,
                              bool async) {
  spdy::SpdyHeaderBlock block;
  if (!host.empty())
    block["Host"] = host;
  if (!method.empty())
    block["method"] = method;
  if (!url.empty())
    block["url"] = url;
  block["version"] = "HTTP/1.1";

  if (bk)
    block.insert(bk->begin(), bk->end());

  spdy::SpdyStreamId newid = id;

  if (!id) // 请求
    newid = GetNewStreamId();

  arch::BufferArray *ba = new arch::BufferArray();
  spdy::SpdyControlFrame* syn;
  if (id) {
    syn = spdy_->CreateSynReply(newid,
         (!length ? spdy::CONTROL_FLAG_FIN : spdy::CONTROL_FLAG_NONE),
         false, //compressed
         &block);
  } else {
    syn = spdy_->CreateSynStream(newid,
          0, // associated 
          0, // priority
          spdy::CONTROL_FLAG_NONE, //(!data ? spdy::CONTROL_FLAG_FIN : spdy::CONTROL_FLAG_NONE),
          false, // compressed 
          &block);
  }
  ba->Append(syn->data(), spdy::SpdyFrame::size() + syn->length());

  if (length || !id) {
    spdy::SpdyDataFrame* sdf = NULL;
    sdf = spdy_->CreateDataFrame(newid, 
                         data,
                         length,
                         //length < 8 ? spdy::DATA_FLAG_FIN : (spdy::SpdyDataFlags)(spdy::DATA_FLAG_FIN | spdy::DATA_FLAG_COMPRESSED)); // TODO: 先不压缩
                         spdy::DATA_FLAG_FIN);
    ba->Append(sdf->data(), spdy::SpdyFrame::size() + sdf->length());
  }
  bool b = DoWrite(newid, ba, async);
  { // TODO: test
    char file[20];
    sprintf(file, "send_%d.txt", newid);
    int f = open(file, O_RDWR | O_CREAT | O_LARGEFILE, S_IRUSR|S_IWUSR|S_IRGRP);
    if (f != -1) {
      pwrite(f, ba->ptr(), ba->size(), 0);
      close(f);
    }
  }
  if (b) {
    if (!id)
      send_stream_map_.insert(std::make_pair(newid, time(NULL))); 
    delete ba;
    return newid;
  } else {
    return 0;
  }
}

void SpdyConnector::HandleError(bool send, spdy::SpdyStreamId id, int error) {
  spdy_error_ = error;
  if (sink_)
    sink_->OnError(false, id, error, mod_);
}

void SpdyConnector::OnControl(const spdy::SpdyControlFrame* frame) {
  LOG(LS_INFO) << "OnControl:"
              << " type:" << frame->type()
              << " version:" << frame->version()
              << " length:" << frame->length()    
              << " flags:" << (int)frame->flags();

  if (spdy::SYN_STREAM == frame->type()) {
    spdy::SpdySynStreamControlFrame syn(frame->data(), false);
    if (recv_stream_map_.find(syn.stream_id()) != recv_stream_map_.end()) {
      HandleError(false, syn.stream_id(), kExistStreamId);
    } else {
      spdy::SpdyHeaderBlock block;
      bool parsed_headers = spdy_->ParseHeaderBlock(frame, &block);
      if (parsed_headers) { 
        recv_stream_map_.insert(std::make_pair(syn.stream_id(),StreamInfo(block))); 
      } else {
        HandleError(false, syn.stream_id(), kParseHeaderError);
      }
    }
  } else if (spdy::SYN_REPLY == frame->type()) {
    spdy::SpdySynReplyControlFrame syn(frame->data(), false);

    { // TODO: test
      char file[20];
      sprintf(file, "recv_reply_control_%d.txt", syn.stream_id());
      int f = open(file, O_RDWR | O_CREAT | O_LARGEFILE, S_IRUSR|S_IWUSR|S_IRGRP);
      if (f != -1) {
        pwrite(f, syn.data(), syn.size(), 0);
        close(f);
      }
    }
    if (send_stream_map_.find(syn.stream_id()) == send_stream_map_.end()) {
      HandleError(false, syn.stream_id(), kNoExistStreamId);
    } else {
      spdy::SpdyHeaderBlock block;
      bool parsed_headers = spdy_->ParseHeaderBlock(frame, &block);
      if (parsed_headers) { 
        // HandleError(false, syn.stream_id(), kParseHeaderError); // TODO: test
        recv_stream_map_.insert(std::make_pair(syn.stream_id(),StreamInfo(block))); 
      } else {
        HandleError(false, syn.stream_id(), kParseHeaderError);
      }
    }
  } else if (spdy::RST_STREAM == frame->type()) {

  } else if (spdy::GOAWAY == frame->type()) {

  }
}

void SpdyConnector::OnStreamFrameData(spdy::SpdyStreamId id, const char* data, size_t len) {
  LOG(LS_INFO) << "OnStreamFrameData:" 
            << "streamid:" << id
            << " buf:" << &data
            << " length:" << len;
  if (spdy_error_)
    return;

  { // TODO: test
    char file[20];
    sprintf(file, "recv_data_%d.txt", id);
    int f = open(file, O_RDWR | O_CREAT | O_LARGEFILE | O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP);
    if (f != -1) {
      write(f, data, len);
      close(f);
    }
  }
  std::map<spdy::SpdyStreamId, StreamInfo>::iterator it = recv_stream_map_.find(id);
  if (it == recv_stream_map_.end()) {
    HandleError(false, 0, kNoExistStreamId);
    //sink_->OnStream(syn.stream_id(), NULL, NULL, kNoExistStreamId);
  } else {
    if (data) {
      // char *buf = new data[len];
      // memcpy(buf, data, len);
      it->second.buffer->Append(data, len);
    } else { // finish this stream
      
      // 发送的请求的处理
      std::map<spdy::SpdyStreamId, time_t>::iterator send_it;
      send_it = send_stream_map_.find(id);
      if ( send_it != send_stream_map_.end()) { // send reply stream
        //delete send_it->second;
        send_stream_map_.erase(send_it);
      }

      // 接收到的远端流的处理
      if (it->second.buffer->size())
        sink_->OnStream(id, &it->second.block, it->second.buffer->ptr(), it->second.buffer->size());
      else 
        sink_->OnStream(id, &it->second.block, NULL, 0);
      
      // 清理工作
      delete it->second.buffer;
      recv_stream_map_.erase(it);
    }
  }
}

void SpdyConnector::HandleRead(const boost::system::error_code& err, size_t bytes_transferred) {
  if (err) {
    if (boost::asio::error::eof == err) {
      LOG(LS_INFO) << "Read Net Data Error: Connection is closed.";
    } else if (boost::asio::error::operation_aborted == err) {
      LOG(LS_INFO) << "Read Program normal exit.";
      return;
    } else {
      LOG(LS_WARNING) << "Read Net Data Error: "
                   << "reason: " << boost::system::system_error(err).what();
    }
    HandleError(false, 0, err.value());
  } else {
    last_time_ = time(NULL);
    size_t len = bytes_transferred;
    size_t pos = 0;
    while(len > 0) {
      size_t bytes = spdy_->ProcessInput(recv_buf_ + pos, len);
      if (!bytes) {
        HandleError(false, 0, kParseHeaderError);
        return;
      }
      bool b = spdy_->MessageFullyRead();
      len -= bytes;
      pos += bytes;
    }
  }
  DoRead();
}

/*
bool SpdyConnector::SyncReadFrame(spdy::SpdyStreamId id, char** buf, int *len) {
  if (!buf || !len)
    return false;

  size_t l;
  while(1) {
    boost::system::error_code ec;
    l = spdy_->BytesSafeToRead(); // TODO: 此函数不是很满意,改动
    if (l == 0)
      return false;
    else if (l > sizeof(recv_buf_))
      l = sizeof(recv_buf_);

    boost::asio::read(socket_, boost::asio::buffer(recv_buf_, l), boost::asio::transfer_all(), ec);
    //if (ec)
    //  return false;
    HandleRead(ec, l, false); //boost::system::error_code()
    if (spdy_error_)
      return false;

    if (!wait_for_streamid_) {
      std::map<spdy::SpdyStreamId, StreamInfo>::iterator it = recv_stream_map_.find(id);
      if (it == recv_stream_map_.end())
        return false;
      *len = it->second.buffer->size(); 
      *buf = it->second.buffer->detach_on_exit();

      delete it->second.buffer;
      recv_stream_map_.erase(it);
      return true;
    }
  }
  return false;
}
*/
void SpdyConnector::DoRead() {
  socket_.async_read_some(boost::asio::buffer(recv_buf_, sizeof(recv_buf_)),
                          boost::bind(&SpdyConnector::HandleRead, 
                            this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred)
                          );
}

void SpdyConnector::HandleWrite(spdy::SpdyStreamId id, const boost::system::error_code& err, size_t bytes_transferred) {
  if (err) {
    if (boost::asio::error::broken_pipe == err) {
      LOG(LS_INFO) << "Write net data error: Connection is closed.";
    } if (boost::asio::error::operation_aborted == err) {
      LOG(LS_INFO) << "Write program normal exit.";
      return;
    } else {
      LOG(LS_WARNING) << "Write Net Data Error: "
                   << "reason: " << boost::system::system_error(err).what();
    }
    HandleError(true, id, err.value());
    return;
  }
  last_time_ = time(NULL);
  // TODO: 取消超时
  // some stream finish
}

bool SpdyConnector::DoWrite(spdy::SpdyStreamId id, const arch::BufferArray *ba, bool async) {
  if (async)
    boost::asio::async_write(socket_,
                             boost::asio::buffer(ba->ptr(), ba->size()),
                             boost::bind(&SpdyConnector::HandleWrite,
                               this,
                               id,
                               boost::asio::placeholders::error,
                               boost::asio::placeholders::bytes_transferred)
                             ); 
  else
    return boost::asio::write(socket_,
                             boost::asio::buffer(ba->ptr(), ba->size()),
                             boost::asio::transfer_all()
                             ) != 0; 
  return true;

}
/*
bool SpdyConnector::DoWrite(spdy::SpdyStreamId id, const spdy::SpdyFrame * frame, bool async) {
  if (async)
    boost::asio::async_write(socket_,
                           boost::asio::buffer(frame->data(), spdy::SpdyFrame::size() + frame->length()),
                           boost::bind(&SpdyConnector::HandleWrite,
                             this,
                             id,
                             boost::asio::placeholders::error,
                             boost::asio::placeholders::bytes_transferred)
                           ); 
  else
    return boost::asio::write(socket_,
                             boost::asio::buffer(frame->data(), spdy::SpdyFrame::size() + frame->length()),
                             boost::asio::transfer_all()
                             ) != 0; 
  return true;
} 
*/
bool SpdyServer::Listen(const Address & address) {
  io_service_.reset(new boost::asio::io_service);
  // TODO: exception
  // TODO: address.first = ip
  int port = boost::lexical_cast<int>(address.second);
  acceptor_.reset(new tcp::acceptor(*io_service_, tcp::endpoint(tcp::v4(), port)));

  if (!sink_.get())
    return false;
  SpdyStreamSink * nw = sink_->OnNew(*io_service_);
  SpdyConnector* cont = nw->GetContext();
  acceptor_->async_accept(cont->socket(), 
                          boost::bind(&SpdyServer::HandleAccept, 
                            this,
                            boost::asio::placeholders::error, 
                            nw)
                          );
  return true;
}


void SpdyServer::Run() {
  // TODO: use thread pool and io_service pool
  io_service_->run();
  // TODO: delete some resource
}

SpdyServer::SpdyServer() {
}

void SpdyServer::Set(SpdyStreamSink *sink) {
  sink_.reset(sink); 
}

void SpdyServer::HandleAccept(const boost::system::error_code& error, SpdyStreamSink *nw) {
  if (!error) {
    SpdyConnector* cont = nw->GetContext();
    LOG(LS_INFO) << "New Client Connection: "
              << "local: " << cont->socket().local_endpoint().address().to_string()
              << "remote: " << cont->socket().remote_endpoint().address().to_string();
    //cont->Start(nw, kServerSendTimeout);
    connets_.push_back(nw);

    SpdyStreamSink * nw1 = sink_->OnNew(*io_service_);
    SpdyConnector* cont1 = nw1->GetContext();
    acceptor_->async_accept(cont1->socket(), 
                            boost::bind(&SpdyServer::HandleAccept, 
                            this,
                            boost::asio::placeholders::error, 
                            nw1)
                            );
  } else {
    LOG(LS_WARNING) << "Accept error: " << "reson: " << error.message();
  }
}

} // namespace
