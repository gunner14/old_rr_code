
#include "download.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "base/logging.h"


typedef std::pair<std::string, int> Address;

void print_data(const char* buf, int size) {
  char *memstr = new char[size+1];
  memcpy(memstr, buf, size);
  memstr[size] = 0;
  puts(memstr);
  delete memstr;
}
using namespace net;

class WebGet;
class GetHandler : public WebHandler {
 public:
  GetHandler(WebGet *wp): webget_(wp) {
  }

  virtual bool OnSendData(const char* host, arch::BufferArray *ba);

  virtual void OnGetHead(std::vector<std::string> heads) {
  }

  virtual void OnGetResult(const char *buf, int size) {
    std::cout << "have a result:" << size << std::endl;
  }
  virtual void OnError() {
    std::cout << "this is a error" << std::endl;
  }
 private:
  std::vector<std::string> head_;
  std::vector<std::string> body_;
  WebGet *webget_;
};

class GetFactory : public WebHandlerFactory {
 public:
  GetFactory(WebGet* wp) : wp_(wp) {}
  WebHandler * Create() {
    return new GetHandler(wp_);
  }
 private:
  WebGet* wp_;
};

tcp::endpoint Addr2Endpoint(const Address &url) {
  boost::system::error_code ec = boost::asio::error::host_not_found; // 连接/读写时的错误码
  boost::asio::ip::address add;
  add = boost::asio::ip::address::from_string(url.first, ec);
  if (ec)
    return tcp::endpoint();
  //std::string test1 = add.to_string(); // TODO: test
  return tcp::endpoint(add, url.second);
}

class WebGet {
 public:
  WebGet()
: now_jobs_(0)
, will_exit_(false)
, last_recheck_(time(NULL))
, gf_(this)
, get_(ios_)
, t_(ios_, boost::posix_time::seconds(5))
, max_jobs_(1000)
, keep_(1) {
    arch::LogToDir(".", arch::LS_VERBOSE, 20*3600);
  }

  bool Start(const std::vector<Address> &v) {
    get_.Start(&gf_);
    for (std::vector<Address>::const_iterator it = v.begin(); it != v.end(); ++it) {
      tcp::endpoint ep = Addr2Endpoint(*it);
      curl_.push_back(ep);
    }
    if (RecheckIp(true) == false)
      return false;
    t_.async_wait(boost::bind(&WebGet::KeepTime, this, boost::asio::placeholders::error));

    work_.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &ios_)));
    return true;
  }
  bool GetFile(const std::string &header) {
   { 
      LOG(LS_INFO) << "<> new photo arriver, list size:" << now_jobs_;
      ++now_jobs_;
      ios_.post(boost::bind(&WebGet::HandleNewJob, this, header));
    }
    return true;
  };

  void Close() {
    will_exit_ = true;
    get_.Close();
    ios_.stop(); 
    work_->join();
  }

  std::list<std::string> jobs_;
  int now_jobs_;
 private:

  void KeepTime(const boost::system::error_code &e) {
    if (will_exit_ || e)
      return ;
    LOG(LS_VERBOSE) << ("<>KeepTime");
    if (keep_++ % 30 == 0) { // 150秒重新注入IP
    }
    t_.expires_at(t_.expires_at() + boost::posix_time::seconds(5));
    t_.async_wait(boost::bind(&WebGet::KeepTime, this, boost::asio::placeholders::error));
  }
  void HandleNewJob(const std::string &header) {
    jobs_.push_back(header);
    get_.BoostJob();
  };

  bool RecheckIp(bool sync) {
    if(sync)
      return get_.Register(curl_);
    ios_.post(boost::bind(&Download::BoostRegister, &get_, curl_));
    return true;
  }
  std::vector<tcp::endpoint> curl_;

  mutable boost::mutex mutex_;
  bool will_exit_;
  time_t last_recheck_;
  //boost::condition queue_not_empty_;
  boost::asio::io_service ios_;
  GetFactory gf_;
  Download get_;
  boost::shared_ptr<boost::thread> work_;
  boost::asio::deadline_timer t_;
  int max_jobs_;
  int keep_;
};

  bool GetHandler::OnSendData(const char* host, arch::BufferArray *ba) {
    if (webget_->jobs_.empty()) {
      return false;
    }
    std::string &job = webget_->jobs_.front();
    webget_->jobs_.pop_front();

    std::string addr = 
    "GET / HTTP/1.1\r\n"
    "Accept:application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n"
    "Accept-Charset:GBK,utf-8;q=0.7,*;q=0.3\r\n"
    "Accept-Encoding:gzip,deflate,sdch\r\n"
    "Accept-Language:zh-CN,zh;q=0.8\r\n"
    "Cache-Control:max-age=0\r\n"
    "Connection:keep-alive\r\n"
    "Host:10.2.18.202\r\n"
    "If-Modified-Since:Fri, 25 Feb 2011 05:57:05 GMT\r\n"
    "User-Agent:Mozilla/5.0 (Windows NT 5.1) AppleWebKit/534.24 (KHTML, like Gecko) Chrome/11.0.696.3 Safari/534.24\r\n";
    addr += job;
    addr += "\r\n\r\n";
    ba->Append(addr.c_str(), addr.size());
    return true;
  }

int main() {
  WebGet wg;
  std::vector<Address> v;
  v.push_back(std::make_pair("10.22.206.180", 80));
  wg.Start(v);
  char str[100];
  for (int i = 0; i < 100000000; i++) {
    sprintf(str, "MyCount: %d", i);
    wg.GetFile(str);
    sleep(1);
  }
  return 0;
}
