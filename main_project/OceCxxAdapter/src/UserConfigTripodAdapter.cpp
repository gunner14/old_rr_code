#include "UserConfigTripodAdapter.h"
#include "LogWrapper.h"
//#include <boost/lexical_cast.hpp>

using namespace xce::adapter;
//using namespace std;
//using namespace mop::hi::svc::model;
//using namespace MyUtil;
//using namespace boost;


UserConfigInfoPtr UserConfigTripodAdapter::getUserConfig(int userid, const Ice::Context& ctx) {
  using namespace xce::tripod;
  UserConfigInfoPtr info = new UserConfigInfo;
  UserConfigData data;

  bool isValid = client_->Get(userid, &data, kExpireSeconds);

  if (isValid) {
    info->parseFromProtoData(data);
  }

  return info;
}

bool UserConfigTripodAdapter::reload(int id, const Ice::Context& ctx) {
  return client_->Load(id, kExpireSeconds);
}

bool UserConfigTripodAdapter::remove(int id) {
  return client_->Remove(id);
}

//UserConfigInfoPtr UserConfigInfo::parseFromProtoData(const xce::tripod::UserConfigData& data) {
//  userId_ = data.id();
//  basicConfig_ = data.basic_config();
//  pokeConfig_ = data.poke_config();
//  requestFriendConfig_ = data.request_friend_config();
//  photoConfig_ = data.photo_config();
//  messageConfig_ = data.message_config();
//  emailConfig_ = data.email_config();
//  browseConfig_ = data.browse_config();
//  displayMenuList_ = data.display_menu_list();
//  moreMenuList_ = data.more_menu_list();
//  wantSeeCss_ = data.want_see_css();
//  sendFeedConfig_ = data.send_feed_config();
//  recvFeedConfig_ = data.recv_feed_config();
//  profilePrivacy_ = data.profile_privacy();
//  statusConfig_ = data.status_config();
//  return this;
//}

//void UserConfigInfo::print() {
//  MCE_INFO("UserConfigInfo---------------------");
//  MCE_INFO( "[id]:" << userId_);
//  MCE_INFO("[email_config]:" << emailConfig_);
//  MCE_INFO("[basic_config]:" << basicConfig_);
//  MCE_INFO("[poke_config]:" << pokeConfig_);
//  MCE_INFO("[request_friend_config]:" << requestFriendConfig_);
//  MCE_INFO("[photo_config]:" << photoConfig_);
//  MCE_INFO("[message_config]:" << messageConfig_);
//  MCE_INFO("[browse_config]:" << browseConfig_);
//  MCE_INFO("[display_menu_list]:" << displayMenuList_);
//  MCE_INFO("[more_menu_list]:" << moreMenuList_);
//  MCE_INFO("[want_see_css]:" << wantSeeCss_);
//  MCE_INFO("[send_feed_config]:" << sendFeedConfig_);
//  MCE_INFO("[recv_feed_config]:" << recvFeedConfig_);
//  MCE_INFO("[profile_privacy]:" << profilePrivacy_);
//  MCE_INFO("[status_config]:" << statusConfig_);
//}

//void UserConfigTripodAdapter::printData(const xce::tripod::UserConfigData& data) {
//  MCE_INFO("UserConfigData---------------------");
//  MCE_INFO("[id]:" << data.id());
//  MCE_INFO("[email_config]:" << data.email_config());
//  MCE_INFO("[basic_config]:" << data.basic_config());
//  MCE_INFO("[poke_config]:" << data.poke_config());
//  MCE_INFO("[request_friend_config]:" << data.request_friend_config());
//  MCE_INFO("[photo_config]:" << data.photo_config());
//  MCE_INFO("[message_config]:" << data.message_config());
//  MCE_INFO("[browse_config]:" << data.browse_config());
//  MCE_INFO("[display_menu_list]:" << data.display_menu_list());
//  MCE_INFO("[more_menu_list]:" << data.more_menu_list());
//  MCE_INFO("[want_see_css]:" << data.want_see_css());
//  MCE_INFO("[send_feed_config]:" << data.send_feed_config());
//  MCE_INFO("[recv_feed_config]:" << data.recv_feed_config());
//  MCE_INFO("[profile_privacy]:" << data.profile_privacy());
//  MCE_INFO("[status_config]:" << data.status_config());
//}

