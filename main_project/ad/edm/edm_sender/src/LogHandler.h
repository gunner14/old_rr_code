/*
 * LogHandler.h
 *
 *  Created on: 2012-2-24
 *      Author: ping.zhang
 */

#ifndef LOGHANDLER_H_
#define LOGHANDLER_H_

#include "MatchHandler.h"

namespace xce {
namespace ad {

class LogHandler : public EdmMatchHandler {
  public:
    virtual bool handle(MatchHandlerParamter& para);
};

class LogHelper : public Singleton<LogHelper> {
  public:
    void ProcessUrl(string& str);
    string MakeIp(int ip);
    string GetMD5(const string &str);
};

}
}
#endif /* LOGHANDLER_H_ */
