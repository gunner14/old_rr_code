#include <stdio.h>
#include "base/stringencode.h" //应该没必要
#include "net/spdy_method.h"
#include "net/url_parse.h"

namespace xoa {
  /*
  本类不处理header中的url/Host/versin三个字段

  */
Method::Method(const std::string &ms, const std::string &path)
: method_style_(ms)
, path_(path)
, mod_(0) {
  heads_.insert(std::make_pair("Accept-Type","text/plain,application/serializable,application/protoc-buff,application/json"));
  heads_.insert(std::make_pair("Accept-Charset", "utf-8"));
  if (ms == "POST" || ms == "PUT") {
    //heads_.insert(std::make_pair("Content-Length", "0")); // 实际上不需要这个,为了兼容暂时加上
    heads_.insert(std::make_pair("Content-Type", "application/x-www-form-urlencoded;charset=UTF-8"));
    //heads_.insert(std::make_pair("Content-Type", "text/html;charset=UTF-8"));
  } else {
  }
  heads_.insert(std::make_pair("User-Agent", "xoa-c-client0.2"));
  heads_.insert(std::make_pair("method", ms));
  bool b = arch::ParseUrl(path, &host_, NULL, &path_, NULL);
  b = arch::ParseVariant(path_, &path_tok_, &path_var_);
}

const std::string& Method::host() const {
  return host_;
}

int Method::mod() const {
  return mod_;
}

std::string Method::method() const {
  if (method_style_ == "PUT")
    return "POST";
  return method_style_;
}

// newpath 必须非空
bool Method::GenPath(std::string *newpath) const {
  if (path_var_.empty()) {
     *newpath = path_;
  } else {
    std::vector<std::string>::const_iterator it = path_tok_.begin();
    std::string var;
    for (; it != path_tok_.end(); ++it) {
      if (arch::IsVariation(*it, &var)) {
        std::map<std::string, std::string>::const_iterator mit = path_var_.find(var);
        if (mit == path_var_.end())
          return false;
        *newpath += mit->second;
      } else {
        *newpath += *it;
      }
    }
  }
  return true;
}

std::string Method::url() const { // path + params or only path
  std::string pt;
  GenPath(&pt);
  if (method_style_ == "GET" || method_style_ == "DELETE") {
    return pt + "?" + params_;
  } 
  if (!content_.empty()) {
    if (method_style_ == "PUT")
      return pt + "?_method=put&" + params_;
    return pt + "?" + params_;
  }

  if (method_style_ == "PUT")
    return pt + "?_method=put"; // TODO: 为兼容tomcat服务器
  return pt;
}

const std::string& Method::content() const { // params or content_ 
  if (content_.empty()) {
    if (method_style_ == "POST" || method_style_ == "PUT")
      return params_;
    else
      return content_; // ""
  } else {
    return content_;
  }
}

const std::map<std::string, std::string>& Method::heads() const {
  if (method_style_ == "POST" || method_style_ == "PUT") {
    char str[20];
    const std::string &c = content();
    sprintf(str, "%d", c.size());
    std::map<std::string, std::string>::iterator it = heads_.find("Content-Length");
    if (it != heads_.end())
      heads_.erase(it);
    heads_.insert(std::make_pair("Content-Length", str));
  }
  //ReplaceHeader("Content-Length", str);
  return heads_;
}

Method Method::Get(const std::string &path) {
  return Method("GET", path);
}

Method Method::Put(const std::string &path) {
  return Method("PUT", path);
}

Method Method::Post(const std::string &path) {
  return Method("POST", path);
}

Method Method::Delete(const std::string &path) {
  return Method("DELETE", path);
}

void Method::SetAcceptType(const std::string &type) {
  ReplaceHeader("Accept-Type", type);
}

void Method::SetContent(const std::string &type, const std::string &content) {
  ReplaceHeader("Content-Type", type);
  content_ = content;
}

void Method::SetParam(const std::string &key, const std::string &value) {
  std::map<std::string, std::string>::iterator it = path_var_.find(key);
  if (it != path_var_.end()) {
    it->second = value;
    return;
  }
  //params_.insert(std::make_pair(key, value));
  if (params_.empty()) {
    params_ += key;
  } else {
    params_ += '&';
    params_ += key;
  }
  params_ += '=';
  params_ += value;

}

void Method::SetHeader(const std::string &key, const std::string &value) {
  ReplaceHeader(key, value);
}

void Method::ReplaceHeader(const std::string &key, const std::string &value) {
  std::map<std::string, std::string>::iterator it = heads_.find(key);
  if (it != heads_.end())
    heads_.erase(it);
  heads_.insert(std::make_pair(key, value));
}

} // namespace
