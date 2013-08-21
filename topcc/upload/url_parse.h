#ifndef _ARCH_URL_PARSE_H__
#define _ARCH_URL_PARSE_H__
#include <stdlib.h>
#include <string>
#include <map>

namespace arch {
/*
 std::string host, path;
 int port;
 std::map<std::string, std::string> param;
 bool b = ParseUrl("http://xoa.xiaonei.com:8188/path1/path2/get?id=1011&key=al38", &host, &port, &path, &param);

 b = ParseUrl("http://xoa.xiaonei.com:8188", &host, &port, &path, &param);
 b = ParseUrl("http://xoa.xiaonei.com:8188/?", &host, &port, &path, &param);
 b = ParseUrl("http://xoa.xiaonei.com:8188?id=1011&key=al38", &host, &port, &path, &param);
 b = ParseUrl("http://xoa.xiaonei.com:8188/?id=1011&key=al38", &host, &port, &path, &param);
 b = ParseUrl("http://xoa.xiaonei.com", &host, &port, &path, &param);
 b = ParseUrl("http://xoa.xiaonei.com/", &host, &port, &path, &param);
 b = ParseUrl("http://xoa.xiaonei.com/?", &host, &port, &path, &param);
 b = ParseUrl("http://xoa.xiaonei.com/path1/path2/get?id=1011&key=al38", &host, &port, &path, &param);
 b = ParseUrl("http://xoa.xiaonei.com?id=1011&key=al38", &host, &port, &path, &param);
 b = ParseUrl("http://xoa.xiaonei.com/?id=1011&key=al38", &host, &port, &path, &param);
 */

inline bool IsValidFistChar(const char c);
// no check empty string
inline bool IsVariation(const std::string &key, std::string *var);
/* unit test
 std::string s1 = "/abc/efg/{xx}";
 std::string s2 = "abc/efg/{xx}{yyy}";
 std::string s3 = "abc/efg/{xx}{yyy}/efg";
 std::string s4 = "abc/efg/{xx}/efg";
 std::string s5 = "abc/efg/{xx}/efg//";
 */
// 分解path
// abc/{gid}/get/{uid} -> ["abc","{gid}","get","{uid}"]
bool ParsePath(const std::string &pp, std::vector<std::string> *v);
// 分解path,并取出所有变量
// abc/{gid}/get/{uid} -> ["abc/","{gid}","/get/","{uid}"],{"gid":"","uid",""}
bool ParseVariant(const std::string &pp, std::vector<std::string> *v,
                  std::map<std::string, std::string> *m);
// a=1&b=eee
bool ParseParam(const std::string &pa,
                std::map<std::string, std::string> *param);

bool _ParseUrl(const std::string &url, std::string *host, int *port,
               std::string *path, std::map<std::string, std::string> *param,
               std::string *param_str, std::string *fullpath);

// http://xoa.xiaonei.com:8188/path1/path2/get?id=1011&key=al38
// host need exist
bool ParseUrl(const std::string &url, std::string *host, int *port,
              std::string *path, std::map<std::string, std::string> *param);

bool ParseUrlSimple(const std::string &url, std::string *host, int *port,
                    std::string *fullpath);
class UrlParse {
public:
  UrlParse(const std::string &url) {
    SetUrl(url);
  }

  bool SetUrl(const std::string &url) {
    is_valid_ = ParseUrl(url, &host_, &port_, &path_, &param_map_);
    return is_valid_;
  }

  const std::string &path() {
    return path_;
  }

  const std::string &url() {
    return url_;
  }

  bool valid() {
    return is_valid_;
  }

  const std::string & host() {
    return host_;
  }

  int port() {
    return port_;
  }

  const std::map<std::string, std::string> & param() {
    return param_map_;
  }

private:
  bool is_valid_;
  std::map<std::string, std::string> param_map_;
  std::string path_;
  std::string host_;
  int port_;
  std::string protocol_;
  std::string url_;

};

} // namespace
#endif // define
