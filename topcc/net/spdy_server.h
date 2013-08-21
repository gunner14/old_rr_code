#ifndef _NET_SPDY_SERVER_H_
#define _NET_SPDY_SERVER_H_

#include "net/spdy_sess.h"

namespace xoa {

#define DECLEAR_CALLBACK(theclass) \
  typedef void (##theclass::*CallFunc)(int id, const std::map<std::string,std::string> &param); \
  static std::map<std::string, CallFunc> _call_map_; \
  virtual void OnRequest(SpdyConnector &context,     \
                       spdy::SpdyStreamId id, const spdy::SpdyHeaderBlock &block, \
                       const std::string &path, const std::map<std::string,std::string> &param, \
                       const char* data, int length);

#define START_FUNC(theclass)  \
  struct _##theclass_call_func_array { \
    std::string path;         \
    ##theclass::CallFunc callfunc;     \
  };                          \
  std::map<std::string, ##theclass::CallFunc> _##theclass_init_call_map() { \
    std::map<std::string, ##theclass::CallFunc> mm;  \
    const static  _##theclass_call_func_array callarray[] = {

#define END_FUNC(theclass)    \
    };    \
    const int numb = sizeof(callarray) / sizeof(_##theclass_call_func_array);\
    for(int i = 0; i < numb; i++) { \
      mm.insert(std::make_pair(callarray[i].path, callarray[i].callfunc));  \
    } \
    return mm;  \
  } \
  std::map<std::string, ##theclass::CallFunc> ##theclass::_call_map_ = _##theclass_init_call_map(); \
  void VldbSession::OnRequest(SpdyConnector &context,\
                       spdy::SpdyStreamId id, const spdy::SpdyHeaderBlock &block, \
                       const std::string &path, const std::map<std::string,std::string> &param, \
                       const char* data, int length) { \
   std::map<std::string, CallFunc>::iterator it =  _call_map_.find(path); \
   if (it == _call_map_.end()) \
     return ;   \
   (this->*(it->second)) (id, param); \
}


#define FUNC_ITEM(str, func) {str,&func},

  
class SpdySession : public SpdyStreamSink {
 public:
  SpdySession(boost::asio::io_service & ios);

  void OnError(bool send, spdy::SpdyStreamId id, int error, int mod); 

  SpdyStreamSink* OnNew(boost::asio::io_service & ios);

  virtual void OnRequest(SpdyConnector &context,
                 spdy::SpdyStreamId id, const spdy::SpdyHeaderBlock &block, 
                 const std::string &path, const std::map<std::string, std::string> &param, 
                 const char* data, int length);
  spdy::SpdyStreamId PutBson(spdy::SpdyStreamId id, const std::string &code, const std::string &bson);
 private:
  void OnStream(spdy::SpdyStreamId id, const spdy::SpdyHeaderBlock * block, const char* buf, int sz);
  SpdyConnector* GetContext();
  SpdyConnector base_;
};

} // namespace
#endif // define
