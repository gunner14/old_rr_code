/*
 * FeedLogRequest.h
 *
 *   Create on: 2012-11-13
 *      Author: rui.qiu
 * Description: 定义处理各种类型log的Request
 */

#ifndef __FEED_LOG_REQUEST_H__
#define __FEED_LOG_REQUEST_H__

#include <string>
#include "LogWrapper.h"
#include "Singleton.h"
#include "Util.h"
#include "Notify/util/fcgi_request.h"

namespace xce {
namespace feed {

using namespace xce::notify;
using namespace MyUtil;

const string gPv = "pv";
const string gClick = "click";
const string gReply = "reply";
const string gShare = "share";
const string gLike = "like";
const string gRead = "read";
const string gBlock = "block";
const string gAction = "action";
const string gAt = "at";

class PvRequest : public Request {
public:
  PvRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
private:
  inline bool ProcessPvLog(string& pv_log);
};

class ClickRequest : public Request {
public:
  ClickRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
private:
  inline bool ProcessClickLog(string& click_log);
};

class ReplyRequest : public Request {
public:
  ReplyRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
private:
  inline bool ProcessReplyLog(string& reply_log);
};

class ShareRequest : public Request {
public:
  ShareRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
private:
  inline bool ProcessShareLog(string& share_log);
};

class LikeRequest : public Request {
public:
  LikeRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
private:
  inline bool ProcessLikeLog(string& like_log);
};

class ReadRequest : public Request {
public:
  ReadRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
private:
  inline bool ProcessReadLog(string& read_log);
};

class BlockRequest : public Request {
public:
  BlockRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
private:
  inline bool ProcessBlockLog(string& block_log);
};

class ActionRequest : public Request {
public:
  ActionRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
private:
  inline bool ProcessActionLog(string& action_log);
};

class AtRequest : public Request {
public:
  AtRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
private:
  inline bool ProcessAtLog(string& at_log);
};

class FeedLogRequest : public Request {
public:
  FeedLogRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
  const static char *log_types_[];
private:
  inline bool ProcessFeedLog(string& at_log);
};

class FeedLogRequestFactory : public RequestFactory {
public:
  virtual RequestPtr Create(FCGX_Request* r) {
    char* path = FCGX_GetParam("SCRIPT_NAME", r->envp);
    if (path) {
      if (strcmp(path, "/xfeed/log/pv") == 0) {
        return new PvRequest(r);
      } else if (strcmp(path, "/xfeed/log/click") == 0) {
        return new ClickRequest(r);
      } else if (strcmp(path, "/xfeed/log/reply") == 0) {
        return new ReplyRequest(r);
      } else if (strcmp(path, "/xfeed/log/share") == 0) {
        return new ShareRequest(r);
      } else if (strcmp(path, "/xfeed/log/like") == 0) {
        return new LikeRequest(r);
      } else if (strcmp(path, "/xfeed/log/read") == 0) {
        return new ReadRequest(r);
      } else if (strcmp(path, "/xfeed/log/block") == 0) {
        return new BlockRequest(r);
      } else if (strcmp(path, "/xfeed/log/action") == 0) {
        return new ActionRequest(r);
      } else if (strcmp(path, "/xfeed/log/at") == 0) {
        return new AtRequest(r);
      } else if (strcmp(path, "/xfeed/log") == 0) {
        return new FeedLogRequest(r);
      }
    }
    return NULL;
  }
};

class LogStringParser : public MyUtil::Singleton<LogStringParser> {
public:
  MyUtil::StrSeq LogSpliter(const string& log_name, const string& logs);
};

}
}

#endif
