#ifndef _ARCH_URL_PARSE_H__
#define _ARCH_URL_PARSE_H__
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include "url_parse.h"
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

inline bool IsValidFistChar(const char c) {
  if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9')
    return true;
  return false;
}

// no check empty string
inline bool IsVariation(const std::string &key, std::string *var) {
  bool b = key[0] == '{' && key[key.size() - 1] == '}';
  if (b && var) {
    *var = key.substr(1, key.size() - 2);
  }
  return b;
}

/* unit test
 std::string s1 = "/abc/efg/{xx}";
 std::string s2 = "abc/efg/{xx}{yyy}";
 std::string s3 = "abc/efg/{xx}{yyy}/efg";
 std::string s4 = "abc/efg/{xx}/efg";
 std::string s5 = "abc/efg/{xx}/efg//";
 */
// 分解path
// abc/{gid}/get/{uid} -> ["abc","{gid}","get","{uid}"]
bool ParsePath(const std::string &pp, std::vector<std::string> *v) {
  if (!v)
    return false;
  std::string key;
  std::string::size_type posk;
  std::string::size_type pos = 0;
  for (; pos < pp.size() - 1;) {
    posk = pp.find('/', pos);
    if (posk == std::string::npos)
      break;
    key = pp.substr(pos, posk - pos);
    if (!key.empty())
      v->push_back(key);
    pos = posk + 1;
  }
  if (pos < pp.size() - 1)
    v->push_back(pp.substr(pos));
  return true;
}

// 分解path,并取出所有变量
// abc/{gid}/get/{uid} -> ["abc/","{gid}","/get/","{uid}"],{"gid":"","uid",""}
bool ParseVariant(const std::string &pp, std::vector<std::string> *v,
                  std::map<std::string, std::string> *m) {
  if (!v)
    return false;
  std::string path;
  std::string var;
  std::string::size_type posl, posr;
  std::string::size_type pos = 0;
  for (; pos < pp.size() - 3;) {
    posl = pp.find('{', pos);
    if (posl == std::string::npos)
      break;
    posr = pp.find('}', posl);
    if (posr == std::string::npos)
      return false;

    path = pp.substr(pos, posl - pos);
    if (!path.empty())
      v->push_back(path);
    var = pp.substr(posl, posr - posl + 1);
    if (!var.empty())
      v->push_back(var);
    if (m && var.size() > 2)
      m->insert(std::make_pair(pp.substr(posl + 1, posr - posl - 1), ""));
    pos = posr + 1;
  }
  if (pos < pp.size() - 1)
    v->push_back(pp.substr(pos));
  return true;
}

// a=1&b=eee
bool ParseParam(const std::string &pa,
                std::map<std::string, std::string> *param) {
  if (!param)
    return false;
  std::string pp;
  if (pa[pa.size() - 1] != '&')
    pp = pa + "&";
  else
    pp = pa;
  std::string key, value;
  std::string::size_type pos0 = 0, posk;
  std::string::size_type pos = 0;
  for (; pos < pp.size() - 2; pos++) { // 至少有2个字符
    posk = pp.find('=', pos);
    if (posk == std::string::npos || posk == pp.size() - 1
        || pp[posk + 1] == '&') // 不能为空,不能最后字节,其后不能为&
      return false;
    key = pp.substr(pos, posk - pos);
    posk++;
    pos = pp.find('&', posk);
    if (pos == std::string::npos)
      return false;
    value = pp.substr(posk, pos - posk);
    param->insert(std::make_pair(key, value));
  }
  return true;
}

bool _ParseUrl(const std::string &url, std::string *host, int *port,
               std::string *path, std::map<std::string, std::string> *param,
               std::string *param_str, std::string *fullpath) {
  if (param)
    param->clear();
  if (port)
    *port = 80;
  if (path)
    *path = "";

  //过滤http://协议
  std::string::size_type pos1 = url.find("://");
  if (pos1 == std::string::npos)
    pos1 = 0;
  else
    pos1 += 3; // 3 == sizeof("://")

  // 检查主机
  if (url.size() == pos1 + 1) // like http://
    return false;
  if (!IsValidFistChar(url[pos1])) // like http://#
    return false;

  std::string::size_type pos2 = url.find_first_of(":/?", pos1);
  if (pos2 == std::string::npos) {
    if (host)
      *host = url.substr(pos1); // like http://abc
    return true;
  }

  // 获取主机
  if (host)
    *host = url.substr(pos1, pos2 - pos1);

  // 获取端口
  if (url[pos2] == ':') {
    std::string tmp;
    std::string::size_type pos0 = url.find_first_of("/?", pos2);
    if (pos0 == std::string::npos) {
      if (url.size() == pos2 + 1) { // like http://abc:
      } else { // like http://abc:88
        tmp = url.substr(pos2 + 1);
        if (port)
          *port = atoi(tmp.c_str());
      }
      return true;
    }
    pos2++;
    if (pos0 == pos2) { // like http://abc:/
      return true;
    }
    tmp = url.substr(pos2, pos0 - pos2);
    if (port)
      *port = atoi(tmp.c_str());
    pos2 = pos0;
  }

  // 全路径
  if (fullpath) {
    *fullpath = url.substr(pos2);
  }

  // 获取路径
  if (url.size() == pos2 + 1) { // like http://abc/ or http://abc?
    return true;
  }
  std::string::size_type pos3 = url.find('?', pos2);
  if (pos3 == std::string::npos) { // like http://abc/path/get
    if (path)
      *path = url.substr(pos2);
    return true;
  }
  if (pos3 == pos2) { // like http://abc?a=1
  }
  if (pos3 == pos2 + 1) { // like http://abc/?a=1
  } else {
    if (path)
      *path = url.substr(pos2, pos3 - pos2);
  }

  // 获取参数段
  pos3++;
  if (url.size() == pos3) // like http://abc/path?
    return true;

  std::string pa = url.substr(pos3);
  if (param_str)
    *param_str = pa;
  if (param)
    ParseParam(pa, param);
  return true;
}
// http://xoa.xiaonei.com:8188/path1/path2/get?id=1011&key=al38
// host need exist
bool ParseUrl(const std::string &url, std::string *host, int *port,
              std::string *path, std::map<std::string, std::string> *param) {
  return _ParseUrl(url, host, port, path, param, NULL, NULL);
}
bool ParseUrlSimple(const std::string &url, std::string *host, int *port,
                    std::string *fullpath) {
  return _ParseUrl(url, host, port, NULL, NULL, NULL, fullpath);
}

} // namespace
#endif // define
