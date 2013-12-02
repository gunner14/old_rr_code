#ifndef __UserLogicI_h__
#define __UserLogicI_h__

#include "CompleteUserInfoAdapter.h"
#include <Ice/Ice.h>
#include <IceUtil/AbstractMutex.h>
#include <IceUtil/Mutex.h>
#include "UserLogic.h"
#include "TaskManager.h"
#include "Singleton.h"
#include "FriendOfFriendIndexerAdapter.h"
#include <CallGuarantee/src/CallGuarantee.h>

namespace mop {

namespace hi {

namespace svc {

namespace model {

const int TASK_LEVEL_USERSTATECACHE = 1;
const int TASK_LEVEL_PRESENCEMSG = 2;
const int TASK_LEVEL_IM = 3;
const int TASK_LEVEL_BUDDYCACHEBYNAME = 4;

const int TASK_LEVEL_BUDDYBYIDLOADER = 5;
const int TASK_LEVEL_BUDDYBYNAMELOADER = 6;
const int TASK_LEVEL_USERCACHERELOADER = 7;
const int TASK_LEVEL_STATUSCACHE = 8;
const int TASK_LEVEL_USERCACHE = 11;
const int TASK_LEVEL_CHARITY = 12;
const int TASK_LEVEL_PHOTOSTATEFILTER = 13;
const int TASK_LEVEL_SCORECACHEVIP = 14;
const int TASK_LEVEL_USERSTATEFILTER = 15;
const int TASK_LEVEL_FRIENDOFFREIENDINDEXER = 16;
const int TASK_LEVEL_INVITEREWARD = 17;
const int TASK_LEVEL_HIGHSCHOOLFILTER = 18;
const int TASK_LEVEL_FRIENDFINDERNEWCOMER = 19;
const int TASK_LEVEL_USERCOUNT = 20;
const int TASK_LEVEL_COMPLETEUSERINFO = 21;
const int TASK_LEVEL_DISTBUDDYBYIDCACHERELOADER = 22;
const int TASK_LEVEL_FOOTPRINTNOTIFY = 23;
const int TASK_LEVEL_USERCONFIGRELOAD = 24;
const int TASK_LEVEL_USERNETWORKTRIPODLOAD = 34;
const int TASK_LEVEL_USERCOLLEGETRIPODLOAD = 35;
const int TASK_LEVEL_USERUNIVERSITYTRIPODLOAD = 36;
const int TASK_LEVEL_USERWORKPLACERIPODLOAD = 37;
const int TASK_LEVEL_USERREGIONTRIPODLOAD = 38;
const int TASK_LEVEL_USERHIGHSCHOOLTRIPODLOAD = 39;
const int TASK_LEVEL_USERJUNIORHIGHSCHOOLTRIPODLOAD = 40;
const int TASK_LEVEL_USERELEMENTARYSCHOOLTRIPODLOAD = 41;
const int TASK_LEVEL_USERBORNRELOAD = 25;
const int TASK_LEVEL_USERDOINGRELOAD = 26;
const int TASK_LEVEL_USERLOGINCOUNTRELOAD = 27;
const int TASK_LEVEL_USERNAMERELOAD = 28;
const int TASK_LEVEL_USERPASSPORTRELOAD = 29;
const int TASK_LEVEL_USERRIGHTRELOAD = 30;
const int TASK_LEVEL_USERSTAGERELOAD = 31;
const int TASK_LEVEL_USERSTATERELOAD = 32;
const int TASK_LEVEL_USERURLRELOAD = 33;
const int TASK_LEVEL_USER3GNOTIFY = 34;
const int TASK_LEVEL_ACCOUNTFILTERNOTIFY = 35;


const string USER_LOGIC_MANAGER = "M";
const string MODIFY_RECORD_DB = "modify_record";

class UserLogicManagerI: virtual public UserLogicManager,
		virtual public MyUtil::Singleton < UserLogicManagerI> {
public:
	virtual bool isValid(const Ice::Current& = Ice::Current());
	virtual void setValid(bool state, const Ice::Current& = Ice::Current());

	//旧接口
	virtual void updateUser(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());


	virtual void updateUserDesc(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	virtual void updateUserNetwork(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	//user_doing
	virtual void insertUserDoing(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	virtual void updateUserDoing(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	//user_stage
	virtual void insertUserStage(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	virtual void updateUserStage(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	//user_born
	virtual void insertUserBorn(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	virtual void updateUserBorn(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	//user_state
	virtual void insertUserState(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	virtual void updateUserState(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	//user_names
	virtual void insertUserNames(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	virtual void updateUserNames(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	//user_passport
	virtual void insertUserPassport(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	virtual void updateUserPassport(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	//user_config
	virtual void insertUserConfig(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	virtual void updateUserConfig(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	//user_url
	virtual void insertUserUrl(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	virtual void updateUserUrl(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	//user_time
	virtual void insertUserTime(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	virtual void updateUserTime(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	//user_contact
	virtual void insertUserContact(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	virtual void updateUserContact(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	//user_fav
	virtual void insertUserFav(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	virtual void updateUserFav(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	//user_right
	virtual void insertUserRight(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	virtual void updateUserRight(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	//user_logincount
	virtual void insertUserLoginCount(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	virtual void updateUserLoginCount(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	virtual void updateUserRegion(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	virtual void updateUserWorkplace(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	virtual void updateUserElementarySchool(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	virtual void updateUserJuniorHighSchool(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	virtual void updateUserHighSchool(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());
	
	virtual void updateUserCollege(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	virtual void updateUserUniversity(::Ice::Int, const ::MyUtil::Str2StrMap&,
			const Ice::Current& = Ice::Current());

	UserLogicManagerI() {}
	virtual ~UserLogicManagerI(){}
private:
	bool valid_;
	IceUtil::RWRecMutex validmutex_;
};

class PresenceMsgNotifyTask : public MyUtil::Task {
public:
	PresenceMsgNotifyTask(int userId, const MyUtil::Str2StrMap& props, const Ice::Current& current):Task(TASK_LEVEL_PRESENCEMSG), _userId(userId),
		_props(props),_current(current) {
	}
	virtual ~PresenceMsgNotifyTask(){}
	virtual void handle();
private:
	int _userId;
	MyUtil::Str2StrMap _props;
	Ice::Current _current;
};

class IMNotifyTask : public MyUtil::Task
{
public:
	IMNotifyTask(int userId,const MyUtil::Str2StrMap& props, const Ice::Current& current):Task(TASK_LEVEL_IM), _userId(userId){
		_props = props;
		_current = current;
	};
	virtual void handle();
private:
	int _userId;
	MyUtil::Str2StrMap _props;
	Ice::Current _current;
	
};
class StatusCacheNotifyTask : public MyUtil::Task
{
public:
	StatusCacheNotifyTask(int userId,const MyUtil::Str2StrMap& props, const Ice::Current& current):Task(TASK_LEVEL_STATUSCACHE), _userId(userId){
		_props = props;
		_current = current;
	};
	virtual void handle();
private:
	int _userId;
	MyUtil::Str2StrMap _props;
	Ice::Current _current;
	
};
class BuddyByIdLoaderNotifyTask : public MyUtil::Task
{
public:
	BuddyByIdLoaderNotifyTask (int userId,const MyUtil::Str2StrMap& props, const Ice::Current& current):Task(TASK_LEVEL_BUDDYBYIDLOADER), _userId(userId){
		_props = props;
		_current = current;
	};
	virtual void handle();
private:
	int _userId;
	MyUtil::Str2StrMap _props;
	Ice::Current _current;
	
};
class BuddyByNameLoaderNotifyTask : public MyUtil::Task
{
public:
	BuddyByNameLoaderNotifyTask (int userId,const MyUtil::Str2StrMap& props, const Ice::Current& current):Task(TASK_LEVEL_BUDDYBYNAMELOADER), _userId(userId){
		_props = props;
		_current = current;
	};
	virtual void handle();
private:
	int _userId;
	MyUtil::Str2StrMap _props;
	Ice::Current _current;
	
};

class FriendOfFriendIndexerNotifyTask : public MyUtil::Task
{
public:
 	 FriendOfFriendIndexerNotifyTask (int userId,const MyUtil::Str2StrMap& props, const Ice::Current& current):Task(TASK_LEVEL_FRIENDOFFREIENDINDEXER), _userId(userId){
		_props = props;
                _current = current;
	};
	virtual void handle();
private:
	int _userId;
	Ice::Current _current;
	MyUtil::Str2StrMap _props;
};

class  PhotoStateFilterUpdateNotifyTask : public MyUtil::Task
{
public:
        PhotoStateFilterUpdateNotifyTask (int userId,const MyUtil::Str2StrMap& props):
		Task(TASK_LEVEL_PHOTOSTATEFILTER), _userId(userId), _props(props){
        };
        virtual void handle();
private:
        int _userId;
        MyUtil::Str2StrMap _props;
        
};

class  ScoreCacheVipNotifyTask : public MyUtil::Task
{
public:
        ScoreCacheVipNotifyTask (int userId,const MyUtil::Str2StrMap& props):
		Task(TASK_LEVEL_SCORECACHEVIP), _userId(userId), _props(props){
        };
        virtual void handle();
private:
        int _userId;
        MyUtil::Str2StrMap _props;
        
};

class  UserStateFilterUpdateNotifyTask : public MyUtil::Task
{
public:
        UserStateFilterUpdateNotifyTask (int userId,const MyUtil::Str2StrMap& props):
		Task(TASK_LEVEL_USERSTATEFILTER), _userId(userId), _props(props){
        };
        virtual void handle();
private:
        int _userId;
        MyUtil::Str2StrMap _props;
        
};

class  UserCountNotifyTask : public MyUtil::Task
{
public:
        UserCountNotifyTask (int userId):
		Task(TASK_LEVEL_USERCOUNT), _userId(userId){
        };
        virtual void handle();
private:
        int _userId;
        
};

class  InviteRewardUpdateNotifyTask : public MyUtil::Task
{
public:
        InviteRewardUpdateNotifyTask (int userId,const MyUtil::Str2StrMap& props):
		Task(TASK_LEVEL_INVITEREWARD), _userId(userId), _props(props){
        };
        virtual void handle();
private:
        int _userId;
        MyUtil::Str2StrMap _props;
        
};

class  HighSchoolUpdateNotifyTask : public MyUtil::Task
{
public:
        HighSchoolUpdateNotifyTask (int userId,const MyUtil::Str2StrMap& props):
		Task(TASK_LEVEL_HIGHSCHOOLFILTER), _userId(userId), _props(props){
        };
        virtual void handle();
private:
        int _userId;
        MyUtil::Str2StrMap _props;
        
};

class CharityNotifyTask : public MyUtil::Task {
public:
	CharityNotifyTask(int userid,const MyUtil::Str2StrMap& data) :
		MyUtil::Task(TASK_LEVEL_CHARITY), userid_(userid), data_(data) {}

	virtual void handle();
private:
	int userid_;
	MyUtil::Str2StrMap data_;
};

class UserCacheInsertNotifyTask : public MyUtil::Task {
public:
	UserCacheInsertNotifyTask(int userid) :
		MyUtil::Task(TASK_LEVEL_USERCACHE), userid_(userid) {}

	virtual void handle();
private:
	int userid_;
};

class UserCacheUpdateNotifyTask : public MyUtil::Task {
public:
	UserCacheUpdateNotifyTask(int userid, Ice::Byte table, const MyUtil::Str2StrMap& data) :
		MyUtil::Task(TASK_LEVEL_USERCACHE), userid_(userid), table_(table), data_(data) {}

	virtual void handle();
private:
	int userid_;
	Ice::Byte table_;
	MyUtil::Str2StrMap data_;
};

class UserCacheInsertCallGuarantee : public callguarantee::CallGuarantee,
	public MyUtil::Singleton<UserCacheInsertCallGuarantee> {
public:
	UserCacheInsertCallGuarantee() {}
	virtual bool Do(const std::vector<std::pair<int, Ice::ObjectPtr> >& params);
	virtual bool Redo(const std::vector<int>& ids);
};

class UserCacheUpdateCallGuarantee : public callguarantee::CallGuarantee,
	public MyUtil::Singleton<UserCacheUpdateCallGuarantee> {
public:
	UserCacheUpdateCallGuarantee() {}
	virtual bool Do(const std::vector<std::pair<int, Ice::ObjectPtr> >& params);
	virtual bool Redo(const std::vector<int>& ids);
};

class UserCacheReloadCallGuarantee : public callguarantee::CallGuarantee,
	public MyUtil::Singleton<UserCacheReloadCallGuarantee> {
public:
	UserCacheReloadCallGuarantee() {}
	virtual bool Do(const std::vector<std::pair<int, Ice::ObjectPtr> >& params);
	virtual bool Redo(const std::vector<int>& ids);
};

class FriendFinderNewComerUpdateNotifyTask : public MyUtil::Task {
public:
	FriendFinderNewComerUpdateNotifyTask(int userid) :
		MyUtil::Task(TASK_LEVEL_FRIENDFINDERNEWCOMER), userid_(userid) {}

	virtual void handle();
private:
	int userid_;
};

class CompleteUserInfoTask : public MyUtil::Task {
public:

	CompleteUserInfoTask(int userId) :
		MyUtil::Task(TASK_LEVEL_COMPLETEUSERINFO), userId_(userId) {}
	virtual void handle();
private:
	int userId_;
};

class FootprintNotifyTask : public MyUtil::Task {
public:

	FootprintNotifyTask(int userId) :
		MyUtil::Task(TASK_LEVEL_FOOTPRINTNOTIFY), userId_(userId) {}
	virtual void handle();
private:
	int userId_;
};

class DistBuddyByIdCacheReloaderNotifyTask : public MyUtil::Task {
public:
  DistBuddyByIdCacheReloaderNotifyTask(int userid, const MyUtil::Str2StrMap& m) : MyUtil::Task(TASK_LEVEL_DISTBUDDYBYIDCACHERELOADER), userid_(userid), m_(m) {}
  virtual void handle();
private:
  int userid_;
  MyUtil::Str2StrMap m_;
};


class UserConfigReloadTask : public MyUtil::Task {
public:
	UserConfigReloadTask(int userId) :
		MyUtil::Task(TASK_LEVEL_USERCONFIGRELOAD), userId_(userId) {}

	virtual void handle();
private:
	int userId_;
};

class UserNetworkTripodLoadTask : public MyUtil::Task {
 public:
  UserNetworkTripodLoadTask(int userId) :
  MyUtil::Task(TASK_LEVEL_USERNETWORKTRIPODLOAD), userId_(userId) {}

  virtual void handle();
 private:
  int userId_;
};

class UserCollegeTripodLoadTask : public MyUtil::Task {
 public:
  UserCollegeTripodLoadTask(int userId) :
  MyUtil::Task(TASK_LEVEL_USERCOLLEGETRIPODLOAD), userId_(userId) {}

  virtual void handle();
 private:
  int userId_;
};

class UserUniversityTripodLoadTask : public MyUtil::Task {
 public:
  UserUniversityTripodLoadTask(int userId) :
  MyUtil::Task(TASK_LEVEL_USERUNIVERSITYTRIPODLOAD), userId_(userId) {}

  virtual void handle();
 private:
  int userId_;
};

class UserWorkplaceTripodLoadTask : public MyUtil::Task {
 public:
  UserWorkplaceTripodLoadTask(int userId) :
  MyUtil::Task(TASK_LEVEL_USERWORKPLACERIPODLOAD), userId_(userId) {}

  virtual void handle();
 private:
  int userId_;
};

class UserRegionTripodLoadTask : public MyUtil::Task {
 public:
  UserRegionTripodLoadTask(int userId) :
  MyUtil::Task(TASK_LEVEL_USERREGIONTRIPODLOAD), userId_(userId) {}

  virtual void handle();
 private:
  int userId_;
};

class UserHighschoolTripodLoadTask : public MyUtil::Task {
 public:
  UserHighschoolTripodLoadTask(int userId) :
  MyUtil::Task(TASK_LEVEL_USERHIGHSCHOOLTRIPODLOAD), userId_(userId) {}

  virtual void handle();
 private:
  int userId_;
};

class UserJuniorhighschoolTripodLoadTask : public MyUtil::Task {
 public:
  UserJuniorhighschoolTripodLoadTask(int userId) :
  MyUtil::Task(TASK_LEVEL_USERJUNIORHIGHSCHOOLTRIPODLOAD), userId_(userId) {}

  virtual void handle();
 private:
  int userId_;
};

class UserElementaryTripodLoadTask : public MyUtil::Task {
 public:
  UserElementaryTripodLoadTask(int userId) :
  MyUtil::Task(TASK_LEVEL_USERELEMENTARYSCHOOLTRIPODLOAD), userId_(userId) {}

  virtual void handle();
 private:
  int userId_;
};

class UserBornReloadTask : public MyUtil::Task {
public:
	UserBornReloadTask(int userId) :
		MyUtil::Task(TASK_LEVEL_USERBORNRELOAD), userId_(userId) {}
	virtual void handle();
private:
	int userId_;
};

class UserDoingReloadTask : public MyUtil::Task {
public:
	UserDoingReloadTask(int userId) :
		MyUtil::Task(TASK_LEVEL_USERDOINGRELOAD), userId_(userId) {}
	virtual void handle();
private:
	int userId_;
};

class UserLoginCountReloadTask : public MyUtil::Task {
public:
	UserLoginCountReloadTask(int userId) :
		MyUtil::Task(TASK_LEVEL_USERLOGINCOUNTRELOAD), userId_(userId) {}
	virtual void handle();
private:
	int userId_;
};

class UserNameReloadTask : public MyUtil::Task {
public:
	UserNameReloadTask(int userId) :
		MyUtil::Task(TASK_LEVEL_USERNAMERELOAD), userId_(userId) {}
	virtual void handle();
private:
	int userId_;
};

class UserPassportReloadTask : public MyUtil::Task {
public:
	UserPassportReloadTask(int userId) :
		MyUtil::Task(TASK_LEVEL_USERPASSPORTRELOAD), userId_(userId) {}
	virtual void handle();
private:
	int userId_;
};

class UserRightReloadTask : public MyUtil::Task {
public:
	UserRightReloadTask(int userId) :
		MyUtil::Task(TASK_LEVEL_USERRIGHTRELOAD), userId_(userId) {}
	virtual void handle();
private:
	int userId_;
};

class UserStageReloadTask : public MyUtil::Task {
public:
	UserStageReloadTask(int userId) :
		MyUtil::Task(TASK_LEVEL_USERSTAGERELOAD), userId_(userId) {}
	virtual void handle();
private:
	int userId_;
};

class UserStateReloadTask : public MyUtil::Task {
public:
	UserStateReloadTask(int userId) :
		MyUtil::Task(TASK_LEVEL_USERSTATERELOAD), userId_(userId) {}
	virtual void handle();
private:
	int userId_;
};

class UserUrlReloadTask : public MyUtil::Task {
public:
	UserUrlReloadTask(int userId) :
		MyUtil::Task(TASK_LEVEL_USERURLRELOAD), userId_(userId) {}
	virtual void handle();
private:
	int userId_;
};

class User3GNotifyTask : public MyUtil::Task {
public:
	User3GNotifyTask(const int user_id, const int table, const MyUtil::Str2StrMap& props) :
		MyUtil::Task(TASK_LEVEL_USER3GNOTIFY), user_id_(user_id), table_(table), props_(props) {}
	virtual void handle();
private:
	int user_id_;
	int table_;
  MyUtil::Str2StrMap props_;
};

class AccountFilterNotifyTask : public MyUtil::Task {
  public:
    AccountFilterNotifyTask(const int id, const MyUtil::Str2StrMap& props) :
      MyUtil::Task(TASK_LEVEL_ACCOUNTFILTERNOTIFY), id_(id), props_(props) {}
    virtual void handle();
  private:
    int id_;
    MyUtil::Str2StrMap props_;
};

}}}} //namespace mop::hi::svc::model

#endif
