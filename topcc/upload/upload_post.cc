
#include "download.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "base3/logging.h"


typedef std::pair<std::string, int> Address;

bool GetFileBuffer(const char *file, char **buf, int* len) {
  bool b = true;
  int f = open(file, O_RDONLY);
  if (f < 0)
    return false;
  const int sz = 1024*1024*10;
  if (buf) {
    if (!len) {
      b = false;
    } else {
      *buf = new char [sz];
      ssize_t res = read(f, *buf, sz);
      if (res >= 0) {
        *len = res;
      } else {
        b = false;
      }
    }
  }
  close(f);
  return b;
}

void print_data(const char* buf, int size) {
  char *memstr = new char[size+1];
  memcpy(memstr, buf, size);
  memstr[size] = 0;
  puts(memstr);
  delete memstr;
}
using namespace net;

class WebPost;

std::string  FindValue(const std::string &str, const std::string &key) {

  size_t pos = str.find(key + "=");
  if (pos == std::string::npos)
    return "";
  size_t pos1 = str.find("\n", pos);
  if (pos1 == std::string::npos)
    return "";
  pos += key.size();
  pos += 1;
  std::string sub = str.substr(pos, pos1 - pos);
  return sub;
}

class PostHandler : public WebHandler {
 public:
  PostHandler(WebPost *wp): webget_(wp) {
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
  WebPost *webget_;
};

class GetFactory : public WebHandlerFactory {
 public:
  GetFactory(WebPost* wp) : wp_(wp) {}
  WebHandler * Create() {
    return new PostHandler(wp_);
  }
 private:
  WebPost* wp_;
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

class WebPost {
 public:
  WebPost()
: now_jobs_(0)
, will_exit_(false)
, last_recheck_(time(NULL))
, gf_(this)
, get_(ios_)
, t_(ios_, boost::posix_time::seconds(5))
, max_jobs_(1000)
, keep_(1) {
    //arch::LogToDir(".", arch::LS_VERBOSE, 20*3600);
  }

  bool Start(const std::vector<Address> &v) {
    get_.Start(&gf_);
    for (std::vector<Address>::const_iterator it = v.begin(); it != v.end(); ++it) {
      tcp::endpoint ep = Addr2Endpoint(*it);
      curl_.push_back(ep);
    }
    if (RecheckIp(true) == false)
      return false;
    t_.async_wait(boost::bind(&WebPost::KeepTime, this, boost::asio::placeholders::error));

    work_.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &ios_)));
    return true;
  }
  bool PostFile(const std::string &header) {
   { 
      LOG(INFO) << "<> new photo arriver, list size:" << now_jobs_;
      ++now_jobs_;
      ios_.post(boost::bind(&WebPost::HandleNewJob, this, header));
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
    //DLOG(INFO) << ("<>KeepTime");
    if (keep_++ % 30 == 0) { // 150秒重新注入IP
    }
    t_.expires_at(t_.expires_at() + boost::posix_time::seconds(5));
    t_.async_wait(boost::bind(&WebPost::KeepTime, this, boost::asio::placeholders::error));
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

  bool PostHandler::OnSendData(const char* ip, arch::BufferArray *ba) {
    if (webget_->jobs_.empty()) {
      return false;
    }
    std::string &job = webget_->jobs_.front();
    webget_->jobs_.pop_front();
    char *buf = NULL;
    int len = 0;
    std::string path = "/data/" + job;
    bool b= GetFileBuffer((path + ".head").c_str(), &buf, &len);
    std::string str(buf, len);
    delete buf;
    std::string type = FindValue(str, "CONTENT_TYPE");
    std::string uri = FindValue(str, "REQUEST_URI");
    std::string length = FindValue(str, "CONTENT_LENGTH");
    std::string host = FindValue(str, "HTTP_HOST");

    char pre[12048];
    sprintf(pre,
      "POST %s HTTP/1.1\r\n"
      "Host: %s\r\n"
      "Connection: keep-alive\r\n"
      "Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n"
      "Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n"
      "Accept-Encoding: gzip,deflate,sdch\r\n"
      "Accept-Language: zh-CN,zh;q=0.8\r\n"
      "Cache-Control: max-age=0\r\n"
      "Connection: keep-alive\r\n"
      "If-Modified-Since: Fri, 25 Feb 2011 05:57:05 GMT\r\n"
      "Content-Length: %s\r\n"
      "Content-Type: %s\r\n"
      "User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/534.24 (KHTML, like Gecko) Chrome/11.0.696.3 Safari/534.24\r\n"
      "\r\n",
      uri.c_str(),
      host.c_str(),
      length.c_str(),
      type.c_str()
    );
    ba->Append(pre, strlen(pre));
    b= GetFileBuffer((path + ".form").c_str(), &buf, &len);
    ba->Append(buf, len);
    delete buf;
    return true;
  }

int main() {
  WebPost wg;
  std::vector<Address> v;
  v.push_back(std::make_pair("10.7.16.147", 80));
  wg.Start(v);
  //char str[100];
  char *ls[] = {
"20110520-144052-6181-3"
  };
  for (int i = 0; i < sizeof(ls) / sizeof(char*); i++) {
    wg.PostFile(ls[i]);
    sleep(1);
  }
  wg.Close();
  sleep(10);
  return 0;
}
