#ifndef __COM_RENREN_DATASYNC_USERSYNCADAPTER_H__ 
#define __COM_RENREN_DATASYNC_USERSYNCADAPTER_H__ 

#include "Singleton.h"
#include "LocalProducerManagerAdapter.h"

namespace com {
namespace renren {
namespace datasync {

using MyUtil::Singleton;

//UserConfig
//--------------------------------------------------------------
class UserConfigSyncAdapter : public Singleton<UserConfigSyncAdapter>,
                            public SingleLocalProducerManagerAdapter<int> {
public:
  UserConfigSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("user_config", "user_config") {}
};

//UserDesc
//--------------------------------------------------------------
class UserTimeSyncAdapter : public Singleton<UserTimeSyncAdapter>,
                            public SingleLocalProducerManagerAdapter<int> {
public:
  UserTimeSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("user_time", "user_time") {}
};

class UserContactSyncAdapter : public Singleton<UserContactSyncAdapter>,
                              public SingleLocalProducerManagerAdapter<int> {
public:
  UserContactSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("user_contact", "user_contact") {}
};

//UserBase
//--------------------------------------------------------------
class UserPassportSyncAdapter : public Singleton<UserPassportSyncAdapter>,
                                public SingleLocalProducerManagerAdapter<int> {
public:
  UserPassportSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("user_passport", "user_passport") {}
};

class UserNamesSyncAdapter : public Singleton<UserNamesSyncAdapter>,
                            public SingleLocalProducerManagerAdapter<int> {
public:
  UserNamesSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("user_names", "user_names") {}
};

class UserRightSyncAdapter : public Singleton<UserRightSyncAdapter>,
                             public SingleLocalProducerManagerAdapter<int> {
public:
  UserRightSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("user_right", "user_right") {}
};

class UserStateSyncAdapter : public Singleton<UserStateSyncAdapter>,
                             public SingleLocalProducerManagerAdapter<int> {
public:
  UserStateSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("user_state", "user_state") {}
};

class UserLoginCountSyncAdapter : public Singleton<UserLoginCountSyncAdapter>,
                                  public SingleLocalProducerManagerAdapter<int> {
public:
  UserLoginCountSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("user_logincount", "user_logincount") {}
};

class UserBornSyncAdapter : public Singleton<UserBornSyncAdapter>,
                            public SingleLocalProducerManagerAdapter<int> {
public:
  UserBornSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("user_born", "user_born") {}
};

class UserStageSyncAdapter : public Singleton<UserStageSyncAdapter>,
                             public SingleLocalProducerManagerAdapter<int> {
public:
  UserStageSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("user_stage", "user_stage") {}
};

class UserDoingSyncAdapter : public Singleton<UserDoingSyncAdapter>,
                             public SingleLocalProducerManagerAdapter<int> {
public:
  UserDoingSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("user_doing", "user_doing") {}
};

class UserUrlSyncAdapter : public Singleton<UserUrlSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<int> {
public:
  UserUrlSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("user_url", "user_url") {}
};

//UserNetwork
//--------------------------------------------------------------
class NetworkHistorySyncAdapter : public Singleton<NetworkHistorySyncAdapter>,
                           public SingleLocalProducerManagerAdapter<int> {
public:
  NetworkHistorySyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("common", "network_history") {}
};

class HighSchoolInfoSyncAdapter : public Singleton<HighSchoolInfoSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<int> {
public:
  HighSchoolInfoSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("common", "high_school_info") {}
};

class UniversityInfoSyncAdapter : public Singleton<UniversityInfoSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<int> {
public:
  UniversityInfoSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("common", "university_info") {}
};

class WorkplaceInfoSyncAdapter : public Singleton<WorkplaceInfoSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<int> {
public:
  WorkplaceInfoSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("common", "workplace_info") {}
};


class CollegeInfoSyncAdapter : public Singleton<CollegeInfoSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<int> {
public:
  CollegeInfoSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("common", "college_info") {}
};


class ElementarySchoolInfoSyncAdapter : public Singleton<ElementarySchoolInfoSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<int> {
public:
  ElementarySchoolInfoSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("common", "elementary_school_info") {}
};


class JuniorHighSchoolInfoSyncAdapter : public Singleton<JuniorHighSchoolInfoSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<int> {
public:
  JuniorHighSchoolInfoSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("common", "junior_high_school_info") {}
};


class RegionInfoSyncAdapter : public Singleton<RegionInfoSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<int> {
public:
  RegionInfoSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("common", "region_info") {}
};

//VipInfo
//--------------------------------------------------------------
class VipIconSyncAdapter : public Singleton<VipIconSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<int> {
public:
  VipIconSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("vipmember", "icon_url") {}
};

class VipHatSyncAdapter : public Singleton<VipHatSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<int> {
public:
  VipHatSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("vipmember", "feed_icon_url") {}
};

class VipMemberSyncAdapter : public Singleton<VipMemberSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<int> {
public:
  VipMemberSyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("vipmember", "user_member_info") {}
};

//Ticket
//--------------------------------------------------------------
class TTicketLoadSyncAdapter : public Singleton<TTicketLoadSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<std::string> {
public:
  TTicketLoadSyncAdapter() :
      SingleLocalProducerManagerAdapter<std::string> ("tripod_ticket", "t_ticket", 60*60*6) {}
};

class TTicketDelSyncAdapter: public Singleton<TTicketDelSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<std::string> {
public:
  TTicketDelSyncAdapter() :
      SingleLocalProducerManagerAdapter<std::string> ("tripod_ticket", "t_ticket", 0, true) {}
};

class PTicketLoadSyncAdapter : public Singleton<PTicketLoadSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<std::string> {
public:
  PTicketLoadSyncAdapter() :
      SingleLocalProducerManagerAdapter<std::string> ("tripod_ticket", "p_ticket", 60*60*24*30) {}
};

class PTicketDelSyncAdapter: public Singleton<PTicketDelSyncAdapter>,
                           public SingleLocalProducerManagerAdapter<std::string> {
public:
  PTicketDelSyncAdapter() :
      SingleLocalProducerManagerAdapter<std::string> ("tripod_ticket", "p_ticket", 0, true) {}
};

/*

class TASyncAdapter : public Singleton<TASyncAdapter>,
                           public SingleLocalProducerManagerAdapter<int> {
public:
  TASyncAdapter() :
      SingleLocalProducerManagerAdapter<int> ("common", "TB") {}
};

*/


//--------------------------------------------------------------

}}} // namespace com::renren::datasync

#endif
