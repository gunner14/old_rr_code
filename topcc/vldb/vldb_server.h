#include "bson/bson.h"
#include "net/spdy_server.h"


#include "storage/vldb.h"

struct StorageInfo {
  arch::DocumentFile df;
  arch::Index *idx;
  time_t last;
};
/*
namespace mongo {
 class bo  {
 };

};*/

using namespace net;
class VldbSession : public SpdySession {
 public:
  VldbSession(boost::asio::io_service & ios)
  : SpdySession(ios) {

  }

  void OnError(bool send, spdy::SpdyStreamId id, int error) {
    printf("client stream(%d) have error:%d\n", id, error);
  }

  SpdyStreamSink* OnNew(boost::asio::io_service & ios) {
    return new VldbSession(ios);
  }

  void OnOpen(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param);
  void OnClose(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param);
  void OnGetSize(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param);
  void OnAdd(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param, const char* data, int length);

  void OnGetPage(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param);

  void OnGet(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param);

  void OnGetPageSize(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param); 
  void OnGetPageList(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param); 
  void OnRemovePage(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param); 
  void OnRemoveItem(spdy::SpdyStreamId id, const std::map<std::string, std::string> &param);
 private:
  void Put200Ok(spdy::SpdyStreamId id, long ret);
  void Put510Error(spdy::SpdyStreamId id, int errorcode, const std::string &msg);

  arch::Index* VerifyDbOpened(int id, StorageInfo **si = NULL);
  std::map<int, StorageInfo*> storage_map_;

  //DECLEAR_CALLBACK(VldbSession);
 friend bool _VldbSession_init_call_map();
 public:
  typedef void (VldbSession::*CallFunc)(spdy::SpdyStreamId id, const std::map<std::string,std::string> &param);
  typedef void (VldbSession::*CallFunc3)(spdy::SpdyStreamId id, const std::map<std::string,std::string> &param,
                                         const bson::bo &bs);
  typedef void (VldbSession::*CallFunc4)(spdy::SpdyStreamId id, const std::map<std::string,std::string> &param,
                                         const char* data, int length);
 private:
  static std::map<std::string, CallFunc> _call_map_;
  static std::map<std::string, CallFunc3> _call3_map_;
  static std::map<std::string, CallFunc4> _call4_map_;
  static bool _call_once_init_ret_;
  virtual void OnRequest(SpdyConnector &context,     
                       spdy::SpdyStreamId id, const spdy::SpdyHeaderBlock &block, 
                       const std::string &path, const std::map<std::string,std::string> &param, 
                       const char* data, int length);
};

