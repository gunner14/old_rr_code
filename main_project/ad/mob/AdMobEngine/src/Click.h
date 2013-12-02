/*
 *  Click.h
 *
 *   Created on: 2012-3-31
 *       Author: yongjun.yin
 *  Description: 定义 点击处理类 ClickRequest
 */

#ifndef __AD_MOB_CLICK_H__
#define __AD_MOB_CLICK_H__

#include "LogWrapper.h"
#include "Notify/util/fcgi_request.h"
//#include <fcgi_stdio.h>

namespace xce {
namespace ad {

using namespace xce::notify;

class ClickRequest : public Request{
public:
  ClickRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
private:
  inline bool ProcessCpcLog(string& cpclog);
  inline bool ProcessClickLog(string& click_log);
};

class OtherRequest : public Request{
public:
  OtherRequest(FCGX_Request* r) : Request(r) {}
  virtual bool response();
};

class AdMobRequestFactory : public RequestFactory {
public:
  virtual RequestPtr Create(FCGX_Request * r) {
    char * path = FCGX_GetParam("SCRIPT_NAME", r->envp);
    if (path) {
      if (strcmp(path, "/mclick/click") == 0) {
        return new ClickRequest(r);
      } else {
        return new OtherRequest(r);
      }
    }
    return NULL;
  }
};


}
}

#endif
