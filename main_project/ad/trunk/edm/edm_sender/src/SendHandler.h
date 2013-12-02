/*
 * SendHandler.h
 *
 *  Created on: 2012-2-27
 *      Author: zhizhao.sun
 */

#ifndef SENDHANDLER_H_
#define SENDHANDLER_H_
#include "MatchHandler.h"
namespace xce {
namespace ad {

class SendHandler : public EdmMatchHandler {
  public:
    virtual bool handle(MatchHandlerParamter& para);
  private:
    bool JsonToMap(const std::string &json_str,MyUtil::Str2StrMap& json_map);
    void BuildEdmAndSend(const UserProfile &user_id, string &json_str, const AdGroupPtr &group_ptr);
    void ProcessJsonStr(string &json_str, string target, string replace);
    //void ProcessUrl(string& str); 

};

}
}


#endif /* SENDHANDLER_H_ */
