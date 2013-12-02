#include "User3GNotifyAdapter.h"
#include "LogWrapper.h"

using namespace xce::adapter::user3gnotify;

void User3GNotifyAdapter::Update(const int user_id, const int table, const wap::user::detail::PropertyMap& props) {
  try {
    switch(table) {
      case TABLE_USER_BORN :
        userBornChange(user_id, props); break;
      case TABLE_USER_NAME :
        userNameChange(user_id, props); break;
      case TABLE_USER_PASSPORT :
        userPassportChange(user_id, props); break;
      case TABLE_USER_URL :
        userUrlChange(user_id, props); break;
      case TABLE_USER_CONTACT :
        userContactChange(user_id, props); break;
      default :
        break;
    }
  } catch (std::exception e) {
    MCE_INFO("User3GNotifyAdapter::Update exception user_id = " 
        << user_id << ", table = " << table << ", " << e.what());
  }
}
