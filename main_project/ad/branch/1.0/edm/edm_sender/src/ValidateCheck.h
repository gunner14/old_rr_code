/*
 * ValidateCheck.h
 *
 * module role :避免短时间内重复登录造成的新鲜事重复发送
 * Created on: 2012-2-14
 * Author: shuangshuang
 * 
 */

#ifndef __EDM_VALIDATE_CHECK_H__
#define __EDM_VALIDATE_CHECK_H__

#include "Singleton.h"
#include "TaskManager.h"
#include <set>

namespace xce {
namespace ad {

class ValidateCheckCache : public MyUtil::Singleton<ValidateCheckCache> {
public:

  bool Clear();
  bool RemoveUserFootPrint(int user_id);
  bool CheckUserValid(int user_id);  //用户信息监测和频率监测

private:
  bool CheckFootPrintAndInsert(int user_id);
  bool CheckUserGuide(int user_id);
  std::set<int> user_login_;
  IceUtil::Mutex mutex_;
};


}
}
#endif
