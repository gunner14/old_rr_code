#ifndef _RMESSAGE_GATE_H_
#define _RMESSAGE_GATE_H_

#include <ctemplate/template.h>
#include <boost/lexical_cast.hpp>

#include "Notify.h"
#include "RmessageGate.h"
#include "RFeed.h"
#include "Singleton.h"
#include "ServiceI.h"
#include "Notify/util/fcgi_request.h"

namespace xce {
namespace notify {

using namespace MyUtil;
using namespace com::xiaonei::xce;

class TimeStatII{
  timeval _begin, _end;
  public:
  TimeStatII(){ 
    reset();
  }
  void reset(){ 
    gettimeofday(&_begin, NULL);
  } 
  float getTime(){
    gettimeofday(&_end, NULL);
    float timeuse=1000000*(_end.tv_sec-_begin.tv_sec)+_end.tv_usec
      -_begin.tv_usec;
    timeuse/=1000;
    return timeuse;
  } 
};  

class MessageCenterRequest : public Request {
public:
  MessageCenterRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
  static ctemplate::Template * GetTemplate() {
    // TODO : lock
    if (!template_) {
      template_ = ctemplate::Template::GetTemplate("../etc/rmessage-center/message-center.tpl", ctemplate::DO_NOT_STRIP);
    }
    return template_; 
  }
private:
  static ctemplate::Template * template_;
};

class MessageTestCenterRequest : public Request {
  public:
    MessageTestCenterRequest(FCGX_Request* r) : Request(r) {}
    virtual bool response();
    static ctemplate::Template * GetTemplate() {
      // TODO : lock
      if (!template_) {
        template_ = ctemplate::Template::GetTemplate("../etc/rmessage-testcenter/message-testcenter.tpl", 
            ctemplate::DO_NOT_STRIP);
      }
      return template_;
    }
  private:
    static ctemplate::Template * template_;
};

class MessageMoreRequest : public Request {
  public:
    MessageMoreRequest(FCGX_Request* r) : Request(r) {}
    virtual bool response();
    static ctemplate::Template * GetTemplate() {
      // TODO : lock
      if (!template_) {
        template_ = ctemplate::Template::GetTemplate("../etc/rmessage-center/message-more.tpl", 
            ctemplate::DO_NOT_STRIP);
      }
      return template_;
    }
  private:
    static ctemplate::Template * template_;
};

class MessageCenterApply : public Request {
  public:
    MessageCenterApply(FCGX_Request* r) : Request(r) {}
    virtual bool response();
    static ctemplate::Template * GetTemplate() {
      // TODO : lock
      if (!template_) {
        template_ = ctemplate::Template::GetTemplate("../etc/rmessage-center/message-apply.tpl",
            ctemplate::DO_NOT_STRIP);
      }
      return template_;
    }
  private:
    static ctemplate::Template * template_;
};

class MessageCenterAdd : public Request {
 public:
  MessageCenterAdd(FCGX_Request* r) : Request(r) {}
  virtual bool response();
  static ctemplate::Template * GetTemplate() {
    // TODO : lock
    if (!template_) {
      template_ = ctemplate::Template::GetTemplate("../etc/rmessage-center/message-apply.tpl",
        ctemplate::DO_NOT_STRIP);
    }
    return template_;
  }
 private:
  static ctemplate::Template * template_;
};

class RemoveAllMessageByBigtype : public Request {
 public:
  RemoveAllMessageByBigtype(FCGX_Request* r) : Request(r) {}
  virtual bool response();
};

class ReadAllMessageByBigtype : public Request {
 public:
  ReadAllMessageByBigtype(FCGX_Request* r) : Request(r) {}
  virtual bool response();
};

struct TplIncludeInfo {
  TplIncludeInfo(int id, const char * name, const char * file) : type_id(id), dict_name(name), tpl_file(file) {}
  int type_id; // 注意不是schema_id
  string dict_name;
  string tpl_file;
};

class RmessageGateI : public RmessageGate, public MyUtil::Singleton<RmessageGateI> {
 public:
  RmessageGateI() {};
  virtual bool reloadSchema(const Ice::Current& = Ice::Current());
  string GetHtmlContent(int uid ,const Ice::Current& = ::Ice::Current());
  // return a number,0 has not msg, -1 is error, 1 has msg in db
  int HasRemindMsgInDb(int uid);
};

class FeedRequestFactory : public RequestFactory {
 public:
  virtual RequestPtr Create(FCGX_Request * r) {
    char * path = FCGX_GetParam("SCRIPT_NAME", r->envp);
    if (path) {
      if (strcmp(path, "/rmessage/rmessage-center.html") == 0) {
        return new MessageCenterRequest(r);
      } else if (strcmp(path, "/rmessage/rmessage-more.html") == 0) {
        return new MessageMoreRequest(r);
      } else if (strcmp(path, "/rmessage/rmessage-testcenter.html") == 0) {
        return new MessageTestCenterRequest(r);
      } else if (strcmp(path, "/rmessage/rmessage-apply.html") == 0) {
        return new MessageCenterApply(r);
      } else if (strcmp(path, "/rmessage/rmessage-add.html") == 0) {
        return new MessageCenterAdd(r);
      } else if (strcmp(path, "/rmessage/rmessage-rmall.html") == 0) {
        return new RemoveAllMessageByBigtype(r);
      } else if (strcmp(path, "/rmessage/rmessage-readall.html") == 0) {
        return new ReadAllMessageByBigtype(r);
      }
    }
    return NULL;
  }
};

}
}

#endif // _RMESSAGE_GATE_H_

