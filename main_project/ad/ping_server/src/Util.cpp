#include "Util.h"

using namespace std;
using namespace xce::ad;

/*size_t xce::ad::WriteCallBack(void *ptr, size_t size, size_t nmemb, void *data) {
  string *buff = (string *) data;
  buff->append((char *) ptr, size * nmemb);
  return size * nmemb;
}

bool xce::ad::GetHttpResult(const string& url, string& result){
  MCE_DEBUG("GetHttpResult --> " << url);
  CURL* handle = NULL;
  try{
    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallBack);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 1000);//连接服务器和发送请求的超时设置,单位是毫秒
    //curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT_MS, 2000);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &result);
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());//Get请求
    CURLcode ret = curl_easy_perform(handle);
    curl_easy_cleanup(handle);

    if ( 0 < ret ){
      result.clear();
      MCE_WARN("Load Url Error! " << url << " ErrorType: " << ret);
      return false;
    }
  } catch (std::exception e){
    MCE_WARN("Http Request Error!");
    curl_easy_cleanup(handle);
    return false;
  }
  return true;
}
*/
void xce::ad::SendPhoneMsg(const vector<string>& phones, const string& msg){
  vector<string>::const_iterator it = phones.begin();
  for (; it != phones.end(); ++it) { 
    ostringstream cmd;
    cmd << "wget -q -O /dev/null \"http://10.22.198.81:2000/receiver?number=" << *it << "&message=" << msg << "\"";
    system(cmd.str().c_str());
    MCE_INFO("SendPhoneMsg:: mobile = " << *it << ", msg = " << msg);
  }
}
