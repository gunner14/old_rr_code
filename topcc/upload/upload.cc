#include "upload.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <boost/thread/thread.hpp>
//#include "base/logging.h"
#include "base3/logging.h"


namespace net {
#ifndef kNewUpload
#define kHost  "photo.renren.com"
#define kUrl "/faceDetect/add"
#else
#define kHost  "upload.renren.com"
#define kUrl "/upload/faceDetect/add"
#endif

struct UseData {
  std::string hostid; //(用户id)
  std::string t; //用户t票
  std::string albumid; //相册id
  std::string ishead; //是否是头像
  std::string large_url; //largeUrl路径
  std::string coord; //对应的坐标
};

HtmlData::HtmlData()
  : data_(NULL), length_(0)
{
}
HtmlData::HtmlData(const HtmlData& rhs)
  : head_(rhs.head_), body_(rhs.body_), data_(NULL), length_(rhs.length_)
{
  if (length_ > 0)
  {
    data_ = new char[length_];
    ::memcpy((void*)data_, (void*)rhs.data_, length_);
  }
}
HtmlData::~HtmlData()
{
  delete []data_;
}
HtmlData& HtmlData::operator=(const HtmlData& rhs)
{
  return *this;
}
const std::vector<std::string>& HtmlData::head() const
{
  return head_;
}
void HtmlData::AppendHead(const std::string& text)
{
  head_.push_back(text);
}
void HtmlData::ClearHead()
{
  head_.clear();
}
const std::vector<std::string>& HtmlData::body() const
{
  return body_;
}
void HtmlData::AppendBody(const std::string& text)
{
  body_.push_back(text);
}
void HtmlData::ClearBody()
{
  body_.clear();
}
const char* HtmlData::data() const
{
  return data_;
}
size_t HtmlData::length() const
{
  return length_;
}
void HtmlData::AssignData(const char* content, size_t length)
{
  if (!content || !length) return;
  delete []data_;
  length_ = length;
  data_ = new char[length_];
  ::memcpy(data_, content, length_);
}
WebPost::WebPost()
: now_jobs_(0)
, will_exit_(false)
, last_recheck_(time(NULL))
, rf_(this)
, uf_(this)
, upload_(ios_)
, report_(ios_)
, t_(ios_, boost::posix_time::seconds(5))
, max_jobs_(5)
, keep_(1)
{
#ifdef _DEBUG
#else
#endif
}

void WebPost::Clear() {
  //upload_.Clear();
  jobs_.clear();
}

tcp::endpoint Addr2Endpoint(const Address &url) {
  boost::system::error_code ec = boost::asio::error::host_not_found; // 连接/读写时的错误码
  boost::asio::ip::address add;
  add = boost::asio::ip::address::from_string(url.first, ec);
  if (ec)
    return tcp::endpoint();
  //std::string test1 = add.to_string(); // TODO: test
  return tcp::endpoint(add, url.second);
}

void WebPost::KeepTime(const boost::system::error_code &e) {
  // TODO: 应该挪到download类中,检查某些连接的超时,不过目前不需要
  if (will_exit_ || e)
    return ;
  //DLOG_IF(INFO) << ("<>KeepTime");
  if (keep_++ % 60 == 0) { // 300秒重新注入IP
    if (RecheckIp(false) == false)
      ;
    if (RequeryIp(false) == false)
      ;
  }
  t_.expires_at(t_.expires_at() + boost::posix_time::seconds(5));
  t_.async_wait(boost::bind(&WebPost::KeepTime, this, boost::asio::placeholders::error));
}

bool WebPost::Start(const std::vector<Address> &v) {
  Clear();
  upload_.Start(&uf_);
  report_.Start(&rf_);
  for (std::vector<Address>::const_iterator it = v.begin(); it != v.end(); ++it) {
    tcp::endpoint ep = Addr2Endpoint(*it);
    curl_.push_back(ep);
  }
  if (RecheckIp(true) == false)
    return false;
  if (RequeryIp(true) == false)
    return false;
  t_.async_wait(boost::bind(&WebPost::KeepTime, this, boost::asio::placeholders::error));

  work_.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &ios_)));
  return true;
}

// 将ip重新回笼
bool WebPost::RecheckIp(bool sync) {
  if(sync)
    return upload_.Register(curl_);
  ios_.post(boost::bind(&Download::BoostRegister, &upload_, curl_));
  return true;
}

UploadFactory::UploadFactory(WebPost* wp) : wp_(wp) {}
WebHandler * UploadFactory::Create() {
  return new PhotoUploadHandler(wp_);
}

ReportFactory::ReportFactory(WebPost* wp) : wp_(wp) {}
WebHandler * ReportFactory::Create() {
  return new ReportCoordHandler(wp_);
}

bool WebPost::RequeryIp(bool sync) {
  std::vector<tcp::endpoint> curl;
  tcp::resolver res(ios_);
  tcp::resolver::query query(kHost,"80");
  tcp::resolver::iterator it = res.resolve(query);
  tcp::resolver::iterator it_end;
  for (; it != it_end; ++it) {
    curl.push_back(*it);
  }
  LOG_IF(WARNING, kPhotoDetectLog) << "<> requery ip: " << kHost << " ip[0]:" << curl.front();
  if (sync)
    return report_.Register(curl);
  ios_.post(boost::bind(&Download::BoostRegister, &report_, curl));
  return true;
}

void MakeReportString(const char* host, const std::string &head, arch::BufferArray *ba) {
  if(!ba)
    return;
  //char pre[1024];
  //sprintf(pre, "POST %s HTTP/1.1\r\n"
               //"Host: %s\r\n", kUrl, host);
  //ba->Append(pre, strlen(pre));
}

void MakePostString(const char* host, const char*buf, int sz, arch::BufferArray *ba, const std::string &logid) {
  if(!ba)
    return;
  //const char* token ="----WebKitFormBoundary529FYphE2Uqppzzb";
  char pre[2048];
  sprintf(pre,
    "POST /detect HTTP/1.1\r\n"
    "Host: %s\r\n"  //1
    "Connection: keep-alive\r\n"
    "Content-Length: %d\r\n"  //3
    "Cache-Control: max-age=0\r\n"
    "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary529FYphE2Uqppzzb\r\n"
    "Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/\x2a;q=0.5\r\n"
    "Image-Path: %s\r\n" // 5
    //"Accept-Encoding: gzip,deflate,sdch\r\n"
    //"Accept-Language: zh-CN,zh;q=0.8\r\n"
    "Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n"
    "\r\n"
    "------WebKitFormBoundary529FYphE2Uqppzzb\r\n"
    "Content-Disposition: form-data; name=\"name_of_files\"; filename=\"test.jpg\"\r\n"
    "Content-Type: image/jpeg\r\n"
    "\r\n",
    host,
    191+sz,
    logid.c_str());
  ba->Append(pre, strlen(pre));
  ba->Append(buf, sz);
  const char end[] = "\r\n------WebKitFormBoundary529FYphE2Uqppzzb--\r\n";
  ba->Append(end, strlen(end));
  
  //int f = open("a.txt", O_RDWR | O_CREAT | O_LARGEFILE, S_IRUSR|S_IWUSR|S_IRGRP);
  //if (f != -1) {
    //pwrite(f, ba->ptr(), ba->size(), 0);
    //close(f);
  //}
}

bool WebPost::PostFile(const HtmlData &hd) {
  ios_.post(boost::bind(&WebPost::HandleNewJob, this, hd));
  return true;
}

//void WebPost::HandleNewRegister(const std::vector<tcp::endpoint> &curl) {
//  upload_.BoostRegister();
//}

void WebPost::HandleNewJob(HtmlData &hd) {
  std::string usrinfo;
  
  jobs_.push_back(hd);
  const std::vector<std::string>& hd_body = hd.body();
  for (size_t i = 2; i < hd_body.size(); i++) {
    usrinfo += hd_body[i];
    usrinfo += "|";
  }
  if (now_jobs_ >= max_jobs_) {// no need lock
    const HtmlData &old = jobs_.front();
    std::string oldinfo;
    const std::vector<std::string>& old_hd_body = old.body();
    for (size_t i = 2; i < old_hd_body.size(); i++) {
      oldinfo += old_hd_body[i];
      oldinfo += "|";
    }

    jobs_.pop_front();

    LOG_IF(WARNING, kPhotoDetectLog) << "<> new photo arriver, but over load,info: " << usrinfo;
    LOG_IF(WARNING, kPhotoDetectLog) << "<> remote queue front,info: " << oldinfo;
    return ;
  } else {
    LOG_IF(INFO, kPhotoDetectLog) << "<> new photo arriver, list size: " << now_jobs_ << " info: " << usrinfo;
    ++now_jobs_;
  }

  // boost::mutex::scoped_lock alock(mutex_);
  /*
  for (std::vector<std::string>::iterator it = hd.head.begin(); it != hd.head.end(); ++it) {
    DLOG_IF(INFO) << *it;
  }
  for (std::vector<std::string>::iterator it = hd.body.begin(); it != hd.body.end(); ++it) {
    DLOG_IF(INFO) << *it;
  }
  */
  upload_.BoostJob();
}

void WebPost::ReportCoord(const std::vector<std::string> &head, const std::vector<std::string> &body, const char* buf, int size, const std::string &logid) {
  std::string headstr;
  headstr = "POST " kUrl " HTTP/1.1\r\n";
  // TODO:test headstr = "GET /faceDetect/add?albumId=216301843&largeUrl=fmn037/20100810/1005/p_large_nnA9_1357000011bc2d14.jpg&coord=sdfsdfsd HTTP/1.1\r\n";
  headstr += "Host: " kHost "\r\n";
  //headstr += "Content-Type: text/html; charset=utf-8\r\n";
  headstr += "Connection: keep-alive\r\n";
  headstr += "Content-Type: application/x-www-form-urlencoded\r\n";
  headstr += "Accept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n";
  headstr += "Accept-Language: zh-CN,zh;q=0.8\r\n";
  headstr += "Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3\r\n";
  for (std::vector<std::string>::const_iterator it = head.begin(); it != head.end(); ++it) {
    headstr += *it;
    headstr += "\r\n";
  }
  
  char lenstr[128];
  std::string bodystr;
  for (std::vector<std::string>::const_iterator it = body.begin(); it != body.end(); ++it) {
    bodystr += *it;
    bodystr += "&";
  }
  bodystr += "coord=";
  bodystr.append(buf, size);
  sprintf(lenstr, "Content-Length: %d\r\n\r\n", (int)bodystr.size());
  report_jobs_.push_back(std::make_pair(headstr+lenstr+bodystr, logid));

  char logbuf[1024] = {0};
  if (size < 1024) {
    memcpy(logbuf, buf, size);
    logbuf[size] = 0;
  }
  LOG_IF(INFO, kPhotoDetectLog) << ("<> Send Coord: size:") << report_jobs_.size() << "info: " << logid << " " << logbuf;
  DLOG_IF(INFO, kPhotoDetectLog) << headstr << lenstr << bodystr;
  report_.BoostJob();
}

void WebPost::Close() {
  will_exit_ = true;
  upload_.Close();
  report_.Close();
  ios_.stop(); 
  work_->join();
  upload_.Clear();
  report_.Clear();
  Clear();
}

ReportCoordHandler::ReportCoordHandler(WebPost *wp) : webpost_(wp) {
}

void print_data(const char* buf, int size) {
  char *memstr = new char[size+1];
  memcpy(memstr, buf, size);
  memstr[size] = 0;
  puts(memstr);
  delete memstr;
}

bool ReportCoordHandler::OnSendData(const char* host, arch::BufferArray *ba) {
  if (webpost_->report_jobs_.empty()) {
    DLOG_IF(INFO, kPhotoDetectLog) << ("<> ReportCoord/SendData no job"); // TODO: test
    return false;
  }
  std::string head = webpost_->report_jobs_.front().first;
  logid_ = webpost_->report_jobs_.front().second;
  LOG_IF(INFO, kPhotoDetectLog) << ("<> ReportCoord/SendData: ") << logid_;

  webpost_->report_jobs_.pop_front();
  //MakeReportString("photo.renren.com", head, ba);
  ba->Append(head.c_str(), head.size());
  //print_data(ba->ptr(), ba->size());
  return true;
}

void ReportCoordHandler::OnGetHead(std::vector<std::string> heads) {
  DLOG_IF(INFO, kPhotoDetectLog) << ("<> ReportCoord/GetHead"); // TODO: test
}

void ReportCoordHandler::OnGetResult(const char *buf, int size) {
  LOG_IF(INFO, kPhotoDetectLog) << ("<> ReportCoord/GetResult: ") << logid_;
  //print_data(buf, size);
}

void ReportCoordHandler::OnError() {
  LOG_IF(WARNING, kPhotoDetectLog) << ("<> ReportCoord/Error") << logid_;
}

PhotoUploadHandler::PhotoUploadHandler(WebPost *wp) : webpost_(wp) {
}

bool PhotoUploadHandler::OnSendData(const char* host, arch::BufferArray *ba) {
  if (webpost_->jobs_.empty()) {
    DLOG_IF(INFO, kPhotoDetectLog) << ("<> PhotoUpload/SendData no jobs"); // TODO: test
    return false;
  }
  
  const HtmlData &job = webpost_->jobs_.front();
  logid_.clear();
  const std::vector<std::string>& hd_body = job.body();
  for (size_t i = 2; i < hd_body.size(); i++) {
    logid_ += hd_body[i];
    logid_ += "|";
  }
  LOG_IF(INFO, kPhotoDetectLog) << ("<> PhotoUpload/SendData: ") << logid_;
  head_ = job.head();
  body_ = job.body();
  MakePostString(host, job.data(), job.length(), ba, logid_);
  webpost_->jobs_.pop_front();
  --webpost_->now_jobs_;
  return true;
}

void PhotoUploadHandler::OnGetHead(std::vector<std::string> heads) {
  DLOG_IF(INFO, kPhotoDetectLog) << ("<> PhotoUpload/GetHead"); // TODO: test
  //std::vector<std::string> aa = head_; // TODO: test
}

void PhotoUploadHandler::OnError() {
  LOG_IF(WARNING, kPhotoDetectLog) << ("<> PhotoUpload/Error") << logid_; // TODO: test
}

void PhotoUploadHandler::OnGetResult(const char *buf, int size) {
  if (size == 0 || strcmp(buf, "[[]]") == 0) {// 不提交空的照片
    LOG_IF(INFO, kPhotoDetectLog) << ("<> Not Detect Coord: ") << logid_;
    return;
  }
  webpost_->ReportCoord(head_, body_, buf, size, logid_);
}

bool WebPool::Start(const std::vector<Address> &v) {
  wp_.clear();
  current_ = 0;
  max_works_ = 5;

  for (int i = 0; i < max_works_; ++i) {
    boost::shared_ptr<WebPost> wp(new WebPost);
    if (wp->Start(v) == false)
      return false;;
    wp_.push_back(wp);
  }
  return true;
}

bool WebPool::PostFile(const HtmlData &hd) {
  current_++;
  if (wp_.empty())
    return false;
  wp_[current_ % max_works_]->PostFile(hd);
  return true;
}

void WebPool::Close() {
  std::vector<boost::shared_ptr<WebPost> >::iterator it = wp_.begin();
  for (; it != wp_.end(); ++it) {
    (*it)->Close();
  }
}

} // namespace

