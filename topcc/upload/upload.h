#ifndef _CURL_UPLOAD_HEAD_
#define _CURL_UPLOAD_HEAD_

#include <string>
#include <vector>
#include <list>
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "upload/download.h"

namespace net {

typedef std::pair<std::string, int> Address;
#ifndef kPhotoDetectLog
#define kPhotoDetectLog false
#endif
//const bool kPhotoDetectLog = false;

class HtmlData {
public:
  HtmlData();
  HtmlData(const HtmlData& rhs);
  ~HtmlData();
  const std::vector<std::string>& head() const;
  void AppendHead(const std::string& text);
  void ClearHead();
  const std::vector<std::string>& body() const;
  void AppendBody(const std::string& text);
  void ClearBody();
  const char* data() const;
  size_t length() const;
  void AssignData(const char* content, size_t length);
private:
  HtmlData& operator=(const HtmlData& rhs);
private:
 std::vector<std::string> head_;
 std::vector<std::string> body_;
 char* data_;
 size_t length_;
};

class WebPost;

class ReportCoordHandler : public WebHandler {
 public:
  ReportCoordHandler(WebPost *wp);
  virtual bool OnSendData(const char* host, arch::BufferArray *ba);
  virtual void OnGetHead(std::vector<std::string> heads);
  virtual void OnGetResult(const char *buf, int size);
  virtual void OnError();
  std::string head_;
  std::string logid_;
 private:
  WebPost *webpost_;
};

class PhotoUploadHandler : public WebHandler {
 public:
  PhotoUploadHandler(WebPost *wp);
  virtual bool OnSendData(const char* host, arch::BufferArray *ba);
  virtual void OnGetHead(std::vector<std::string> heads);
  virtual void OnGetResult(const char *buf, int size);
  virtual void OnError();
 private:
  std::vector<std::string> head_;
  std::vector<std::string> body_;
  std::string logid_;
  WebPost *webpost_;
};

class UploadFactory : public WebHandlerFactory {
 public:
  UploadFactory(WebPost* wp);
  virtual WebHandler * Create();
 private:
  WebPost* wp_;
};

class ReportFactory : public WebHandlerFactory {
 public:
  ReportFactory(WebPost* wp);
  virtual WebHandler * Create();
 private:
  WebPost* wp_;
};

class WebPost {
 public:
  WebPost();
  bool Start(const std::vector<Address> &v);
  bool PostFile(const HtmlData &hd);
  void Run();
  void Close();

  void ReportCoord(const std::vector<std::string> &head, const std::vector<std::string> &body, const char *buf, int size, const std::string &logid);
  std::list<HtmlData> jobs_;
  std::list<std::pair<std::string, std::string> > report_jobs_;
  int now_jobs_;
 private:

  void Clear();
  bool RecheckIp(bool sync);
  bool RequeryIp(bool sync);

  void KeepTime(const boost::system::error_code &e);
  void HandleNewJob(HtmlData &hd);

  std::vector<tcp::endpoint> curl_;
  //std::map<tcp::endpoint, ValidConnect*> gvalid_; // 有效及忙标记

  mutable boost::mutex mutex_;
  bool will_exit_;
  time_t last_recheck_;
  //boost::condition queue_not_empty_;
  boost::asio::io_service ios_;
  ReportFactory rf_;
  UploadFactory uf_;
  Download upload_;
  Download report_;
  boost::shared_ptr<boost::thread> work_;
  boost::asio::deadline_timer t_;
  int max_jobs_;
  int keep_;
};

struct WebPool {
  bool Start(const std::vector<Address> &v);
  bool PostFile(const HtmlData &hd);
  void Close();
  std::vector<boost::shared_ptr<WebPost> > wp_;
  int max_works_;
  int current_;
};

} //namespacpe
#endif // define
