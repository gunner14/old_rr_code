/*
 * ValidateCheck.cpp
 *
 * module role :防止短时间内重复登录造成的新鲜事重复发送
 * Created on: 2012-2-14
 * Author: shuangshuang
 * 
 */

#include "ValidateCheck.h"
#include "LogWrapper.h"
#include "UserStateAdapter.h"

namespace xce {
namespace ad {

bool ValidateCheckCache::CheckUserValid(int user_id) {
  if (user_id < 0) {
    return false;
  }

  bool valid = CheckUserGuide(user_id);
  if (!valid) {
    MCE_INFO("User not pass the guide,user_id:"<<user_id);
    return false;
  }

  bool duplicate = CheckFootPrintAndInsert(user_id);
  if (duplicate) {
    MCE_DEBUG("User duplicate log in user_id:" << user_id);
    return false;
  }

  return true;
}

bool ValidateCheckCache::CheckFootPrintAndInsert(int user_id) {
  IceUtil::Mutex::Lock lock(mutex_);
  std::set<int>::iterator ite = user_login_.find(user_id);
  if (ite != user_login_.end()) {
    return true;
  } 
  user_login_.insert(user_id);
  return false;
}

bool ValidateCheckCache::RemoveUserFootPrint(int user_id) {
  IceUtil::Mutex::Lock lock(mutex_);
  std::set<int>::iterator ite = user_login_.find(user_id);
  if (ite != user_login_.end()) {
    user_login_.erase(ite);
  }  
  return true;
}

bool ValidateCheckCache::Clear() {
  IceUtil::Mutex::Lock lock(mutex_);
  user_login_.clear();
  return true;
}

bool ValidateCheckCache::CheckUserGuide(int user_id) {
  xce::adapter::userstate::UserStateInfoPtr user_state_ptr;
  try{
    user_state_ptr = xce::adapter::userstate::UserStateAdapter::instance().getUserState(user_id);
  } catch (Ice::Exception& e) {
    MCE_WARN("call UserStateAdapter error -> "<<e);
  } catch (std::exception& ex) {
    MCE_WARN("call UserStateAdapter exception error -> "<<ex.what());
  } catch (...) {
    MCE_WARN("call UserStateAdapter failed");
  }
  if(user_state_ptr){
    return user_state_ptr->isGuideDone();
  }
  return false;
}

}
}
