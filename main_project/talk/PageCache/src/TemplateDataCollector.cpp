#include <boost/lexical_cast.hpp>
#include "TemplateDataCollector.h"
#include "PageCacheManagerI.h"
#include "MessageHolderAdapter.h"
#include "TalkFunStateAdapter.h"
#include "UserCountAdapter.h"
#include "FeedMemcProxy/client/talk_cache_client.h"
#include "util/cpp/String.h"

using namespace xce::talk;
using namespace com::xiaonei::talk::fun::state;
using namespace talk::adapter;
using namespace com::xiaonei::talk;
using namespace talk::http;
using ctemplate::TemplateDictionary;
static int GetUserId(const Str2StrMap& cookies, const Str2StrMap& parameter) {
  int uid = 0;
  Str2StrMap::const_iterator it = cookies.find("id");
  if (it != cookies.end() && (!it->second.empty())) {
    try {
      uid = boost::lexical_cast<int>(it->second);
      return uid;
    } catch (...) {
      MCE_WARN("OnlineBuddyListCollector::fillData --> cast err, " << it->second);
    }
  }

  it = parameter.find("uid");
  if (it != parameter.end()) {
    try {
      uid = boost::lexical_cast<int>(it->second);
    } catch (std::exception& e) {
      MCE_WARN("OnlineBuddyListCollector::fillData --> cast wap uid err:"<<e.what());
    }
  }
  return uid;
}

static string GetTicket(const Str2StrMap& cookies, const Str2StrMap& parameter) {
  Str2StrMap::const_iterator it = cookies.find("t");
  if (it != cookies.end()) {
    return it->second;
  }
  it = parameter.find("t");
  if (it != parameter.end()) {
    return it->second;
  }
  return string();
}


static int GetTargetUserId(const Str2StrMap& parameter){
	int targetuser = -1;
	Str2StrMap::const_iterator it = parameter.find("targetuser");
	if(it != parameter.end()){
  	try {
      targetuser = boost::lexical_cast<int>(it->second);
    } catch (std::exception& e) {
      MCE_WARN("OnlineBuddyListCollector::fillData GetTargetUserId cast targetuser = " << it->second << "--> ERROR  err:"<<e.what());
			return -1;
    }
	}
	return targetuser;
}


bool OnlineBuddyListCollector::fillData(const std::string& path,
    const Str2StrMap& cookies, const Str2StrMap& parameter,
    TemplateDictionary& dict) {
  int uid = GetUserId(cookies, parameter);

  if (uid <= 0) {
    MCE_WARN("OnlineBuddyListCollector::fillData --> uid:" << uid << " path:" << path);
    return false;
  }

  string ticket = GetTicket(cookies, parameter);
	int video_view_power = PageCacheManagerI::instance().GetVideoViewPower();
  IceUtil::Mutex::Lock lock(mutex_[uid/10 % kLockSize]);

  ActiveUserPtr au = ServiceI::instance().locateObject<ActiveUserPtr> (AU, uid);
  if (au->checkTicket(ticket, path.find("/wap_") == 0)) {

    dict.SetValue("UID", boost::lexical_cast<string>(uid));
    dict.SetValue("USER_NAME", au->name());
    vector<BuddyPtr> bl;
		try{
			au->getFullBuddyList(bl);
		}catch(std::exception& e){
			MCE_WARN("OnlineBuddyListCollector::fillData --> get buddy list uid" << uid << " error:" << e.what());
		}
    MCE_INFO("OnlineBuddyListCollector::fillData --> get buddy list uid:" << uid << " size" << bl.size());
    for (size_t i = 0; i < bl.size(); ++i) {
      TemplateDictionary* buddy_dict = dict.AddSectionDictionary( "BUDDY_SECTION");
      buddy_dict->SetValue("VIDEO_VIEW_POWER", boost::lexical_cast<string>(video_view_power));
      buddy_dict->SetValue("BUDDY_TINY_URL", bl.at(i)->tinyUrl());
      buddy_dict->SetValue("BUDDY_ONLINE_STATUS", boost::lexical_cast<string>( bl.at(i)->onlineStatus()));
      buddy_dict->SetValue("BUDDY_NAME", bl.at(i)->name());
      //buddy_dict->SetValue("BUDDY_DOING", bl.at(i)->doing());
      buddy_dict->SetValue("BUDDY_ID", boost::lexical_cast<string>( bl.at(i)->id()));
    }

    dict.SetValue("BUDDY_COUNT", boost::lexical_cast<string>(bl.size()));
    return true;

  } else {
    return false;
  }
}

bool AllFullBuddyListCollector::fillData (const std::string& path,
    const Str2StrMap& cookies, const Str2StrMap& parameter,
    TemplateDictionary& dict) {

  int uid = GetUserId(cookies, parameter);



  int begin = -1;
  int limit = -1;
  Str2StrMap::const_iterator it = parameter.find("begin");
  if (it != parameter.end())
    begin = boost::lexical_cast<int>(it->second);

  it = parameter.find("limit");
  if (it != parameter.end())
    limit = boost::lexical_cast<int>(it->second);


  MCE_INFO("AllFullBuddyListCollector::fillData --> uid:" << uid << " path:" << path << " limit:" << limit << " begin:" << begin);
  
  if (begin < 0 || limit <= 0)
    return false;

  if (uid <= 0) {
    MCE_WARN("AllFullBuddyListCollector::fillData --> uid:" << uid << " path:" << path);
    return false;
  }



  string ticket = GetTicket(cookies, parameter);
  int video_view_power = PageCacheManagerI::instance().GetVideoViewPower();
	
  IceUtil::Mutex::Lock lock(mutex_[uid/10 % kLockSize]);

  ActiveUserPtr au = ServiceI::instance().locateObject<ActiveUserPtr> (AU, uid);
  if (au->checkTicket(ticket, path.find("/wap_") == 0)) {

    dict.SetValue("UID", boost::lexical_cast<string>(uid));
    dict.SetValue("USER_NAME", au->name());
    vector<BuddyPtr> bl;
		int online_count = 0;
    int friend_count = 0;
    try{
      au->getAllFullBuddyList(begin, limit, friend_count, online_count, bl);
    }catch(std::exception& e){
      MCE_WARN("AllFullBuddyListCollector::fillData --> get buddy list uid " << uid << " error:" << e.what());
    }
    MCE_INFO("AllFullBuddyListCollector::fillData --> get buddy list uid:" << uid << " size " << bl.size());
    for (size_t i = 0; i < bl.size(); ++i) {
      TemplateDictionary* buddy_dict = dict.AddSectionDictionary( "BUDDY_SECTION");
      buddy_dict->SetValue("VIDEO_VIEW_POWER", boost::lexical_cast<string>(video_view_power));
      buddy_dict->SetValue("BUDDY_TINY_URL", bl.at(i)->tinyUrl());
      buddy_dict->SetValue("BUDDY_ONLINE_STATUS", boost::lexical_cast<string>( bl.at(i)->onlineStatus()));
      buddy_dict->SetValue("BUDDY_NAME", bl.at(i)->name());
      //buddy_dict->SetValue("BUDDY_DOING", bl.at(i)->doing());
      buddy_dict->SetValue("BUDDY_ID", boost::lexical_cast<string>( bl.at(i)->id()));

      MyUtil::StrSeq buddygroups = bl.at(i)->buddyGroup();
      for(MyUtil::StrSeq::iterator bgit = buddygroups.begin(); bgit != buddygroups.end(); ++bgit){
        //MCE_INFO("GET IT!    id :" << bl.at(i)->id() << " gname:" << *bgit);
	TemplateDictionary* buddy_group_dict = buddy_dict->AddSectionDictionary("BUDDY_GROUP");
	buddy_group_dict->SetValue("BUDYGROUPNAME", *bgit);
      }
    }

    dict.SetValue("BUDDY_COUNT", boost::lexical_cast<string>(online_count));
    dict.SetValue("ALL_BUDDY_COUNT", boost::lexical_cast<string>(friend_count));
    return true;

  } else {
    return false;
  }

}

bool FullRecentBuddyListCollector::fillData (const std::string& path,
    const Str2StrMap& cookies, const Str2StrMap& parameter,
    TemplateDictionary& dict) {
	int uid = GetUserId(cookies, parameter);

  if (uid <= 0) {
    MCE_WARN("FullRecentBuddyListCollector::fillData --> uid:" << uid << " path:" << path);
    return false;
  }

  int option = 2;
  Str2StrMap::const_iterator it = parameter.find("opt");
  if (it != parameter.end())
    option = boost::lexical_cast<int>(it->second);


  string ticket = GetTicket(cookies, parameter);
	int video_view_power = PageCacheManagerI::instance().GetVideoViewPower();
	
	IceUtil::Mutex::Lock lock(mutex_[uid/10 % kLockSize]);

  ActiveUserPtr au = ServiceI::instance().locateObject<ActiveUserPtr> (AU, uid);
  if (au->checkTicket(ticket, path.find("/wap_") == 0)) {

    dict.SetValue("UID", boost::lexical_cast<string>(uid));
    dict.SetValue("USER_NAME", au->name());
    vector<BuddyPtr> bl;
    vector<string> recents;
    int friend_count = 0;
		try{
			au->getFullAndRecentBuddyList(option, bl, recents, friend_count);
		}catch(std::exception& e){
			MCE_WARN("FullRecentBuddyListCollector::fillData --> get buddy list uid " << uid << " error:" << e.what());
		}
    MCE_INFO("FullRecentBuddyListCollector::fillData --> get buddy list uid:" << uid << " size " << bl.size());
    for (size_t i = 0; i < bl.size(); ++i) {
      TemplateDictionary* buddy_dict = dict.AddSectionDictionary( "BUDDY_SECTION");
      buddy_dict->SetValue("VIDEO_VIEW_POWER", boost::lexical_cast<string>(video_view_power));
      buddy_dict->SetValue("BUDDY_TINY_URL", bl.at(i)->tinyUrl());
      buddy_dict->SetValue("BUDDY_ONLINE_STATUS", boost::lexical_cast<string>( bl.at(i)->onlineStatus()));
      buddy_dict->SetValue("BUDDY_NAME", bl.at(i)->name());
      //buddy_dict->SetValue("BUDDY_DOING", bl.at(i)->doing());
      buddy_dict->SetValue("BUDDY_ID", boost::lexical_cast<string>( bl.at(i)->id()));

			MyUtil::StrSeq buddygroups = bl.at(i)->buddyGroup();
			for(MyUtil::StrSeq::iterator bgit = buddygroups.begin(); bgit != buddygroups.end(); ++bgit){
        //				MCE_INFO("GET IT!    id :" << bl.at(i)->id() << " gname:" << *bgit);
				TemplateDictionary* buddy_group_dict = buddy_dict->AddSectionDictionary("BUDDY_GROUP");
				buddy_group_dict->SetValue("BUDYGROUPNAME", *bgit);
			}
    }

    for (size_t i = 0; i < recents.size(); ++i) {
      TemplateDictionary *recent_dict = dict.AddSectionDictionary( "RECENT_SECTION");
      recent_dict->SetValue("RECENT_INFO", recents.at(i));

    }
    dict.SetValue("ERROR_CODE", "0");
    dict.SetValue("BUDDY_COUNT", boost::lexical_cast<string>(friend_count));
    return true;

  } else {
    return false;
  }
}



bool FullBuddyListCollector::fillData (const std::string& path,
    const Str2StrMap& cookies, const Str2StrMap& parameter,
    TemplateDictionary& dict) {
	int uid = GetUserId(cookies, parameter);

  if (uid <= 0) {
    MCE_WARN("FullBuddyListCollector::fillData --> uid:" << uid << " path:" << path);
    return false;
  }

  string ticket = GetTicket(cookies, parameter);
	int video_view_power = PageCacheManagerI::instance().GetVideoViewPower();
	
	IceUtil::Mutex::Lock lock(mutex_[uid/10 % kLockSize]);

  ActiveUserPtr au = ServiceI::instance().locateObject<ActiveUserPtr> (AU, uid);
  if (au->checkTicket(ticket, path.find("/wap_") == 0)) {

    dict.SetValue("UID", boost::lexical_cast<string>(uid));
    dict.SetValue("USER_NAME", au->name());
    vector<BuddyPtr> bl;
		try{
			au->getFullBuddyList(bl);
		}catch(std::exception& e){
			MCE_WARN("FullBuddyListCollector::fillData --> get buddy list uid" << uid << " error:" << e.what());
		}
    MCE_INFO("FullBuddyListCollector::fillData --> get buddy list uid:" << uid << " size" << bl.size());
    for (size_t i = 0; i < bl.size(); ++i) {
      TemplateDictionary* buddy_dict = dict.AddSectionDictionary( "BUDDY_SECTION");
      buddy_dict->SetValue("VIDEO_VIEW_POWER", boost::lexical_cast<string>(video_view_power));
      buddy_dict->SetValue("BUDDY_TINY_URL", bl.at(i)->tinyUrl());
      buddy_dict->SetValue("BUDDY_ONLINE_STATUS", boost::lexical_cast<string>( bl.at(i)->onlineStatus()));
      buddy_dict->SetValue("BUDDY_NAME", bl.at(i)->name());
      //buddy_dict->SetValue("BUDDY_DOING", bl.at(i)->doing());
      buddy_dict->SetValue("BUDDY_ID", boost::lexical_cast<string>( bl.at(i)->id()));

			MyUtil::StrSeq buddygroups = bl.at(i)->buddyGroup();
			for(MyUtil::StrSeq::iterator bgit = buddygroups.begin(); bgit != buddygroups.end(); ++bgit){
        //				MCE_INFO("GET IT!    id :" << bl.at(i)->id() << " gname:" << *bgit);
				TemplateDictionary* buddy_group_dict = buddy_dict->AddSectionDictionary("BUDDY_GROUP");
				buddy_group_dict->SetValue("BUDYGROUPNAME", *bgit);
			}
    }

    dict.SetValue("BUDDY_COUNT", boost::lexical_cast<string>(bl.size()));
    return true;

  } else {
    return false;
  }
}


bool OnlineBuddyCountCollector::fillData(const std::string& path,
    const Str2StrMap& cookies, const Str2StrMap& parameter,
    TemplateDictionary& dict) {
  int uid = GetUserId(cookies, parameter);
  if (uid <= 0) {
    MCE_WARN("OnlineBuddyListCollector::fillData --> uid:" << uid << " path:" << path);
    return false;
  }

  string ticket = GetTicket(cookies, parameter);

  IceUtil::Mutex::Lock lock(mutex_[uid/10 % kLockSize]);

  ActiveUserPtr au = ServiceI::instance().locateObject<ActiveUserPtr> (AU, uid);
  if (au->checkTicket(ticket)) {

    dict.SetValue("UID", boost::lexical_cast<string>(uid));
    dict.SetValue("USER_NAME", au->name());
    dict.SetValue("USER_TINY_URL", au->tiny_url());

    Ice::Int buddy_count = au->getBuddyCount();
		MCE_INFO("OnlineBuddyListCollector::fillData --> uid:" << uid << " count:" << buddy_count);
    dict.SetValue("BUDDY_COUNT", boost::lexical_cast<string>(buddy_count));
    return true;

  } else {
    return false;
  }
}

bool OnlineToolbarCountsCollector::fillData(const std::string& path,
    const Str2StrMap& cookies, const Str2StrMap& parameter,
    TemplateDictionary& dict) {

  int uid = GetUserId(cookies, parameter);
  if (uid <= 0) {
    MCE_WARN("OnlineBuddyListCollector::fillData --> uid:" << uid << " path:" << path);
    return false;
  }

  string ticket = GetTicket(cookies, parameter);

  IceUtil::Mutex::Lock lock(mutex_[uid/10 % kLockSize]);

  ActiveUserPtr au = ServiceI::instance().locateObject<ActiveUserPtr> (AU, uid);
  if (au->checkTicket(ticket)) {
    int offlineMessageCount = 0;
    int unreadReplyCount = 0;
    int unreadFeedCount = 0;
    int buddyApplyCount = 0;
    int notifyCount = 0;

    try {
      offlineMessageCount
          = MessageHolderAdapter::instance().getOfflineMessageCount(
              au->userId());
    } catch (Ice::Exception& e) {
      MCE_WARN("OnlineToolbarCountsCollector::fillData-->MessageHolderAdapter::getOfflineMessageCount-->" << e);
    } catch (std::exception& e) {
      MCE_WARN("ResponseAction --> get offline message count err"<< e.what());
    }
    try {
      unreadReplyCount = 0;//NotifyRplAdapter::instance().getCount(au->userId(),-1,true);
    } catch (...) {
      MCE_WARN("ResponseAction --> get reply count err");
    };

    try {
      xce::usercount::UserCounterPtr uc =
          xce::usercount::adapter::UserCountAdapter::instance().getAll(
              au->userId());
      int cc = 0;
      for (size_t i = 0; i < uc->values.size(); ++i) {
        if (uc->types.at(i) == xce::usercount::CGuestRequest) {
          buddyApplyCount = uc->values.at(i);
          if (++cc == 2) {
            break;
          }
        }
        if (uc->types.at(i) == xce::usercount::CNotify) {
          notifyCount = uc->values.at(i);
          if (++cc == 2) {
            break;
          }
        }
      }
    } catch (Ice::Exception & e) {
      MCE_WARN("OnlineToolbarCountsCollector::fillData-->UserCountAdapter::getAll-->"<<e);
    };

    dict.SetValue("OFFLINE_MSG_COUNT", boost::lexical_cast<string>(
        offlineMessageCount));
    dict.SetValue("REPLY_COUNT", boost::lexical_cast<string>(unreadReplyCount));
    dict.SetValue("FEED_COUNT", boost::lexical_cast<string>(unreadFeedCount));
    dict.SetValue("BUDDY_APPLY_COUNT", boost::lexical_cast<string>(
        buddyApplyCount));
    dict.SetValue("NOTIFY_COUNT", boost::lexical_cast<string>(notifyCount));

    return true;

  } else {
    return false;
  }
}



bool UserInfoCollector2::fillData(const std::string& path,
		const Str2StrMap& cookies, const Str2StrMap& parameter,
		TemplateDictionary& dict) {

	int uid = GetUserId(cookies, parameter);

  if (uid <= 0) {
    MCE_WARN("UserInfoCollector::fillData --> uid:" << uid << " path:" << path);
    return false;
  }

  Str2StrMap::const_iterator it = parameter.find("uids");
  if (it == parameter.end())
		return false;

	MCE_INFO("UserInfoCollector::fillData--> uids:" << it->second);
	vector<string> uidlist = StrUtil::split(it->second, ",");

  string ticket = GetTicket(cookies, parameter);
	int video_view_power = PageCacheManagerI::instance().GetVideoViewPower();
	
	IceUtil::Mutex::Lock lock(mutex_[uid/10 % kLockSize]);

  ActiveUserPtr au = ServiceI::instance().locateObject<ActiveUserPtr> (AU, uid);
  if (au->checkTicket(ticket, path.find("/wap_") == 0)) {

    vector<BuddyPtr> bl;
		int online_count = 0;
		try{
			if (!au->getUserInfoList(uidlist, online_count, bl))
				return false;

		}catch(std::exception& e){
			MCE_WARN("UserInfoCollector::fillData --> get buddy list uid" << uid << " error:" << e.what());
		}
    MCE_INFO("UserInfoCollector::fillData --> get buddy list uid:" << uid << " size:" << bl.size());
    for (size_t i = 0; i < bl.size(); ++i) {
      TemplateDictionary* buddy_dict = dict.AddSectionDictionary( "BUDDY_SECTION");
      buddy_dict->SetValue("VIDEO_VIEW_POWER", boost::lexical_cast<string>(video_view_power));
      buddy_dict->SetValue("BUDDY_TINY_URL", bl.at(i)->tinyUrl());
      buddy_dict->SetValue("BUDDY_ONLINE_STATUS", boost::lexical_cast<string>( bl.at(i)->onlineStatus()));
      buddy_dict->SetValue("BUDDY_NAME", bl.at(i)->name());
      buddy_dict->SetValue("BUDDY_DOING", bl.at(i)->doing());
      buddy_dict->SetValue("BUDDY_ID", boost::lexical_cast<string>( bl.at(i)->id()));

			MyUtil::StrSeq buddygroups = bl.at(i)->buddyGroup();
			for(MyUtil::StrSeq::iterator bgit = buddygroups.begin(); bgit != buddygroups.end(); ++bgit){
				//MCE_INFO("GET IT!    id :" << bl.at(i)->id() << " gname:" << *bgit);
				TemplateDictionary* buddy_group_dict = buddy_dict->AddSectionDictionary("BUDDY_GROUP");
				buddy_group_dict->SetValue("BUDYGROUPNAME", *bgit);
			}
    }

    //dict.SetValue("BUDDY_COUNT", boost::lexical_cast<string>(online_count));
    return true;

  } else {
    return false;
  }
	
}

//add by guoqing.liu : 传入一个userId 返回这个用户的在线状态，头像链接，姓名
bool UserInfoCollector::fillData(const std::string& path,
                                 const Str2StrMap& cookies, const Str2StrMap& parameter,
                                 TemplateDictionary& dict) {
  int uid = GetUserId(cookies, parameter);
  if (uid <= 0) {
    MCE_WARN("UserInfoCollector::fillData --> uid:" << uid << " path:" << path);
    return false;
  }
  string ticket = GetTicket(cookies, parameter);
  int targetuser = GetTargetUserId(parameter);
  if(-1 == targetuser || !targetuser){
    MCE_WARN("UserInfoCollector::fillData --> targetuser error");
    return false;
  }
  IceUtil::Mutex::Lock lock(mutex_[uid/10 % kLockSize]);
  ActiveUserPtr au = ServiceI::instance().locateObject<ActiveUserPtr> (AU, uid);
  if (au->checkTicket(ticket, path.find("/wap_") == 0)) {
    dict.SetValue("TARGETUSER_ID", boost::lexical_cast<string>(targetuser));
    TalkUserPtr uPtr;
    try {
      uPtr = TalkCacheClient::instance().GetUserByIdWithLoad(targetuser);
    } catch (Ice::Exception& e) {
      MCE_WARN("UserInfoCollector::fillData --> call TalkCacheClient err : " <<e);
    }
    UserOnlineTypeSeq user_online_type;
    try{
      MyUtil::IntSeq uids;
      uids.push_back(targetuser);
      user_online_type = TalkFunStateAdapter::instance().getOnlineUsers(uids);
    }catch(Ice::Exception& e){
      MCE_WARN("UserInfoCollector::fillData --> call TalkFunState err : " <<e);
    }
    if(!user_online_type.empty()){
      UserOnlineTypeSeq::iterator it=user_online_type.begin();
      MCE_INFO("UserInfoCollector::fillData --> targetuser = " << targetuser << " onlinetype = " << (*it)->type);
      dict.SetValue("TARGETUSER_ONLINE_TYPE", boost::lexical_cast<string>((*it)->type));
    }
    else{
      dict.SetValue("TARGETUSER_ONLINE_TYPE", "0");
    }
    if(uPtr){
      string username = uPtr->name;
      string tinyurl = uPtr->tinyurl;
      dict.SetValue("TARGETUSER_TINY_URL", tinyurl);
      dict.SetValue("TARGETUSER_NAME", username);
      return true;
    }
    return false;
  } else {
    return false;
  }
}

