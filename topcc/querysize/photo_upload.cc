#include "photo_upload.h"
#include "photo_head.h"
#include "base3/logging.h"
#include "arch_diff/site_xiaonei.h"

#include "memcache.h"
#include "util.h"
//#include "json/json.h"

namespace photo {

//using namespace cwf;

PhotoAction::PhotoAction() {
}

bool PhotoAction::Match(const std::string& url) const {
  return true;// 目前就一个页面,直接返回了 std::string::npos != url.find("eat");
}

HttpStatusCode PhotoAction::Process(Request* request, Response* response) {
  OutResponse resp;

  // 初始化resp
  resp.url_ = request->get("url");

  resp.code_ = 0;
  LOG(INFO) << "new query, url: " << resp.url_;
  if (resp.url_.empty()) {
    resp.code_ = 503;
    resp.msg_ = "缺少参数或参数错误, 请配置url参数";
    goto EXIT_OUT;
  }

  imagehandler_.ProcessImage(resp.url_, &resp);

EXIT_OUT:  // 输出
  std::string strOutput = resp.ToJson();
  response->WriteRaw(strOutput+'\n');

  return HC_OK;
}

inline char* itoa(char* strnum, int numb) {
  sprintf(strnum, "%d", numb);
  return strnum;
}

std::string OutResponse::ToJson() {
  char strnum[32];
  std::string json =  "Status: 200\r\nContent-Type: text/html; charset=utf-8\r\n\r\n{";
  json += "\"code\":"; json += itoa(strnum, code_);
  json += ",\n\"msg\":\""; json += msg_;
  json += "\",\n\"url\":\""; json += url_;
  json += "\",\n\"width\":"; json += itoa(strnum, width_);
  json += ",\n\"height\":"; json += itoa(strnum, height_);
    
  json += "\n]\n}\n";
  //return "{\"uploadid\":\"fu_200435454_12345671234\",\"code\":504,\"msg\":\"这是一个测试\"}";
  return json;
}

} // namespace 
