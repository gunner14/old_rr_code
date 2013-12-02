#ifndef __USER_3G_NOTIFY_ADAPTER_H__
#define __USER_3G_NOTIFY_ADAPTER_H__

#include "UserDetailAdapter.h"
#include "Singleton.h"

namespace xce {
namespace adapter {
namespace user3gnotify {

const static int TABLE_USER_BORN = 1;
const static int TABLE_USER_NAME = 2;
const static int TABLE_USER_PASSPORT = 3;
const static int TABLE_USER_URL = 4;
const static int TABLE_USER_CONTACT = 5;

using namespace MyUtil;

class User3GNotifyAdapter : public wap::adapter::UserDetailAdapter, public MyUtil::Singleton<User3GNotifyAdapter> {
public:
	void Update(const int user_id, const int table, const wap::user::detail::PropertyMap& props);
private:
  friend class MyUtil::Singleton<User3GNotifyAdapter>;
	User3GNotifyAdapter() : wap::adapter::UserDetailAdapter() {}
};

}}} //namespace xce::adapter::user3gnotify

#endif

