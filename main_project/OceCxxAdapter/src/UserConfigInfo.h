#ifndef __USER_CONFIG_INFO_H__
#define __USER_CONFIG_INFO_H__

#include <Markable.h>
#include <boost/lexical_cast.hpp>
#include "UserConfig.h"
#include "UserConfig.pb.h"


namespace xce {
namespace adapter {

using namespace MyUtil;

class UserConfigInfo;
typedef IceUtil::Handle<UserConfigInfo> UserConfigInfoPtr;
class UserConfigInfo: virtual public Ice::Object, public MyUtil::Markable {
public:
	UserConfigInfoPtr parse(const MyUtil::Str2StrMap& data){
    Str2StrMap::const_iterator i;
    i = data.find("ID");
    if (i != data.end()) {
      userId_ = boost::lexical_cast<int>(i->second);
    }
    i = data.find("BASIC_CONFIG");
    if (i != data.end()) {
      basicConfig_ = boost::lexical_cast<int>(i->second);
    }
    i = data.find("POKE_CONFIG");
    if (i != data.end()) {
      pokeConfig_ = boost::lexical_cast<int>(i->second);
    }
    i = data.find("REQUEST_FRIEND_CONFIG");
    if (i != data.end()) {
      requestFriendConfig_ = boost::lexical_cast<int>(i->second);
    }
    i = data.find("PHOTO_CONFIG");
    if (i != data.end()) {
      photoConfig_ = boost::lexical_cast<int>(i->second);
    }
    i = data.find("MESSAGE_CONFIG");
    if (i != data.end()) {
      messageConfig_ = boost::lexical_cast<int>(i->second);
    }
    i = data.find("EMAIL_CONFIG");
    if (i != data.end()) {
      emailConfig_ = boost::lexical_cast<long>(i->second);
    }
    i = data.find("BROWSE_CONFIG");
    if (i != data.end()) {
      browseConfig_ = boost::lexical_cast<int>(i->second);
    }
    i = data.find("RECV_FEED_CONFIG");
    if (i != data.end()) {
      recvFeedConfig_ = i->second;
    }
    i = data.find("SEND_FEED_CONFIG");
    if (i != data.end()) {
      sendFeedConfig_ = i->second;
    }
    i = data.find("WANT_SEE_CSS");
    if (i != data.end()) {
      wantSeeCss_ = boost::lexical_cast<short>(i->second);
    }
    i = data.find("DISPLAY_MENU_LIST");
    if (i != data.end()) {
      displayMenuList_ = i->second;
    }
    i = data.find("MORE_MENU_LIST");
    if (i != data.end()) {
      moreMenuList_ = i->second;
    }
    i = data.find("PROFILE_PRIVACY");
    if (i != data.end()) {
      profilePrivacy_ = i->second;
    }
    i = data.find("STATUS_CONFIG");
    if (i != data.end()) {
      statusConfig_ = boost::lexical_cast<int>(i->second);
    }
    return this;
  }

  UserConfigInfoPtr parseFromProtoData(const xce::tripod::UserConfigData& data) {
    userId_ = data.id();
    basicConfig_ = data.basic_config();
    pokeConfig_ = data.poke_config();
    requestFriendConfig_ = data.request_friend_config();
    photoConfig_ = data.photo_config();
    messageConfig_ = data.message_config();
    emailConfig_ = data.email_config();
    browseConfig_ = data.browse_config();
    displayMenuList_ = data.display_menu_list();
    moreMenuList_ = data.more_menu_list();
    wantSeeCss_ = data.want_see_css();
    sendFeedConfig_ = data.send_feed_config();
    recvFeedConfig_ = data.recv_feed_config();
    profilePrivacy_ = data.profile_privacy();
    statusConfig_ = data.status_config();
    return this;
  }

  void print() {
    std::cout <<"UserConfigInfo---------------------" << std::endl;
    std::cout << "[id]:" << userId_ << std::endl;
    std::cout <<"[email_config]:" << emailConfig_ << std::endl;
    std::cout <<"[basic_config]:" << basicConfig_ << std::endl;
    std::cout <<"[poke_config]:" << pokeConfig_ << std::endl;
    std::cout <<"[request_friend_config]:" << requestFriendConfig_ << std::endl;
    std::cout <<"[photo_config]:" << photoConfig_ << std::endl;
    std::cout <<"[message_config]:" << messageConfig_ << std::endl;
    std::cout <<"[browse_config]:" << browseConfig_ << std::endl;
    std::cout <<"[display_menu_list]:" << displayMenuList_ << std::endl;
    std::cout <<"[more_menu_list]:" << moreMenuList_ << std::endl;
    std::cout <<"[want_see_css]:" << wantSeeCss_ << std::endl;
    std::cout <<"[send_feed_config]:" << sendFeedConfig_ << std::endl;
    std::cout <<"[recv_feed_config]:" << recvFeedConfig_ << std::endl;
    std::cout <<"[profile_privacy]:" << profilePrivacy_ << std::endl;
    std::cout <<"[status_config]:" << statusConfig_ << std::endl;
  }

	int userId() const {
		return userId_;
	}
	int basicConfig() const {
		return basicConfig_;
	}
	int pokeConfig() const {
		return pokeConfig_;
	}
	int requestFriendConfig() const {
		return requestFriendConfig_;
	}
	int photoConfig() const {
		return photoConfig_;
	}
	int messageConfig() const {
		return messageConfig_;
	}
	long emailConfig() const {
		return emailConfig_;
	}
	int browseConfig() const {
		return browseConfig_;
	}
	string displayMenuList() const {
		return displayMenuList_;
	}
	string moreMenuList() const {
		return moreMenuList_;
	}
	short wantSeeCss() const {
		return wantSeeCss_;
	}
	string sendFeedConfig() const {
		return sendFeedConfig_;
	}
	string recvFeedConfig() const {
		return recvFeedConfig_;
	}
	string profilePrivacy() const {
		return profilePrivacy_;
	}
	int statusConfig() const {
		return statusConfig_;
	}

	void set_userId(int userId) {
		userId_ = userId;
		mark("ID", boost::lexical_cast<string>(userId));
	}
	void set_basicConfig(int basicConfig) {
		basicConfig_ = basicConfig;
		mark("BASIC_CONFIG", boost::lexical_cast<string>(basicConfig));
	}
	void set_pokeConfig(int pokeConfig) {
		pokeConfig_ = pokeConfig;
		mark("POKE_CONFIG", boost::lexical_cast<string>(pokeConfig));
	}
	void set_requestFriendConfig(int requestFriendConfig) {
		requestFriendConfig_ = requestFriendConfig;
		mark("REQUEST_FRIEND_CONFIG", boost::lexical_cast<string>(requestFriendConfig));
	}
	void set_photoConfig(int photoConfig) {
		photoConfig_ = photoConfig;
		mark("PHOTO_CONFIG", boost::lexical_cast<string>(photoConfig));
	}
	void set_messageConfig(int messageConfig) {
		messageConfig_ = messageConfig;
		mark("MESSAGE_CONFIG", boost::lexical_cast<string>(messageConfig));
	}
	void set_emailConfig(long emailConfig) {
		emailConfig_ = emailConfig;
		mark("EMAIL_CONFIG", boost::lexical_cast<string>(emailConfig));
	}
	void set_browseConfig(int browseConfig) {
		browseConfig_ = browseConfig;
		mark("BROWSE_CONFIG", boost::lexical_cast<string>(browseConfig));
	}
	void set_displayMenuList(const string& displayMenuList) {
		displayMenuList_ = displayMenuList;
		mark("DISPLAY_MENU_LIST", displayMenuList);
	}
	void set_moreMenuList(const string& moreMenuList) {
		moreMenuList_ = moreMenuList;
		mark("MORE_MENU_LIST", moreMenuList);
	}
	void set_wantSeeCss(short wantSeeCss) {
		wantSeeCss_ = wantSeeCss;
		mark("WANT_SEE_CSS", boost::lexical_cast<string>(wantSeeCss));
	}
	void set_sendFeedConfig(const string& sendFeedConfig) {
		sendFeedConfig_ = sendFeedConfig;
		mark("SEND_FEED_CONFIG", sendFeedConfig);
	}
	void set_recvFeedConfig(const string& recvFeedConfig) {
		recvFeedConfig_ = recvFeedConfig;
		mark("RECV_FEED_CONFIG", recvFeedConfig);
	}
	void set_profilePrivacy(const string& profilePrivacy) {
		profilePrivacy_ = profilePrivacy;
		mark("PROFILE_PRIVACY", profilePrivacy);
	}
	void set_statusConfig(int statusConfig) {
		statusConfig_ = statusConfig;
		mark("STATUS_CONFIG", boost::lexical_cast<string>(statusConfig));
	}
  

private:
	int userId_;
	int basicConfig_;
	int pokeConfig_;
	int requestFriendConfig_;
	int photoConfig_;
	int messageConfig_;
	long emailConfig_;
	int browseConfig_;
	string displayMenuList_;
	string moreMenuList_;
	short wantSeeCss_;
	string sendFeedConfig_;
	string recvFeedConfig_;
	string profilePrivacy_;
	int statusConfig_;
};


}
}

#endif
