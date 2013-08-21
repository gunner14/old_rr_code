#ifndef _NET_SPDY_METHOD_H_
#define _NET_SPDY_METHOD_H_
#include <string>
#include <vector>
#include <map>

namespace xoa {
const std::string kAcceptType = "text/plain,application/serializable,application/protoc-buff,application/json";
const std::string kAcceptCharset = "utf-8";
const std::string kContentLength = "0";
const std::string kContentType = "text/html;charset=UTF-8";
const std::string kUserAgent = "xoa-c-client";

class Method {
 public:
  static Method Get(const std::string &path);
  static Method Put(const std::string &path);
  static Method Post(const std::string &path);
  static Method Delete(const std::string &path);
  
  // json或bson或自定义类型,默认为:"text/plain,application/serializable,application/protoc-buff,application/json"
  // 即支持空白/protoc-buff/json等格式数据,最终会放置到header中
  void SetAcceptType(const std::string &type); 
  
  // 设置内容类型及内容,仅仅post或put能调用此函数,相当于http-form数据
  // 指定为:application/json等自定义类型,请确保服务端支持此种类型
  void SetContent(const std::string &type, const std::string &content); 

  // 设置参数,这个参数相当于http-url的后边参数
  void SetParam(const std::string &key, const std::string &value);

  // 相当于http头的head,一般不用这个参数, 仅仅放置自定义header,除非你知道后果
  void SetHeader(const std::string &key, const std::string &value); 

  std::string url() const; // path + params
  const std::string &content() const; // params or content_
  const std::map<std::string, std::string> &heads() const;
  std::string method() const; // GET or PUT or POST or DELETE
  const std::string &host() const;
  int mod() const;
 private:
  void ReplaceHeader(const std::string &key, const std::string &value);
  Method(const std::string &ms, const std::string &path);
  std::string MakeString();
  // newpath 必须非空
  bool GenPath(std::string *newpath) const;
  friend class XoaClient;
  std::string method_style_;
  std::string path_;
  std::string host_;
  std::string content_;
  std::string params_;
  mutable std::map<std::string, std::string> heads_;

  std::vector<std::string> path_tok_;
  std::map<std::string, std::string> path_var_;
  int mod_;
  //std::string url_;
};

} // namespace
#endif
