/*
 * UpdateUserInfoHandler.h
 *
 *  Created on: 2012-2-27
 *      Author: zhizhao.sun
 */

#ifndef UPDATEUSERINFOHANDLER_H_
#define UPDATEUSERINFOHANDLER_H_

#include "MatchHandler.h"
namespace xce {
namespace ad {
class UpdateUserInfoHandler : public EdmMatchHandler {
  public:
    virtual bool handle(MatchHandlerParamter& para);
  private:
    void UpdateUserInfo(int user_id, Ice::Long group_id, SendType edm_type); //用户信息更新
    void UpdateDbForWeeklySendEdm(int user_id, Ice::Long group_id); //数据库数据更新
    void UpdateDbForDailySendEdm(int user_id, Ice::Long group_id);
};

}
}


#endif /* UPDATEUSERINFOHANDLER_H_ */
