#include "UserBaseI.h"
#include "LogWrapper.h"
#include "BatchUpdaterAdapter.h"
#include "UserLogicAdapter.h"
#include "Date.h"
#include "QueryRunner.h"
#include "RpcFilterManager.h"


#include "time.h"

#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"

#include "util/cpp/TimeCost.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "boost/lexical_cast.hpp"

#include "UserPassportReaderAdapter.h"
#include "UserNameReaderAdapter.h"
#include "UserStateReaderAdapter.h"
#include "UserRightReaderAdapter.h"
#include "UserLoginCountReaderAdapter.h"
#include "UserUrlReaderAdapter.h"
#include "UserBornReaderAdapter.h"
#include "UserStageReaderAdapter.h"
#include "UserDoingReaderAdapter.h"

#include "UserPassportTripodAdapter.h"
#include "UserNameTripodAdapter.h"
#include "UserStateTripodAdapter.h"
#include "UserRightTripodAdapter.h"
#include "UserLoginCountTripodAdapter.h"
#include "UserUrlTripodAdapter.h"
#include "UserBornTripodAdapter.h"
#include "UserStageTripodAdapter.h"
#include "UserDoingTripodAdapter.h"

using namespace com::xiaonei::xce;
using namespace mop::hi::svc::adapter;
using namespace xce::userbase;
using namespace MyUtil;
using namespace xce::serverstate;

using xce::adapter::userpassport::UserPassportReaderAdapter;
using xce::adapter::username::UserNameReaderAdapter;
using xce::adapter::userstate::UserStateReaderAdapter;
using xce::adapter::userright::UserRightReaderAdapter;
using xce::adapter::userlogincount::UserLoginCountReaderAdapter;
using xce::adapter::userurl::UserUrlReaderAdapter;
using xce::adapter::userborn::UserBornReaderAdapter;
using xce::adapter::userstage::UserStageReaderAdapter;
using xce::adapter::userdoing::UserDoingReaderAdapter;

using xce::adapter::userpassport::UserPassportTripodAdapter;
using xce::adapter::username::UserNameTripodAdapter;
using xce::adapter::userstate::UserStateTripodAdapter;
using xce::adapter::userright::UserRightTripodAdapter;
using xce::adapter::userlogincount::UserLoginCountTripodAdapter;
using xce::adapter::userurl::UserUrlTripodAdapter;
using xce::adapter::userborn::UserBornTripodAdapter;
using xce::adapter::userstage::UserStageTripodAdapter;
using xce::adapter::userdoing::UserDoingTripodAdapter;
//-------------------------UserPassportManagerI-------------------------------
/* the new instialize
 */
void MyUtil::initialize()
{
	ServiceI& service=ServiceI::instance();


	service.registerObjectCache(USER_PASSPORT, "UP", new UserPassportFactoryI, false);
	service.registerObjectCache(USER_NAME, "UN", new UserNameFactoryI, false);
	service.registerObjectCache(USER_RIGHT, "UR", new UserRightFactoryI, false);
	service.registerObjectCache(USER_LOGINCOUNT, "ULC", new UserLoginCountFactoryI, false);
	service.registerObjectCache(USER_STATE, "US", new UserStateFactoryI, false);
	service.registerObjectCache(USER_URL, "UU", new UserUrlFactoryI, false);
	service.registerObjectCache(USER_BORN, "UBO", new UserBornFactoryI, false);
	service.registerObjectCache(USER_STAGE, "USG", new UserStageFactoryI, false);
	service.registerObjectCache(USER_DOING, "UD", new UserDoingFactoryI, false);
	
	service.getAdapter()->add(&UserPassportManagerI::instance(), service.createIdentity(
			USER_PASSPORT_MANAGER, ""));
	service.getAdapter()->add(&UserNameManagerI::instance(), service.createIdentity(
			USER_NAME_MANAGER, ""));
	service.getAdapter()->add(&UserRightManagerI::instance(), service.createIdentity(
			USER_RIGHT_MANAGER, ""));
	service.getAdapter()->add(&UserStateManagerI::instance(), service.createIdentity(
			USER_STATE_MANAGER, ""));
	service.getAdapter()->add(&UserLoginCountManagerI::instance(), service.createIdentity(
			USER_LOGINCOUNT_MANAGER, ""));

	service.getAdapter()->add(&UserBaseViewManagerI::instance(), service.createIdentity(
			USER_BASEVIEW_MANAGER, ""));
	
	service.getAdapter()->add(&UserUrlManagerI::instance(), service.createIdentity(USER_URL_MANAGER, ""));
	service.getAdapter()->add(&UserBasicManagerI::instance(),service.createIdentity(USER_BASIC_MANAGER, ""));
	
	service.getAdapter()->add(&UserBornManagerI::instance(),service.createIdentity(USER_BORN_MANAGER, ""));
	service.getAdapter()->add(&UserStageManagerI::instance(),service.createIdentity(USER_STAGE_MANAGER, ""));
	service.getAdapter()->add(&UserDoingManagerI::instance(),service.createIdentity(USER_DOING_MANAGER, ""));

//用来setdata的task 	
    TaskManager::instance().config(TASK_LEVEL_FILL, MyUtil::ThreadPoolConfig(1, 100));
    TaskManager::instance().config(TASK_LEVEL_LOAD, MyUtil::ThreadPoolConfig(1, 10));
	
//-----------------------------------HA---------------------------------------------
	int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("UserBase.Mod");
	int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("UserBase.Interval",5);
	
	map<string,Ice::ObjectPtr> mapNameAndService;
	
	mapNameAndService.insert(make_pair<string,Ice::ObjectPtr>(USER_PASSPORT_MANAGER,&UserPassportManagerI::instance()));
	mapNameAndService.insert(make_pair<string,Ice::ObjectPtr>(USER_NAME_MANAGER,&UserNameManagerI::instance()));
	mapNameAndService.insert(make_pair<string,Ice::ObjectPtr>(USER_RIGHT_MANAGER,&UserRightManagerI::instance()));
	mapNameAndService.insert(make_pair<string,Ice::ObjectPtr>(USER_STATE_MANAGER,&UserStateManagerI::instance()));
	mapNameAndService.insert(make_pair<string,Ice::ObjectPtr>(USER_LOGINCOUNT_MANAGER,&UserLoginCountManagerI::instance()));
	mapNameAndService.insert(make_pair<string,Ice::ObjectPtr>(USER_URL_MANAGER,&UserUrlManagerI::instance()));
	mapNameAndService.insert(make_pair<string,Ice::ObjectPtr>(USER_BASIC_MANAGER,&UserBasicManagerI::instance()));
	
	mapNameAndService.insert(make_pair<string,Ice::ObjectPtr>(USER_BORN_MANAGER,&UserBornManagerI::instance()));
	mapNameAndService.insert(make_pair<string,Ice::ObjectPtr>(USER_STAGE_MANAGER,&UserStageManagerI::instance()));
	mapNameAndService.insert(make_pair<string,Ice::ObjectPtr>(USER_DOING_MANAGER,&UserDoingManagerI::instance()));

	mapNameAndService.insert(make_pair<string,Ice::ObjectPtr>(USER_BASEVIEW_MANAGER,&UserBaseViewManagerI::instance()));
	
	ServerStateSubscriber::instance().initialize("ControllerUserBase",mapNameAndService,mod,interval);

	{
		int status = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("UserBase.Status",0);
		
		ServerStateSubscriber::instance().setStatus(status,USER_PASSPORT_MANAGER);
		ServerStateSubscriber::instance().setStatus(status,USER_NAME_MANAGER);
		ServerStateSubscriber::instance().setStatus(status,USER_RIGHT_MANAGER);
		ServerStateSubscriber::instance().setStatus(status,USER_STATE_MANAGER);
		ServerStateSubscriber::instance().setStatus(status,USER_LOGINCOUNT_MANAGER);
		ServerStateSubscriber::instance().setStatus(status,USER_URL_MANAGER);
		ServerStateSubscriber::instance().setStatus(status,USER_BASIC_MANAGER);
		ServerStateSubscriber::instance().setStatus(status,USER_BORN_MANAGER);
		ServerStateSubscriber::instance().setStatus(status,USER_STAGE_MANAGER);
		ServerStateSubscriber::instance().setStatus(status,USER_DOING_MANAGER);
		
		ServerStateSubscriber::instance().setStatus(status);
		
		std::ostringstream os;
		os<<"MyUtil::initialize set status : "<<status;

		MCE_WARN( os.str() );
	}
  MyUtil::RpcFilterManager::instance().initialize("UserBase");
  ServiceMonitorManager::instance().start();
}

//--------------------------------------------------------------------------------

void UserPassportManagerI::createUserPassport(const Str2StrMap& props,
		                                      const Ice::Current& current)
{
	InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);
	ostringstream inMsg;
	inMsg<<"UserPassportManagerI::createUserPassport" << " " << current.con->toString().substr(current.con->toString().find("remote",25));
	TimeCost tc = TimeCost::create(inMsg.str());

	
	int id;
	Str2StrMap::const_iterator it = props.find(CUPID);
	if(it!=props.end())
	{
		id=boost::lexical_cast<Ice::Int>(it->second);
	}else
	{
		MCE_WARN("UserPassportManagerI::createUserPassport CreateUserPassportException::Can find userID in props");
		return;
	}

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, id);
	
		QueryRunner( DB_USER_PASSPORT , CDbWServer ).execute( Statement::buildInsert( TABLE_USER_PASSPORT, props ) );
	
	
		ostringstream idMsg;
		idMsg<<" id="<<id;
		tc.step(idMsg.str());
	
		MCE_INFO("UserPassportManagerI::createUserPassport Loading " << id);
		
		UserPassportDataIPtr rs = new UserPassportDataI ;
		Statement sql;
		sql << "SELECT id, account, domain, password_md5, status, safe_status FROM user_passport WHERE id = '"<<id<<"'";
		BatchPassportResHandlerI handler(rs);
		size_t resultSize = QueryRunner(DB_USER_PASSPORT, CDbWServer ).query(sql, handler);
		
		if (resultSize == 0) {
			MCE_WARN("UserPassportManagerI::createUserPassport CreateUserPassportException::Can not load new added user ,userid  [" << id << "]");
			return ;
		}
		
		if (rs->uID != id) {
			MCE_WARN("UserPassportManagerI::createUserPassport CreateUserPassportException::UserId error,userid = " << id << ", resultID = "<< rs->uID);
			return ;
		}
	
		MCE_INFO( "UserPassportManagerI::createUserPassport Loading " << id << " done.");
	
		ServiceI::instance().addObject(rs , USER_PASSPORT, id);
		UserPassportReaderAdapter::instance().setData(id, rs->getData());
	}
	
  UserPassportTripodAdapter::instance().Remove(id);
	UserLogicAdapter::instance().insertUserPassport(id, props);
}

UserPassportDataPtr UserPassportManagerI::getUserPassport(Ice::Int uid,
                                            		   const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);
  
	bool flag = MyUtil::RpcFilterManager::instance().filter(current, "user_passport");

	if (!flag) {
	  MCE_WARN("UserPassportManagerI::getUserPassport filter account and passwordmd5");
	  UserPassportDataPtr data = (m_getUserPassport(uid))->getData();
	  data->account = "";
	  data->passwordmd5 = "";
	  return data;
	} else {
		return (m_getUserPassport(uid))->getData();
	}
}

void UserPassportManagerI::setUserPassport(Ice::Int uid,
		                                   const MyUtil::Str2StrMap& props, 
										   const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	
	//去掉map中的用户id
	Str2StrMap::iterator it = const_cast<Str2StrMap&>(props).find("ID");
	if(it!=props.end())
	{
		const_cast<Str2StrMap&>(props).erase(it);
	}

	Str2StrMap filter;
	filter[CUPID] = boost::lexical_cast<string>(uid);

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, uid);
		QueryRunner( DB_USER_PASSPORT, CDbWServer ).execute( Statement::buildUpdate( TABLE_USER_PASSPORT , filter ,  props ) ); 
		m_getUserPassport(uid)->setProperties(props);

		UserPassportReaderAdapter::instance().setUserPassport(uid, props);
	}

  UserPassportTripodAdapter::instance().Remove(uid);
	UserLogicAdapter::instance().updateUserPassport(uid, props, current.ctx);
}

//reload for us
void UserPassportManagerI::reload(Ice::Int userId, const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, userId);
		ServiceI::instance().removeObject(USER_PASSPORT, userId);
		UserPassportDataIPtr data = ServiceI::instance().locateObject<UserPassportDataIPtr> (USER_PASSPORT, userId);
		if(data)
		{
			UserPassportReaderAdapter::instance().setData(userId, data->getData());
		}
	}

}

Ice::Int UserPassportManagerI::getUserIdByEmail(const string& email, const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"email: " << email;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);
	
	int result = -1;
	Statement sql;
	sql << "SELECT id FROM user_passport WHERE account = " << mysqlpp::quote << email;
	UserEmailResultHandlerI handler(result);
	QueryRunner(DB_USER_PASSPORT, CDbWServer).query(sql, handler);
	MCE_INFO("UserPassportManagerI::getUserIdByEmail(" << email<< ") -> id: "<<result);
	return result;

}


UserPassportDataIPtr UserPassportManagerI::m_getUserPassport(int userId)
{
	if (isValid()) {
		UserPassportDataIPtr res = ServiceI::instance().findObject<UserPassportDataIPtr>(USER_PASSPORT, userId);
		if (res) {
			return res;
		} else {
			TaskManager::instance().execute(new LoadTask(LOAD_TYPE_USERPASSPORT, userId));
			MCE_WARN("UserPassportManagerI::m_getUserPassport NoSuchUserPassportException:: reason - cache miss [" << userId << "]");
			throw NoSuchObjectException();
		}
	} else {
		try{
			return ServiceI::instance().locateObject<UserPassportDataIPtr>(USER_PASSPORT, userId);
		} catch(const MyUtil::NoSuchObjectException& e) {
			MCE_WARN("UserPassportManagerI::m_getUserPassport NoSuchUserPassportException:: reason - NoSuchUserPassportException [" << userId << "]");
			throw NoSuchObjectException();
		}
	}
}

//-------------------------UserNameManagerI-------------------------------

void UserNameManagerI::createUserName(const Str2StrMap& props,
		                              const Ice::Current& current)
{
	InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);
	
	ostringstream inMsg;
	inMsg<<"UserNameManagerI::createUserName" <<  " " << current.con->toString().substr(current.con->toString().find("remote",25));
	TimeCost tc = TimeCost::create(inMsg.str());
	
	int id;
	Str2StrMap::const_iterator it = props.find(CUNID);
	if(it!=props.end())
	{
		id=boost::lexical_cast<Ice::Int>(it->second);
	}else
	{
		MCE_WARN("UserNameManagerI::createUserName CreateUserNameException::Can find userID in props");
		return;
	}

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, id);
		QueryRunner( DB_USER_NAME , CDbWServer ).execute( Statement::buildInsert( TABLE_USER_NAME, props ) );
		
		ostringstream idMsg;
		idMsg<<" id="<<id;
		tc.step(idMsg.str());
		
		MCE_INFO("UserNameManagerI::createUserName Loading " << id);
		
		
		UserNameDataIPtr rs = new UserNameDataI ;
		BatchNameResHandlerI handler(rs);
		Statement sql;
		sql << "SELECT id, name, nickname FROM user_names WHERE id = '"<<id<<"'";
		size_t resultSize = QueryRunner(DB_USER_NAME, CDbWServer ).query(sql,handler);
		
		if (resultSize == 0) {
			MCE_WARN("UserNameManagerI::createUserName CreateUserNameException::Can not load new added user ,userid  [" << id << "]");
			return ;
		}
		if (rs->uID != id) {
			MCE_WARN("UserNameManagerI::createUserName CreateUserNameException::UserId error,userid = " << id << ", resultID = "<< rs->uID);
			return ;
		}
	
		MCE_INFO( "UserNameManagerI::createUserName Loading " << id << " done.");
	
		ServiceI::instance().addObject(rs , USER_NAME, id);

		UserNameReaderAdapter::instance().setData(id, rs->getData());
	}
  UserNameTripodAdapter::instance().Remove(id);
	UserLogicAdapter::instance().insertUserNames(id, props);
}

UserNameDataPtr UserNameManagerI::getUserName(Ice::Int uid,
                                           const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);
	
	return (m_getUserName(uid))->getData();
}

void UserNameManagerI::setUserName(Ice::Int uid,
		                           const MyUtil::Str2StrMap& props, 
							       const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	
	//去掉map中的用户id
	Str2StrMap::iterator it = const_cast<Str2StrMap&>(props).find("ID");
	if(it!=props.end())
	{
		const_cast<Str2StrMap&>(props).erase(it);
	}

	Str2StrMap filter;
	filter[CUNID] = boost::lexical_cast<string>(uid);
	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, uid);
		QueryRunner( DB_USER_NAME, CDbWServer ).execute( Statement::buildUpdate( TABLE_USER_NAME , filter ,  props ) );
		m_getUserName(uid)->setProperties(props);

		UserNameReaderAdapter::instance().setUserName(uid, props);
	}
	
  UserNameTripodAdapter::instance().Remove(uid);
	UserLogicAdapter::instance().updateUserNames(uid, props, current.ctx);
}

//reload for us
void UserNameManagerI::reload(Ice::Int userId, const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, userId);
		ServiceI::instance().removeObject(USER_NAME, userId);
		UserNameDataIPtr data = ServiceI::instance().locateObject<UserNameDataIPtr>(USER_NAME, userId);

		if(data)
		{
			UserNameReaderAdapter::instance().setData(userId, data->getData());
		}
	}

}

UserNameDataIPtr UserNameManagerI::m_getUserName(int userId)
{
	if (isValid()) {
		UserNameDataIPtr res = ServiceI::instance().findObject<UserNameDataIPtr>(USER_NAME, userId);
		if (res) {
			return res;
		} else {
			TaskManager::instance().execute(new LoadTask(LOAD_TYPE_USERNAMES, userId));
			MCE_WARN("UserNameManagerI::m_getUserName NoSuchUserNameException:: reason - cache miss [" << userId << "]");
			throw NoSuchObjectException();
		}
	} else {
		try{
			return ServiceI::instance().locateObject<UserNameDataIPtr>(USER_NAME, userId);
		} catch(const MyUtil::NoSuchObjectException& e) {
			MCE_WARN("UserNameManagerI::m_getUserName NoSuchUserNameException:: reason - NoSuchUserNameException [" << userId << "]");
			throw NoSuchObjectException();
		}
	}
}

//-------------------------UserStateManagerI-------------------------------

//--------------------------------------------------------------------------------

void UserStateManagerI::createUserState(const Str2StrMap& props,
		                              const Ice::Current& current)
{
	InvokeClient ic = InvokeClient::create(current,"",InvokeClient::INVOKE_LEVEL_INFO);
	
	ostringstream inMsg;
	inMsg<<"UserStateManagerI::createUserState" <<  " " << current.con->toString().substr(current.con->toString().find("remote",25));
	TimeCost tc = TimeCost::create(inMsg.str());
	
	
	int id;
	Str2StrMap::const_iterator it = props.find(CUSID);
	if(it!=props.end())
	{
		id=boost::lexical_cast<Ice::Int>(it->second);
	}else
	{
		MCE_WARN("UserStateManagerI::createUserState CreateUserStateException::Can find userID in props");
		return;
	}

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, id);
		QueryRunner( DB_USER_STATE , CDbWServer ).execute( Statement::buildInsert( TABLE_USER_STATE, props ) );
		
		ostringstream idMsg;
		idMsg<<" id="<<id;
		tc.step(idMsg.str());
		
		MCE_INFO("UserStateManagerI::createUserState Loading " << id);
		
		UserStateDataIPtr rs = new UserStateDataI ;
		BatchStateResHandlerI handler(rs);
		Statement sql;
		sql << "SELECT id, state, star, level FROM user_state WHERE id = '"<<id<<"'";
		size_t resultSize = QueryRunner(DB_USER_STATE, CDbWServer ).query(sql, handler);
		
		if (resultSize == 0) {
			MCE_WARN("UserStateManagerI::createUserState CreateUserStateException::Can not load new added user ,userid  [" << id << "]");
			return ;
		}
		if (rs->id != id) {
			MCE_WARN("UserStateManagerI::createUserState CreateUserStateException::UserId error,userid = " << id << ", resultID = "<< rs->id);
			return ;
		}
	
		MCE_INFO( "UserStateManagerI::createUserState Loading " << id << " done.");
		ServiceI::instance().addObject(rs , USER_STATE, id);

		UserStateReaderAdapter::instance().setData(id, rs->getData());
	}
  UserStateTripodAdapter::instance().Remove(id);
	UserLogicAdapter::instance().insertUserState(id, props);
}

UserStateDataPtr UserStateManagerI::getUserState(Ice::Int uid,
                                           const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);
	
	return (m_getUserState(uid))->getOldData();
}

UserStateDataNPtr UserStateManagerI::getUserStateN(Ice::Int uid,
                                           const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);
	
	return (m_getUserState(uid))->getData();
}

void UserStateManagerI::setUserState(Ice::Int uid,
		                           const MyUtil::Str2StrMap& props, 
							       const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	
	//去掉map中的用户id
	Str2StrMap::iterator it = const_cast<Str2StrMap&>(props).find("ID");
	if(it!=props.end())
	{
		const_cast<Str2StrMap&>(props).erase(it);
	}
	
	Str2StrMap filter;
	filter[CUSID] = boost::lexical_cast<string>(uid);
	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, uid);
		QueryRunner( DB_USER_STATE, CDbWServer ).execute( Statement::buildUpdate( TABLE_USER_STATE , filter ,  props ) );
		m_getUserState(uid)->setProperties(props);

		UserStateReaderAdapter::instance().setUserState(uid, props);
	}

  UserStateTripodAdapter::instance().Remove(uid);
	UserLogicAdapter::instance().updateUserState(uid, props, current.ctx);
}

//reload for us
void UserStateManagerI::reload(Ice::Int userId, const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, userId);
		ServiceI::instance().removeObject(USER_STATE, userId);
		UserStateDataIPtr data = ServiceI::instance().locateObject<UserStateDataIPtr>(USER_STATE, userId);

		if(data)
		{
			UserStateReaderAdapter::instance().setData(userId, data->getData());
		}
	}

}

void UserStateManagerI::setStateOn(Ice::Int userId, Ice::Int pos, const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, userId);
		Statement sql;
		sql << "UPDATE user_state SET state = state | " << (1L << pos) << " WHERE " << CUSID << " = '"<<userId<<"' LIMIT 1";
		QueryRunner(DB_USER_STATE, CDbWServer).execute(sql);
		m_getUserState(userId)->setStateOn(pos);

		UserStateReaderAdapter::instance().setStateOn(userId, pos);
	}
	
	Str2StrMap props;
	props[CUSSTATE] = boost::lexical_cast<string>(m_getUserState(userId)->getState());
	
  UserStateTripodAdapter::instance().Remove(userId);
	UserLogicAdapter::instance().updateUserState(userId,props);
}

void UserStateManagerI::setStateOff(Ice::Int userId, Ice::Int pos, const Ice::Current& current) 
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, userId);

		Statement sql;
		sql << "UPDATE user_state SET state = state & (~" << (1L << pos) << ") WHERE " << CUSID << " = '"<<userId<<"' LIMIT 1";
		QueryRunner(DB_USER_STATE, CDbWServer).execute(sql);
		m_getUserState(userId)->setStateOff(pos);
		
		UserStateReaderAdapter::instance().setStateOff(userId, pos);
	}
	
	Str2StrMap props;
	props[CUSSTATE] = boost::lexical_cast<string>(m_getUserState(userId)->getState());

  UserStateTripodAdapter::instance().Remove(userId);
	UserLogicAdapter::instance().updateUserState(userId,props);
}

UserStateDataIPtr UserStateManagerI::m_getUserState(int userId)
{
	if (isValid()) {
		UserStateDataIPtr res = ServiceI::instance().findObject<UserStateDataIPtr>(USER_STATE, userId);
		if (res) {
			return res;
		} else {
			TaskManager::instance().execute(new LoadTask(LOAD_TYPE_USERSTATE, userId));
			MCE_WARN("UserStateManagerI::m_getUserState NoSuchUserStateException:: reason - cache miss [" << userId << "]");
			throw NoSuchObjectException();
		}
	} else {
		try{
			return ServiceI::instance().locateObject<UserStateDataIPtr>(USER_STATE, userId);
		} catch(const MyUtil::NoSuchObjectException& e) {
			MCE_WARN("UserStateManagerI::m_getUserState NoSuchUserStateException:: reason - NoSuchUserStateException [" << userId << "]");
			throw NoSuchObjectException();
		}
	}
}

//-------------------------UserRightManagerI-------------------------------

//--------------------------------------------------------------------------------

void UserRightManagerI::createUserRight(const Str2StrMap& props,
		                              const Ice::Current& current)
{
	InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);
	
	ostringstream inMsg;
	inMsg<<"UserRightManagerI::createUserRight" <<  " " << current.con->toString().substr(current.con->toString().find("remote",25));
	TimeCost tc = TimeCost::create(inMsg.str());
	
	int id;
	Str2StrMap::const_iterator it = props.find(CURID);
	if(it!=props.end())
	{
		id=boost::lexical_cast<Ice::Int>(it->second);
	}else
	{
		MCE_WARN("UserRightManagerI::createUserRight CreateUserRightException::Can find userID in props");
		return;
	}

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, id);
		QueryRunner( DB_USER_RIGHT , CDbWServer ).execute( Statement::buildInsert( TABLE_USER_RIGHT, props ) );
		
		ostringstream idMsg;
		idMsg<<" id="<<id;
		tc.step(idMsg.str());
	
		MCE_INFO("UserRightManagerI::createUserRight Loading " << id);
		
		UserRightDataIPtr rs = new UserRightDataI ;
		BatchRightResHandlerI handler(rs);
		Statement sql;
		sql << "SELECT id, auth, common_rights FROM user_right WHERE id = '"<<id <<"'";
		size_t resultSize = QueryRunner(DB_USER_RIGHT, CDbWServer ).query(sql,handler);
		
		if (resultSize == 0) {
			MCE_WARN("UserRightManagerI::createUserRight CreateUserRightException::Can not load new added user ,userid  [" << id << "]");
			return ;
		}
		
		if (rs->uID != id) {
			MCE_WARN("UserRightManagerI::createUserRight CreateUserRightException::UserId error,userid = " << id << ", resultID = "<< rs->uID);
			return ;
		}
	
		MCE_INFO( "UserRightManagerI::createUserRight Loading " << id << " done.");
		ServiceI::instance().addObject(rs , USER_RIGHT, id);

		UserRightReaderAdapter::instance().setData(id, rs->getData());
	}
  UserRightTripodAdapter::instance().Remove(id);
	UserLogicAdapter::instance().insertUserRight(id, props);
}

UserRightDataPtr UserRightManagerI::getUserRight(Ice::Int uid,
                                           const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);
	
	return (m_getUserRight(uid))->getData();
}

void UserRightManagerI::setUserRight(Ice::Int uid,
		                           const MyUtil::Str2StrMap& props, 
							       const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	
	//去掉map中的用户id
	Str2StrMap::iterator it = const_cast<Str2StrMap&>(props).find("ID");
	if(it!=props.end())
	{
		const_cast<Str2StrMap&>(props).erase(it);
	}
	
	Str2StrMap filter;
	filter[CURID] = boost::lexical_cast<string>(uid);
	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, uid);
		QueryRunner( DB_USER_RIGHT, CDbWServer ).execute( Statement::buildUpdate( TABLE_USER_RIGHT , filter ,  props ) );
		m_getUserRight(uid)->setProperties(props);

		UserRightReaderAdapter::instance().setUserRight(uid, props);
	}

  UserRightTripodAdapter::instance().Remove(uid);
	UserLogicAdapter::instance().updateUserRight(uid, props, current.ctx);
}

void UserRightManagerI::reload(Ice::Int userId, const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, userId);
		ServiceI::instance().removeObject(USER_RIGHT, userId);
		UserRightDataIPtr data = ServiceI::instance().locateObject<UserRightDataIPtr>(USER_RIGHT, userId);

		if(data)
		{
			UserRightReaderAdapter::instance().setData(userId, data->getData());
		}
	}

}

UserRightDataIPtr UserRightManagerI::m_getUserRight(int userId)
{
	if (isValid()) {
		UserRightDataIPtr res = ServiceI::instance().findObject<UserRightDataIPtr>(USER_RIGHT, userId);
		if (res) {
			return res;
		} else {
			TaskManager::instance().execute(new LoadTask(LOAD_TYPE_USERRIGHT, userId));
			MCE_WARN("UserRightManagerI::m_getUserRight NoSuchUserRightException:: reason - cache miss [" << userId << "]");
			throw NoSuchObjectException();
		}
	} else {
		try{
			return ServiceI::instance().locateObject<UserRightDataIPtr>(USER_RIGHT, userId);
		} catch(const MyUtil::NoSuchObjectException& e) {
			MCE_WARN("UserRightManagerI::m_getUserRight NoSuchUserRightException:: reason - NoSuchUserRightException [" << userId << "]");
			throw NoSuchObjectException();
		}
	}
}


//-------------------------UserLoginCountManagerI-------------------------------

//--------------------------------------------------------------------------------

void UserLoginCountManagerI::createUserLoginCount(const Str2StrMap& props,
		                              const Ice::Current& current)
{
	InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);
	
	ostringstream inMsg;
	inMsg<<"UserLoginCountManagerI::createUserLoginCount" <<  " " << current.con->toString().substr(current.con->toString().find("remote",25));
	TimeCost tc = TimeCost::create(inMsg.str());
	
	
	int id;
	Str2StrMap::const_iterator it = props.find(CULCID);
	if(it!=props.end())
	{
		id=boost::lexical_cast<Ice::Int>(it->second);
	}else
	{
		MCE_WARN("UserLoginCountManagerI::createUserLoginCount CreateUserLoginCountException::Can find userID in props");
		return;
	}

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, id);
		QueryRunner( DB_USER_LOGINCOUNT , CDbWServer ).execute( Statement::buildInsert( TABLE_USER_LOGINCOUNT, props ) );
		
		ostringstream idMsg;
		idMsg<<" id="<<id;
		tc.step(idMsg.str());
		
		MCE_INFO("UserLoginCountManagerI::createUserLoginCount Loading " << id);
		
		UserLoginCountDataIPtr rs = new UserLoginCountDataI ;
		BatchLoginCountResHandlerI handler(rs);
		Statement sql;
		sql << "SELECT id, logincount FROM user_logincount WHERE id = '"<<id<<"'";
		size_t resultSize = QueryRunner(DB_USER_LOGINCOUNT, CDbWServer ).query(sql,handler);
		
		if (resultSize == 0) {
			MCE_WARN("UserLoginCountManagerI::createUserLoginCount CreateUserLoginCountException::Can not load new added user ,userid  [" << id << "]");
			rs->uID = id;
			rs->logincount = 0;
			return ;
		}
		if (rs->uID != id) {
			MCE_WARN("UserLoginCountManagerI::createUserLoginCount CreateUserLoginCountException::UserId error,userid = " << id << ", resultID = "<< rs->uID);
			rs->uID = id;
			rs->logincount = 0;
		}
	
	
		MCE_INFO( "UserLoginCountManagerI::createUserLoginCount Loading " << id << " done.");
		ServiceI::instance().addObject(rs , USER_LOGINCOUNT, id);
		
		UserLoginCountReaderAdapter::instance().setData(id, rs->getData());
	}
}

int UserLoginCountManagerI::getUserLoginCount(Ice::Int uid,
                                           const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return (m_getUserLoginCount(uid))->getLogincount();
}

void UserLoginCountManagerI::incUserLoginCount(Ice::Int uid,
							       const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	
	mop::hi::svc::adapter::BatchUpdaterAdapter::instance().incLoginCount(uid,1);
	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, uid);
		m_getUserLoginCount(uid)->incLogincount();

		UserLoginCountReaderAdapter::instance().incUserLoginCount(uid);
		UserLoginCountTripodAdapter::instance().Inc(uid, 1);
	}
}


//reload for us
void UserLoginCountManagerI::reload(Ice::Int userId, const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, userId);
		ServiceI::instance().removeObject(USER_LOGINCOUNT, userId);
		UserLoginCountDataIPtr data = ServiceI::instance().locateObject<UserLoginCountDataIPtr>(USER_LOGINCOUNT, userId);

		if(data)
		{
			UserLoginCountReaderAdapter::instance().setData(userId, data->getData());
		}
	}

}

UserLoginCountDataIPtr UserLoginCountManagerI::m_getUserLoginCount(int userId)
{
	if (isValid()) {
		UserLoginCountDataIPtr res = ServiceI::instance().findObject<UserLoginCountDataIPtr>(USER_LOGINCOUNT, userId);
		if (res) {
			return res;
		} else {
			TaskManager::instance().execute(new LoadTask(LOAD_TYPE_USERLOGINCOUNT, userId));
			MCE_WARN("UserLoginCountManagerI::m_getUserLoginCount NoSuchUserLoginCountException:: reason - cache miss [" << userId << "]");
			throw NoSuchObjectException();
		}
	} else {
		try{
			return ServiceI::instance().locateObject<UserLoginCountDataIPtr>(USER_LOGINCOUNT, userId);
		} catch(const MyUtil::NoSuchObjectException& e) {
			MCE_WARN("UserLoginCountManagerI::m_getUserLoginCount NoSuchUserLoginCountException:: reason - NoSuchUserLoginCountException [" << userId << "]");
			throw NoSuchObjectException();
		}
	}
}



//----------------UserUrlManager--------------------
UserUrlDataPtr UserUrlManagerI::getUserUrl(Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg<<"id="<<id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);
	
	return m_getUserUrl(id)->getOldData();
}

UserUrlDataNPtr UserUrlManagerI::getUserUrlN(Ice::Int id, const Ice::Current& current) {
	ostringstream inMsg;
	inMsg<<"id="<<id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);
	
	return m_getUserUrl(id)->getData();
}

void UserUrlManagerI::setUserUrl(Ice::Int id, const MyUtil::Str2StrMap& props, const Ice::Current & current) {
	ostringstream inMsg;
	inMsg<<"id="<<id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	
	//去掉map中的用户id
	Str2StrMap::iterator it = const_cast<Str2StrMap&>(props).find("ID");
	if(it!=props.end())
	{
		const_cast<Str2StrMap&>(props).erase(it);
	}
	
	MyUtil::Str2StrMap filter;
	filter["id"] = boost::lexical_cast<string>(id);
	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, id);
		
		QueryRunner(DB_USER_URL,CDbWServer).execute(Statement::buildUpdate(TABLE_USER_URL, filter, props));
		UserUrlDataIPtr result = m_getUserUrl(id);
		result->setProperities(props);

		UserUrlReaderAdapter::instance().setUserUrl(id, props);
	}
  UserUrlTripodAdapter::instance().Remove(id);
	UserLogicAdapter::instance().updateUserUrl(id, props, current.ctx);
}

void UserUrlManagerI::createUserUrl(const MyUtil::Str2StrMap& props, const Ice::Current & current) 
{
	InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);
	
	ostringstream inMsg;
	inMsg<<"UserUrlManagerI::createUserUrl" <<  " " << current.con->toString().substr(current.con->toString().find("remote",25));
	TimeCost tc = TimeCost::create(inMsg.str());

	int id;
	Str2StrMap::const_iterator it = props.find("ID");
	if(it!=props.end())
	{
		id=boost::lexical_cast<Ice::Int>(it->second);
	}else
	{
		MCE_WARN("UserUrlManagerI::createUserUrl CreateUserUrlException::Can find userID in props");
		return;
	}

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, id);
		QueryRunner(DB_USER_URL,CDbWServer).execute(Statement::buildInsert(TABLE_USER_URL, props));
		ostringstream idMsg;
		idMsg<<" id="<<id;
		tc.step(idMsg.str());
		
		MCE_INFO("UserUrlManagerI::createUserUrl Loading " << id);
		UserUrlDataIPtr result = new UserUrlDataI;
		Statement sql;
		sql << "SELECT id, largeurl, mainurl, headurl, tinyurl, flexurl FROM user_url WHERE id = '" << id << "'";
		UserUrlResultHandlerI handler(result);
		size_t resultSize = QueryRunner(DB_USER_URL, CDbWServer).query(sql, handler);
		
		if (resultSize == 0) {
			MCE_WARN("UserUrlManagerI::createUserUrl CreateUserUrlException::Can not load new added user ,userid  [" << id << "]");
			return ;
		}
		if (id != result->id) {
			MCE_WARN("UserUrlManagerI::createUserUrl CreateUserUrlException::UserId error,userid = " << id << ", resultID = "<< result->id);
			return ;
		}
		MCE_INFO("UserUrlManagerI::createUserUrl Loading  " << id << " done.");
		ServiceI::instance().addObject(result, USER_URL, id);

		UserUrlReaderAdapter::instance().setData(id, result->getData());
	}
  UserUrlTripodAdapter::instance().Remove(id);
	UserLogicAdapter::instance().insertUserUrl(id, props);
}

void UserUrlManagerI::reload(Ice::Int id, const Ice::Current & current) {
	ostringstream inMsg;
	inMsg<<"id="<<id;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, id);
		
		MyUtil::ServiceI::instance().removeObject(USER_URL, id);
		UserUrlDataIPtr data = ServiceI::instance().locateObject<UserUrlDataIPtr>(USER_URL, id);

		if(data)
		{
			UserUrlReaderAdapter::instance().setData(id, data->getData());
		}
	}

}

UserUrlDataIPtr UserUrlManagerI::m_getUserUrl(int userId)
{
	if (isValid()) {
		UserUrlDataIPtr res = ServiceI::instance().findObject<UserUrlDataIPtr>(USER_URL, userId);
		if (res) {
			return res;
		} else {
			TaskManager::instance().execute(new LoadTask(LOAD_TYPE_USERURL, userId));
			MCE_WARN("UserUrlManagerI::m_getUserUrl NoSuchUserUrlException:: reason - cache miss [" << userId << "]");
			throw NoSuchObjectException();
		}
	} else {
		try{
			return ServiceI::instance().locateObject<UserUrlDataIPtr>(USER_URL, userId);
		} catch(const MyUtil::NoSuchObjectException& e) {
			MCE_WARN("UserUrlManagerI::m_getUserUrl NoSuchUserUrlException:: reason - NoSuchUserUrlException [" << userId << "]");
			throw NoSuchObjectException();
		}
	}
}
//-------------------------UserBasic---------------------------

UserBasicDataPtr UserBasicManagerI::getUserBasic(Ice::Int userId, const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);
	
	return m_getUserBasic(userId)->getData();
}

UserBasicDataIPtr UserBasicManagerI::m_getUserBasic(int userId) { 
	
	UserBasicDataIPtr result = new UserBasicDataI;
	
	UserPassportDataIPtr  upd;
	UserStateDataIPtr     usd; 
	UserUrlDataIPtr       uud;
	UserNameDataIPtr      und;
	UserBornDataIPtr      ubd;
	UserStageDataIPtr     usgd; 
	UserDoingDataIPtr     udd;

	if (isValid()) {
		upd = ServiceI::instance().findObject<UserPassportDataIPtr> (USER_PASSPORT, userId); 
		usd = ServiceI::instance().findObject<UserStateDataIPtr> (USER_STATE, userId); 
		und = ServiceI::instance().findObject<UserNameDataIPtr> (USER_NAME, userId); 
		uud = ServiceI::instance().findObject<UserUrlDataIPtr> (USER_URL, userId); 
		ubd  = ServiceI::instance().findObject<UserBornDataIPtr> (USER_BORN, userId); 
		usgd = ServiceI::instance().findObject<UserStageDataIPtr> (USER_STAGE, userId); 
		udd = ServiceI::instance().findObject<UserDoingDataIPtr> (USER_DOING, userId);
		if (upd && usd && und && uud && ubd && usgd && udd) {
			upd = upd->getData();
			usd = usd->getData();
			und = und->getData();
			uud = uud->getData();
			ubd = ubd->getData();
			usgd = usgd->getData();
			udd = udd->getData();
		} else {
			TaskManager::instance().execute(new LoadTask(LOAD_TYPE_USERBASIC, userId));
			MCE_WARN("UserBasicManagerI::m_getUserBasic NoSuchUserBasicException:: reason - cache miss [" << userId << "]");
			throw NoSuchObjectException();
		}
	} else {
		try {
			upd = ServiceI::instance().locateObject<UserPassportDataIPtr> (USER_PASSPORT, userId)->getData(); 
		} catch(...) {
			MCE_WARN("UserBasicManagerI::m_getUserBasic NoSuchUserPassportException:: reason - NoSuchUserPassportException [" << userId << "]");
			throw NoSuchObjectException();
	}

		try {
			usd = ServiceI::instance().locateObject<UserStateDataIPtr> (USER_STATE, userId)->getData(); 
		} catch(...) {
			MCE_WARN("UserBasicManagerI::m_getUserBasic NoSuchUserStateException:: reason - NoSuchUserStateException [" << userId << "]");
			throw NoSuchObjectException();
		}
		try {
			und = ServiceI::instance().locateObject<UserNameDataIPtr> (USER_NAME, userId)->getData(); 
		} catch(...) {
			MCE_WARN("UserBasicManagerI::m_getUserBasic NoSuchUserNameException:: reason - NoSuchUserNameException [" << userId << "]");
			throw NoSuchObjectException();
		}
		
		try {	
			uud = ServiceI::instance().locateObject<UserUrlDataIPtr> (USER_URL, userId)->getData(); 
		} catch(...) {
			MCE_WARN("UserBasicManagerI::m_getUserBasic NoSuchUserUrlException:: reason - NoSuchUserUrlException [" << userId << "]");
			throw NoSuchObjectException();
		}
		
		try {
			ubd  = ServiceI::instance().locateObject<UserBornDataIPtr> (USER_BORN, userId)->getData(); 
		} catch(...) {
			MCE_WARN("UserBasicManagerI::m_getUserBasic NoSuchUserBornException:: reason - NoSuchUserBornException [" << userId << "]");
			throw NoSuchObjectException();
		}

		try {
			usgd = ServiceI::instance().locateObject<UserStageDataIPtr> (USER_STAGE, userId)->getData(); 
		} catch(...) {
			MCE_WARN("UserBasicManagerI::m_getUserBasic NoSuchUserStageException:: reason - NoSuchUserStageException [" << userId << "]");
			throw NoSuchObjectException();
		}
	
		try {
			udd = ServiceI::instance().locateObject<UserDoingDataIPtr> (USER_DOING, userId)->getData(); 
		} catch(...) {
			MCE_WARN("UserBasicManagerI::m_getUserBasic NoSuchUserDoingException:: reason - NoSuchUserDoingException [" << userId << "]");
			throw NoSuchObjectException();
		}
	}

	parseData(result,ubd,usgd,udd,und,uud,upd,usd);
		
	return result;
}

void UserBasicManagerI::parseData(UserBasicDataIPtr& desData, const UserBornDataIPtr& bornData, 
		                          const UserStageDataIPtr& stageData, const UserDoingDataIPtr& doingData,
								  const UserNameDataIPtr& nameData, const UserUrlDataIPtr&  urlData, 
								  const UserPassportDataIPtr&  passportData, const UserStateDataIPtr& stateData )
{
	//将userborn中的数据添加到userbasic对象中
	if(bornData)
	{
       desData->id = bornData->id ;
       desData->gender = bornData->gender ;
       desData->birthDay = bornData->birthDay ;
       desData->birthMonth = bornData->birthMonth ;
       desData->birthYear = bornData->birthYear ;
       desData->homeProvince = bornData->homeProvince ;
       desData->homeCity = bornData->homeCity ;
       desData->cityCode = bornData->cityCode ;
       desData->astrology = bornData->astrology ;
	}
	
	//将userstage中的数据添加到userbasic对象中
	if(stageData)
	{
		desData->univ = stageData->univ ;
		desData->stage = stageData->stage ;
	}
	
	//将userdoing中的数据添加到userbasic对象中
	if(doingData)
	{
		desData->doing = doingData->doing;
		desData->doingTime = doingData->doingTime;
	}
	//将username中的数据添加到userbasic对象中
	if(nameData)
	{
       desData->name= nameData->name;
	}
	//将userurl中的数据添加到userbasic对象中
	if(urlData)
	{
       desData->headUrl = urlData->headUrl;
       desData->tinyUrl = urlData->tinyUrl;
	}
	//将userpassport中的数据添加到userbasic对象中
	if(passportData)
	{
       desData->status = passportData->status;
	}
	//将userstate中的数据添加到userbasic对象中
	if(stateData)
	{
       desData->star = stateData->star;
	}
}
//-------------------------UserBorn---------------------------

void UserBornManagerI::createUserBorn(const Str2StrMap& props, const Ice::Current& current )
{
	InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);
	
	ostringstream inMsg;
	inMsg<<"UserBornManagerI::createUserBorn" <<  " " << current.con->toString().substr(current.con->toString().find("remote",25));
	TimeCost tc = TimeCost::create(inMsg.str());
	
	int id;
	Str2StrMap::const_iterator it = props.find("ID");
	if(it!=props.end())
	{
		id=boost::lexical_cast<Ice::Int>(it->second);
	}else
	{
		MCE_WARN("UserBornManagerI::createUserBorn CreateUserBornException::Can find userID in props");
		return;
	}

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, id);
		QueryRunner(DB_USER_BORN, CDbWServer).execute(Statement::buildInsert(TABLE_USER_BORN, props));
		
		ostringstream idMsg;
		idMsg<<" id="<<id;
		tc.step(idMsg.str());
		
		MCE_INFO("UserBornManagerI::createUserBorn Loading " << id);
		Str2StrMap filter;
		filter[COLUMN_ID] = boost::lexical_cast<string>(id);
		UserBornDataIPtr rs = new UserBornDataI ;
		UserBornResultHandlerI handler(rs);
		size_t resultSize = QueryRunner(DB_USER_BORN, CDbWServer).query(Statement::buildSelect(TABLE_USER_BORN, filter), handler);
		
		if (resultSize == 0) {
			MCE_WARN("UserBornManagerI::createUserBorn CreateUserBornException::Can not load new added user ,userid  [" << id << "]");
			return ;
		}
		if (id != rs->id) {
			MCE_WARN("UserBornManagerI::createUserBorn CreateUserBornException::UserId error,userid = " << id << ", resultID = "<< rs->id);
			return ;
		}
		MCE_INFO("UserBornManagerI::createUserBorn Loading " << id << " done.");
		ServiceI::instance().addObject(rs, USER_BORN, id);

		UserBornReaderAdapter::instance().setData(id, rs->getData());
	}
  UserBornTripodAdapter::instance().Remove(id);
	UserLogicAdapter::instance().insertUserBorn(id, props);
}

void UserBornManagerI::reloadUserBorn(Ice::Int userId,const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, userId);
	
		ServiceI::instance().removeObject(USER_BORN, userId);
		UserBornDataIPtr data = ServiceI::instance().locateObject<UserBornDataIPtr> (USER_BORN, userId);

		if(data)
		{
			UserBornReaderAdapter::instance().setData(userId, data->getData());
		}
	}
}

UserBornDataPtr UserBornManagerI::getUserBorn(Ice::Int userId, const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);
	
	return m_getUserBorn(userId)->getOldData();
}

void UserBornManagerI::setUserBorn(Ice::Int userId,const Str2StrMap& props,const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	
	//去掉map中的用户id
	Str2StrMap::iterator it = const_cast<Str2StrMap&>(props).find("ID");
	if(it!=props.end())
	{
		const_cast<Str2StrMap&>(props).erase(it);
	}
	
	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, userId);
		Str2StrMap filter;
		filter[COLUMN_ID] = boost::lexical_cast<string>(userId);
		QueryRunner(DB_USER_BORN, CDbWServer).execute(Statement::buildUpdate(TABLE_USER_BORN, filter, props));
		m_getUserBorn(userId)->setProperties(props);

		UserBornReaderAdapter::instance().setUserBorn(userId, props);
	}
  UserBornTripodAdapter::instance().Remove(userId);
	UserLogicAdapter::instance().updateUserBorn(userId, props,current.ctx);
}

UserBornDataIPtr UserBornManagerI::m_getUserBorn(int userId) {
	if (isValid()) {
		UserBornDataIPtr res = ServiceI::instance().findObject<UserBornDataIPtr> (USER_BORN, userId);
		if (res) {
			return res;
		} else {
			TaskManager::instance().execute(new LoadTask(LOAD_TYPE_USERBORN, userId));
			MCE_WARN("UserBornManagerI::m_getUserBorn NoSuchUserBornException:: reason - cache miss [" << userId << "]");
			throw NoSuchObjectException();
		}
	} else {
		try{
			return ServiceI::instance().locateObject<UserBornDataIPtr> (USER_BORN, userId);
		} catch(const MyUtil::NoSuchObjectException& e) {
			MCE_WARN("UserBornManagerI::m_getUserBorn NoSuchUserBornException:: reason - NoSuchUserBornException [" << userId << "]");
			throw NoSuchObjectException();
		}
	}
}

//-------------------------UserStage---------------------------

void UserStageManagerI::createUserStage(const Str2StrMap& props, const Ice::Current& current )
{
	InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);
	
	ostringstream inMsg;
	inMsg<<"UserStageManagerI::createUserStage" <<  " " << current.con->toString().substr(current.con->toString().find("remote",25));
	TimeCost tc = TimeCost::create(inMsg.str());

	int id;
	Str2StrMap::const_iterator it = props.find("ID");
	if(it!=props.end())
	{
		id=boost::lexical_cast<Ice::Int>(it->second);
	}else
	{
		MCE_WARN("UserStageManagerI::createUserStage CreateUserStageException::Can find userID in props");
		return;
	}

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, id);
		QueryRunner(DB_USER_STAGE, CDbWServer).execute(Statement::buildInsert(TABLE_USER_STAGE, props));
		
		ostringstream idMsg;
		idMsg<<" id="<<id;
		tc.step(idMsg.str());
		
		MCE_INFO("UserStageManagerI::createUserStage Loading " << id);
		Str2StrMap filter;
		filter[COLUMN_ID] = boost::lexical_cast<string>(id);
		UserStageDataIPtr rs = new UserStageDataI ;
		UserStageResultHandlerI handler(rs);
		size_t resultSize = QueryRunner(DB_USER_STAGE, CDbWServer).query(Statement::buildSelect(TABLE_USER_STAGE, filter), handler);
		
		if (resultSize == 0) {
			MCE_WARN("UserStageManagerI::createUserStage CreateUserStageException::Can not load new added user ,userid  [" << id << "]");
			return ;
		}
		if (id != rs->id) {
			MCE_WARN("UserStageManagerI::createUserStage CreateUserStageException::UserId error,userid = " << id << ", resultID = "<< rs->id);
			return ;
		}
		MCE_INFO("UserStageManagerI::createUserStage Loading " << id << " done.");
		ServiceI::instance().addObject(rs, USER_STAGE, id);

		UserStageReaderAdapter::instance().setData(id, rs->getData());
	}
  UserStageTripodAdapter::instance().Remove(id);
	UserLogicAdapter::instance().insertUserStage(id, props);
}

void UserStageManagerI::reloadUserStage(Ice::Int userId,const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, userId);
		ServiceI::instance().removeObject(USER_STAGE, userId);
		UserStageDataIPtr data = ServiceI::instance().locateObject<UserStageDataIPtr> (USER_STAGE, userId);
		if(data)
		{
			UserStageReaderAdapter::instance().setData(userId, data->getData());
		}
	}
}

UserStageDataPtr UserStageManagerI::getUserStage(Ice::Int userId, const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);
	
	return m_getUserStage(userId)->getData();
}

void UserStageManagerI::setUserStage(Ice::Int userId,const Str2StrMap& props,const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	
	//去掉map中的用户id
	Str2StrMap::iterator it = const_cast<Str2StrMap&>(props).find("ID");
	if(it!=props.end())
	{
		const_cast<Str2StrMap&>(props).erase(it);
	}
	
	Str2StrMap filter;
	filter[COLUMN_ID] = boost::lexical_cast<string>(userId);

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, userId);
		QueryRunner(DB_USER_STAGE, CDbWServer).execute(Statement::buildUpdate(TABLE_USER_STAGE, filter, props));
		m_getUserStage(userId)->setProperties(props);

		UserStageReaderAdapter::instance().setUserStage(userId, props);
	}
  UserStageTripodAdapter::instance().Remove(userId);
	UserLogicAdapter::instance().updateUserStage(userId, props,current.ctx);
}

UserStageDataIPtr UserStageManagerI::m_getUserStage(int userId) {
	if (isValid()) {
		UserStageDataIPtr res = ServiceI::instance().findObject<UserStageDataIPtr> (USER_STAGE, userId);
		if (res) {
			return res;
		} else {
			TaskManager::instance().execute(new LoadTask(LOAD_TYPE_USERSTAGE, userId));
			MCE_WARN("UserStageManagerI::m_getUserStage NoSuchUserStageException:: reason - cache miss [" << userId << "]");
			throw NoSuchObjectException();
		}
	} else {
		try{
			return ServiceI::instance().locateObject<UserStageDataIPtr> (USER_STAGE, userId);
		} catch(const MyUtil::NoSuchObjectException& e) {
			MCE_WARN("UserStageManagerI::m_getUserStage NoSuchUserStageException:: reason - NoSuchUserStageException [" << userId << "]");
			throw NoSuchObjectException();
		}
	}
}

//-------------------------UserDoing---------------------------

void UserDoingManagerI::createUserDoing(const Str2StrMap& props, const Ice::Current& current )
{
	InvokeClient ic = InvokeClient::create(current, "", InvokeClient::INVOKE_LEVEL_INFO);
	
	ostringstream inMsg;
	inMsg<<"UserDoingManagerI::createUserDoing" <<  " " << current.con->toString().substr(current.con->toString().find("remote",25));
	TimeCost tc = TimeCost::create(inMsg.str());

	int id;
	Str2StrMap::const_iterator it = props.find("ID");
	if(it!=props.end())
	{
		id=boost::lexical_cast<Ice::Int>(it->second);
	}else
	{
		MCE_WARN("UserDoingManagerI::createUserDoing CreateUserDoingException::Can find userID in props");
		return;
	}

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, id);
		QueryRunner(DB_USER_DOING, CDbWServer).execute(Statement::buildInsert(TABLE_USER_DOING, props));
		ostringstream idMsg;
		idMsg<<" id="<<id;
		tc.step(idMsg.str());
		
		MCE_INFO("UserDoingManagerI::createUserDoing Loading " << id);
		Str2StrMap filter;
		filter[COLUMN_ID] = boost::lexical_cast<string>(id);
		UserDoingDataIPtr rs = new UserDoingDataI ;
		UserDoingResultHandlerI handler(rs);
		size_t resultSize = QueryRunner(DB_USER_DOING, CDbWServer).query(Statement::buildSelect(TABLE_USER_DOING, filter), handler);
		
		if (resultSize == 0) {
			MCE_WARN("UserDoingManagerI::createUserDoing CreateUserDoingException::Can not load new added user ,userid  [" << id << "]");
			return ;
		}
		if (id != rs->id) {
			MCE_WARN("UserDoingManagerI::createUserDoing CreateUserDoingException::UserId error,userid = " << id << ", resultID = "<< rs->id);
			return ;
		}
		MCE_INFO("UserDoingManagerI::createUserDoing Loading " << id << " done.");
		ServiceI::instance().addObject(rs, USER_DOING, id);
		UserDoingReaderAdapter::instance().setData(id, rs->getData());
	}
  UserDoingTripodAdapter::instance().Remove(id);
	UserLogicAdapter::instance().insertUserDoing(id, props);
}

void UserDoingManagerI::reloadUserDoing(Ice::Int userId,const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);

	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, userId);
		ServiceI::instance().removeObject(USER_DOING, userId);
		UserDoingDataIPtr data = ServiceI::instance().locateObject<UserDoingDataIPtr> (USER_DOING, userId);

		if(data)
		{
			UserDoingReaderAdapter::instance().setData(userId, data->getData());
		}
	}
}

UserDoingDataPtr UserDoingManagerI::getUserDoing(Ice::Int userId, const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

	return m_getUserDoing(userId)->getData();
}

void UserDoingManagerI::setUserDoing(Ice::Int userId,const Str2StrMap& props,const Ice::Current& current)
{
	ostringstream inMsg;
	inMsg<<"id="<<userId;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_INFO);
	
	//去掉map中的用户id
	Str2StrMap::iterator it = const_cast<Str2StrMap&>(props).find("ID");
	if(it!=props.end())
	{
		const_cast<Str2StrMap&>(props).erase(it);
	}
	
	Str2StrMap filter;
	filter[COLUMN_ID] = boost::lexical_cast<string>(userId);
	{
		xce::tempmutex::Lock lock(tempMutexManagerPtr_, userId);
		QueryRunner(DB_USER_DOING, CDbWServer).execute(Statement::buildUpdate(TABLE_USER_DOING, filter, props));
		m_getUserDoing(userId)->setProperties(props);
		UserDoingReaderAdapter::instance().setUserDoing(userId, props);
	}
  UserDoingTripodAdapter::instance().Remove(userId);
	UserLogicAdapter::instance().updateUserDoing(userId, props,current.ctx);
}

UserDoingDataIPtr UserDoingManagerI::m_getUserDoing(int userId) {
	if (isValid()) {
		UserDoingDataIPtr res = ServiceI::instance().findObject<UserDoingDataIPtr> (USER_DOING, userId);
		if (res) {
			return res;
		} else {
			TaskManager::instance().execute(new LoadTask(LOAD_TYPE_USERDOING, userId));
			MCE_WARN("UserDoingManagerI::m_getUserDoing NoSuchUserDoingException:: reason - cache miss [" << userId << "]");
			throw NoSuchObjectException();
		}
	} else {
		try{
			return ServiceI::instance().locateObject<UserDoingDataIPtr> (USER_DOING, userId);
		} catch(const MyUtil::NoSuchObjectException& e) {
			MCE_WARN("UserDoingManagerI::m_getUserDoing NoSuchUserDoingException:: reason - NoSuchUserDoingException [" << userId << "]");
			throw NoSuchObjectException();
		}
	}
}
//-------------------------UserBaseVeiwManagerI---------------------------


//-----------------------------------------------------------------------
UserBaseViewDataPtr UserBaseViewManagerI::getUserBaseView( Ice::Int uid,
		                                               const Ice::Current& current) {
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

  return m_getUserBaseView(uid, current)->getOldData();
}

UserBaseViewData3Ptr UserBaseViewManagerI::getUserBaseView3( Ice::Int uid,
		                                               const Ice::Current& current) {
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

  return m_getUserBaseView(uid, current)->getData();
}

UserBaseViewDataIPtr UserBaseViewManagerI::m_getUserBaseView( Ice::Int uid,
		                                               const Ice::Current& current) {
	
	bool flag = MyUtil::RpcFilterManager::instance().filter(current, "user_passport");
	
	UserPassportDataIPtr    upd ;
	UserNameDataIPtr        und ;
	UserRightDataIPtr      urd  ;
	UserLoginCountDataIPtr ulcd  ;
	UserStateDataIPtr usd  ;
	UserUrlDataIPtr uurld  ;

	UserBasicDataIPtr  ubd ;
	
	if (isValid()) {
			upd = ServiceI::instance().findObject<UserPassportDataIPtr>(USER_PASSPORT, uid);
			und = ServiceI::instance().findObject<UserNameDataIPtr>(USER_NAME, uid);
			urd = ServiceI::instance().findObject<UserRightDataIPtr>(USER_RIGHT, uid);
			ulcd = ServiceI::instance().findObject<UserLoginCountDataIPtr>(USER_LOGINCOUNT, uid);
			usd = ServiceI::instance().findObject<UserStateDataIPtr>(USER_STATE, uid);
			uurld = ServiceI::instance().findObject<UserUrlDataIPtr> (USER_URL, uid);

			//拆分userbasic表之后，为原来userbasic对象作的兼容，将拆分后的对象拼成原来的userbasic对象
			ubd = parseToUserBasicData(uid, und, uurld, upd, usd);

			if (upd && und && urd && ulcd && usd && uurld && ubd) {
				upd = upd->getData();
				und = und->getData();
				urd = urd->getData();
				ulcd = ulcd->getData();
				usd = usd->getData();
				uurld = uurld->getData();
			} else {
				TaskManager::instance().execute(new LoadTask(LOAD_TYPE_USERBASEFULLVIEW, uid));
				MCE_WARN("UserBaseViewManagerI::getUserBaseView NoSuchUserBaseViewException:: reason - cache miss [" << uid << "]");
				throw NoSuchObjectException();
			}
	} else {
		try {
			upd = ServiceI::instance().locateObject<UserPassportDataIPtr>(USER_PASSPORT, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserBaseView NoSuchUserBaseViewException:: reason - NoSuchUserPassportException [" << uid << "]");
			throw NoSuchObjectException();
		}

		try {
			und = ServiceI::instance().locateObject<UserNameDataIPtr>(USER_NAME, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserBaseView NoSuchUserBaseViewException:: reason - NoSuchUserNameException [" << uid << "]");
			throw NoSuchObjectException();
		}
	
		try {
			urd = ServiceI::instance().locateObject<UserRightDataIPtr>(USER_RIGHT, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserBaseView NoSuchUserBaseViewException:: reason - NoSuchUserRightException [" << uid << "]");
			throw NoSuchObjectException();
		}
	
		try {
			ulcd = ServiceI::instance().locateObject<UserLoginCountDataIPtr>(USER_LOGINCOUNT, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserBaseView NoSuchUserBaseViewException:: reason - NoSuchUserLoginCountException [" << uid << "]");
			throw NoSuchObjectException();
		}
	
		try {
			usd = ServiceI::instance().locateObject<UserStateDataIPtr>(USER_STATE, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserBaseView NoSuchUserBaseViewException:: reason - NoSuchUserStateException [" << uid << "]");
			throw NoSuchObjectException();
		}
	
		try {
			uurld = ServiceI::instance().locateObject<UserUrlDataIPtr> (USER_URL, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserBaseView NoSuchUserBaseViewException:: reason - NoSuchUserUrlException [" << uid << "]");
			throw NoSuchObjectException();
		}
	

		//拆分userbasic表之后，为原来userbasic对象作的兼容，将拆分后的对象拼成原来的userbasic对象
		ubd = parseToUserBasicData(uid, und, uurld, upd, usd);
	}
	

	UserBaseViewDataIPtr res = new UserBaseViewDataI(upd, und, urd, ulcd, usd,uurld,ubd);
  if (!flag) {
    res->updi_->account="";
    res->updi_->passwordmd5="";
  }
	return res; 
}


UserBasicDataIPtr UserBaseViewManagerI::parseToUserBasicData(const int uid,
		                                                 const UserNameDataIPtr& nameData, 
		                                                 const UserUrlDataIPtr&  urlData, 
														 const UserPassportDataIPtr&  passportData, 
														 const UserStateDataIPtr& stateData )
{
	UserBasicDataIPtr res = new UserBasicDataI;

	UserBornDataIPtr      ubod;
	UserStageDataIPtr     usgd; 
	UserDoingDataIPtr     udd;
	
	if (isValid()) {
		ubod  = ServiceI::instance().findObject<UserBornDataIPtr> (USER_BORN, uid); 
		usgd = ServiceI::instance().findObject<UserStageDataIPtr> (USER_STAGE, uid); 
		udd = ServiceI::instance().findObject<UserDoingDataIPtr> (USER_DOING, uid);
		if (ubod && usgd && udd) {
			ubod = ubod->getData();
			usgd = usgd->getData();
			udd = udd->getData();
		} else {
			return 0;
		}
	} else {
		try {
			ubod  = ServiceI::instance().locateObject<UserBornDataIPtr> (USER_BORN, uid)->getData(); 
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserBaseView(parseToUserBasicData) NoSuchUserBaseViewException:: reason - NoSuchUserBornException [" << uid << "]");
			throw NoSuchObjectException();
		}

		try {
			usgd = ServiceI::instance().locateObject<UserStageDataIPtr> (USER_STAGE, uid)->getData(); 
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserBaseView(parseToUserBasicData) NoSuchUserBaseViewException:: reason - NoSuchUserStageException [" << uid << "]");
			throw NoSuchObjectException();
		}
	
		try {
			udd = ServiceI::instance().locateObject<UserDoingDataIPtr> (USER_DOING, uid)->getData(); 
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserBaseView(parseToUserBasicData) NoSuchUserBaseViewException:: reason - NoSuchUserDoingException [" << uid << "]");
			throw NoSuchObjectException();
		}
	}

	//将userborn中的数据添加到userbasic对象中
	if(ubod)
	{
       res->id = ubod->id ;
       res->gender = ubod->gender ;
       res->birthDay = ubod->birthDay ;
       res->birthMonth = ubod->birthMonth ;
       res->birthYear = ubod->birthYear ;
       res->homeProvince = ubod->homeProvince ;
       res->homeCity = ubod->homeCity ;
       res->cityCode = ubod->cityCode ;
       res->astrology = ubod->astrology ;
	}
	
	//将userstage中的数据添加到userbasic对象中
	if(usgd)
	{
		res->univ = usgd->univ ;
		res->stage = usgd->stage ;
	}
	
	//将userdoing中的数据添加到userbasic对象中
	if(udd)
	{
		res->doing = udd->doing;
		res->doingTime = udd->doingTime;
	}
	//将username中的数据添加到userbasic对象中
	if(nameData)
	{
       res->name= nameData->name;
	}
	//将userurl中的数据添加到userbasic对象中
	if(urlData)
	{
       res->headUrl = urlData->headUrl;
       res->tinyUrl = urlData->tinyUrl;
	}
	//将userpassport中的数据添加到userbasic对象中
	if(passportData)
	{
       res->status = passportData->status;
	}
	//将userstate中的数据添加到userbasic对象中
	if(stateData)
	{
       res->star = stateData->star;
	}

	return res;
}

UserCommonViewDataPtr UserBaseViewManagerI::getUserCommonView(int uid, const Ice::Current& current) 
{
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

  return m_getUserCommonView(uid, current)->getOldData();
}

UserCommonViewDataNPtr UserBaseViewManagerI::getUserCommonViewN(int uid, const Ice::Current& current) 
{
	ostringstream inMsg;
	inMsg<<"id="<<uid;
	InvokeClient ic = InvokeClient::create(current, inMsg.str(), InvokeClient::INVOKE_LEVEL_DEBUG);

  return m_getUserCommonView(uid, current)->getData();
}

UserCommonViewDataIPtr UserBaseViewManagerI::m_getUserCommonView(int uid, const Ice::Current& current) 
{
	bool flag = MyUtil::RpcFilterManager::instance().filter(current, "user_passport");

	UserPassportDataIPtr    upd ;
	UserNameDataIPtr        und ;
	UserRightDataIPtr      urd  ;
	UserLoginCountDataIPtr ulcd  ;
	UserStateDataIPtr usd  ;
	UserUrlDataIPtr uurld  ;
	UserBornDataIPtr  ubod ;
	UserStageDataIPtr usgd ;

	if (isValid()) {
			upd = ServiceI::instance().findObject<UserPassportDataIPtr>(USER_PASSPORT, uid);
			und = ServiceI::instance().findObject<UserNameDataIPtr>(USER_NAME, uid);
			urd = ServiceI::instance().findObject<UserRightDataIPtr>(USER_RIGHT, uid);
			ulcd = ServiceI::instance().findObject<UserLoginCountDataIPtr>(USER_LOGINCOUNT, uid);
			usd = ServiceI::instance().findObject<UserStateDataIPtr>(USER_STATE, uid);
			uurld = ServiceI::instance().findObject<UserUrlDataIPtr> (USER_URL, uid);
			ubod = ServiceI::instance().findObject<UserBornDataIPtr> (USER_BORN, uid);
			usgd = ServiceI::instance().findObject<UserStageDataIPtr> (USER_STAGE, uid);

			if (upd && und && urd && ulcd && usd && uurld && ubod && usgd) {
				upd = upd->getData();
				und = und->getData();
				urd = urd->getData();
				ulcd = ulcd->getData();
				uurld = uurld->getData();
				ubod = ubod->getData();
				usgd = usgd->getData();
			} else {
				TaskManager::instance().execute(new LoadTask(LOAD_TYPE_USERBASECOMMONVIEW, uid));
				MCE_WARN("UserBaseViewManagerI::getUserCommonView NoSuchUserCommonViewException:: reason - cache miss [" << uid << "]");
				throw NoSuchObjectException();
			}
	} else {
		try{
			upd = ServiceI::instance().locateObject<UserPassportDataIPtr>(USER_PASSPORT, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserCommonView NoSuchUserCommonViewException:: reason - NoSuchUserPassportException [" << uid << "]");
			throw NoSuchObjectException();
		}

		try{
			und = ServiceI::instance().locateObject<UserNameDataIPtr>(USER_NAME, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserCommonView NoSuchUserCommonViewException:: reason - NoSuchUserNameException [" << uid << "]");
			throw NoSuchObjectException();
		}
	
		try{
			urd = ServiceI::instance().locateObject<UserRightDataIPtr>(USER_RIGHT, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserCommonView NoSuchUserCommonViewException:: reason - NoSuchUserRightException [" << uid << "]");
			throw NoSuchObjectException();
		}
	
		try{
			ulcd = ServiceI::instance().locateObject<UserLoginCountDataIPtr>(USER_LOGINCOUNT, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserCommonView NoSuchUserCommonViewException:: reason - NoSuchUserLoginCountException [" << uid << "]");
			throw NoSuchObjectException();
		}
	
		try{
			usd = ServiceI::instance().locateObject<UserStateDataIPtr>(USER_STATE, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserCommonView NoSuchUserCommonViewException:: reason - NoSuchUserStateException [" << uid << "]");
			throw NoSuchObjectException();
		}
	
		try{
			uurld = ServiceI::instance().locateObject<UserUrlDataIPtr> (USER_URL, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserCommonView NoSuchUserCommonViewException:: reason - NoSuchUserUrlException [" << uid << "]");
			throw NoSuchObjectException();
		}
	
		try{
			ubod = ServiceI::instance().locateObject<UserBornDataIPtr> (USER_BORN, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserCommonView NoSuchUserCommonViewException:: reason - NoSuchUserBornException [" << uid << "]");
			throw NoSuchObjectException();
		}
	
		try{
			usgd = ServiceI::instance().locateObject<UserStageDataIPtr> (USER_STAGE, uid)->getData();
		} catch(...) {
			MCE_WARN("UserBaseViewManagerI::getUserCommonView NoSuchUserCommonViewException:: reason - NoSuchUserStageException [" << uid << "]");
			throw NoSuchObjectException();
		}
	}

	UserCommonViewDataIPtr res = new UserCommonViewDataI(upd, und, urd, ulcd, usd, uurld, ubod, usgd);

  if (!flag) {
    res->updi_->account = "";
    res->updi_->passwordmd5 = "";
  }
	return res; 
}


//--------------------------------------------------------------------------------


void UserPassportManagerI::setValid(bool valid, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(valid), InvokeClient::INVOKE_LEVEL_INFO);
	
	{
		IceUtil::RWRecMutex::WLock lock(validMutex_);
		valid_ = valid;
		ServerStateSubscriber::instance().setStatus((int)valid,USER_PASSPORT_MANAGER);
	}
	UserBasicManagerI::instance().setValid(valid);
	UserBaseViewManagerI::instance().setValid(valid);
}
void UserPassportManagerI::setData(const ObjectResultPtr& userData, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);
	TaskManager::instance().execute(new FillTask<UserPassportDataIPtr, UserPassportDataI>(USER_PASSPORT , userData));
}
bool UserPassportManagerI::isValid(const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);

	IceUtil::RWRecMutex::RLock lock(validMutex_);
	return valid_;
}

void UserStateManagerI::setValid(bool valid, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(valid), InvokeClient::INVOKE_LEVEL_INFO);
	
	{
		IceUtil::RWRecMutex::WLock lock(validMutex_);
		valid_ = valid;
		ServerStateSubscriber::instance().setStatus((int)valid,USER_STATE_MANAGER);
	}
	UserBasicManagerI::instance().setValid(valid);
	UserBaseViewManagerI::instance().setValid(valid);
}
void UserStateManagerI::setData(const ObjectResultPtr& userData, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);
	TaskManager::instance().execute(new FillTask<UserStateDataIPtr, UserStateDataI>(USER_STATE, userData));
}
bool UserStateManagerI::isValid(const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);

	IceUtil::RWRecMutex::RLock lock(validMutex_);
	return valid_;
}

void UserLoginCountManagerI::setValid(bool valid, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(valid), InvokeClient::INVOKE_LEVEL_INFO);
	
	{
		IceUtil::RWRecMutex::WLock lock(validMutex_);
		valid_ = valid;
		ServerStateSubscriber::instance().setStatus((int)valid,USER_LOGINCOUNT_MANAGER);
	}
	UserBaseViewManagerI::instance().setValid(valid);
}
void UserLoginCountManagerI::setData(const ObjectResultPtr& userData, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);
	TaskManager::instance().execute(new FillTask<UserLoginCountDataIPtr, UserLoginCountDataI>(USER_LOGINCOUNT, userData));
}
bool UserLoginCountManagerI::isValid(const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);

	IceUtil::RWRecMutex::RLock lock(validMutex_);
	return valid_;
}

void UserRightManagerI::setValid(bool valid, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(valid), InvokeClient::INVOKE_LEVEL_INFO);
	
	{
		IceUtil::RWRecMutex::WLock lock(validMutex_);
		valid_ = valid;
		ServerStateSubscriber::instance().setStatus((int)valid,USER_RIGHT_MANAGER);
	}
	UserBaseViewManagerI::instance().setValid(valid);
}
void UserRightManagerI::setData(const ObjectResultPtr& userData, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);
	TaskManager::instance().execute(new FillTask<UserRightDataIPtr, UserRightDataI>(USER_RIGHT, userData));
}
bool UserRightManagerI::isValid(const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);

	IceUtil::RWRecMutex::RLock lock(validMutex_);
	return valid_;
}

void UserUrlManagerI::setValid(bool valid, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(valid), InvokeClient::INVOKE_LEVEL_INFO);
	
	{
		IceUtil::RWRecMutex::WLock lock(validMutex_);
		valid_ = valid;
		ServerStateSubscriber::instance().setStatus((int)valid,USER_URL_MANAGER);
	}
	UserBasicManagerI::instance().setValid(valid);
	UserBaseViewManagerI::instance().setValid(valid);
}
void UserUrlManagerI::setData(const ObjectResultPtr& userData, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);
	TaskManager::instance().execute(new FillTask<UserUrlDataIPtr, UserUrlDataI>(USER_URL, userData));
}
bool UserUrlManagerI::isValid(const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);

	IceUtil::RWRecMutex::RLock lock(validMutex_);
	return valid_;
}

void UserNameManagerI::setValid(bool valid, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(valid), InvokeClient::INVOKE_LEVEL_INFO);
	
	{
		IceUtil::RWRecMutex::WLock lock(validMutex_);
		valid_ = valid;
		ServerStateSubscriber::instance().setStatus((int)valid,USER_NAME_MANAGER);
	}
	UserBasicManagerI::instance().setValid(valid);
	UserBaseViewManagerI::instance().setValid(valid);
}
void UserNameManagerI::setData(const ObjectResultPtr& userData, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);
	TaskManager::instance().execute(new FillTask<UserNameDataIPtr, UserNameDataI>(USER_NAME, userData));
}
bool UserNameManagerI::isValid(const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);

	IceUtil::RWRecMutex::RLock lock(validMutex_);
	return valid_;
}

void UserBornManagerI::setValid(bool valid, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(valid), InvokeClient::INVOKE_LEVEL_INFO);

	{
		IceUtil::RWRecMutex::WLock lock(validMutex_);
		valid_ = valid;
		ServerStateSubscriber::instance().setStatus((int)valid,USER_BORN_MANAGER);
	}
	UserBasicManagerI::instance().setValid(valid);
	UserBaseViewManagerI::instance().setValid(valid);
}
void UserBornManagerI::setData(const ObjectResultPtr& userData, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);
	TaskManager::instance().execute(new FillTask<UserBornDataIPtr, UserBornDataI>(USER_BORN, userData));
}
bool UserBornManagerI::isValid(const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);

	IceUtil::RWRecMutex::RLock lock(validMutex_);
	return valid_;
}

void UserStageManagerI::setValid(bool valid, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(valid), InvokeClient::INVOKE_LEVEL_INFO);
	
	{
		IceUtil::RWRecMutex::WLock lock(validMutex_);
		valid_ = valid;
		ServerStateSubscriber::instance().setStatus((int)valid,USER_STAGE_MANAGER);
	}
	UserBasicManagerI::instance().setValid(valid);
	UserBaseViewManagerI::instance().setValid(valid);
}
void UserStageManagerI::setData(const ObjectResultPtr& userData, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);
	TaskManager::instance().execute(new FillTask<UserStageDataIPtr, UserStageDataI>(USER_STAGE, userData));
}
bool UserStageManagerI::isValid(const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);

	IceUtil::RWRecMutex::RLock lock(validMutex_);
	return valid_;
}

void UserDoingManagerI::setValid(bool valid, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(valid), InvokeClient::INVOKE_LEVEL_INFO);
	
	{
		IceUtil::RWRecMutex::WLock lock(validMutex_);
		valid_ = valid;
		ServerStateSubscriber::instance().setStatus((int)valid,USER_DOING_MANAGER);
	}
	UserBasicManagerI::instance().setValid(valid);
	UserBaseViewManagerI::instance().setValid(valid);
}
void UserDoingManagerI::setData(const ObjectResultPtr& userData, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);
	TaskManager::instance().execute(new FillTask<UserDoingDataIPtr, UserDoingDataI>(USER_DOING, userData));
}
bool UserDoingManagerI::isValid(const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);

	IceUtil::RWRecMutex::RLock lock(validMutex_);
	return valid_;
}

void UserBasicManagerI::setValidForce(bool valid, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(valid), InvokeClient::INVOKE_LEVEL_INFO);

	{
		IceUtil::RWRecMutex::WLock lock(validMutex_);
		valid_ = valid;
		ServerStateSubscriber::instance().setStatus((int)valid,USER_BASIC_MANAGER);
	}
}

void UserBasicManagerI::setValid(bool valid) {
	IceUtil::RWRecMutex::WLock lock(validMutex_);
	if (UserPassportManagerI::instance().isValid() 
		&& UserStateManagerI::instance().isValid() 
		&& UserUrlManagerI::instance().isValid()
		&& UserNameManagerI::instance().isValid()
		&& UserBornManagerI::instance().isValid()
		&& UserStageManagerI::instance().isValid()
		&& UserDoingManagerI::instance().isValid()) {
		valid_ = true;
	} else {
		valid_ = false;
	}
}

bool UserBasicManagerI::isValid(const ::Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);

	IceUtil::RWRecMutex::RLock lock(validMutex_);
	return valid_;
}

void UserBaseViewManagerI::setValidForce(bool valid, const Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current, boost::lexical_cast<string>(valid), InvokeClient::INVOKE_LEVEL_INFO);
	
	{
		IceUtil::RWRecMutex::WLock lock(validMutex_);
		valid_ = valid;
		ServerStateSubscriber::instance().setStatus((int)valid,USER_BASEVIEW_MANAGER);
	}
}

void UserBaseViewManagerI::setValid(bool valid) {
	IceUtil::RWRecMutex::WLock lock(validMutex_);
	UserPassportDataIPtr    upd ;
	UserNameDataIPtr        und ;
	UserRightDataIPtr      urd  ;
	UserLoginCountDataIPtr ulcd  ;
	UserStateDataIPtr usd  ;
	UserUrlDataPtr uurld  ;
	UserBornDataPtr  ubod ;
	UserStageDataPtr usgd ;
	if (UserPassportManagerI::instance().isValid()
		&& UserNameManagerI::instance().isValid()
		&& UserRightManagerI::instance().isValid()
		&& UserLoginCountManagerI::instance().isValid()
		&& UserStateManagerI::instance().isValid()
		&& UserUrlManagerI::instance().isValid()
		&& UserBornManagerI::instance().isValid()
		&& UserStageManagerI::instance().isValid()
		&& UserDoingManagerI::instance().isValid()) {
		valid_ = true;
	} else {
		valid_ = false;
	}
}
bool UserBaseViewManagerI::isValid(const ::Ice::Current& current) {
	InvokeClient ic = InvokeClient::create(current);

	IceUtil::RWRecMutex::RLock lock(validMutex_);
	return valid_;
}

void LoadTask::handle() {
	switch (type_) {
		case LOAD_TYPE_USERPASSPORT:
			LoadUserPassport();
			break;
		case LOAD_TYPE_USERRIGHT:
			LoadUserRight();
			break;
		case LOAD_TYPE_USERSTATE:
			LoadUserState();
			break;
		case LOAD_TYPE_USERNAMES:
			LoadUserNames();
			break;
		case LOAD_TYPE_USERLOGINCOUNT:
			LoadUserLoginCount();
			break;
		case LOAD_TYPE_USERURL:
			LoadUserUrl();
			break;
		case LOAD_TYPE_USERBORN:
			LoadUserBorn();
			break;
		case LOAD_TYPE_USERSTAGE:
			LoadUserStage();
			break;
		case LOAD_TYPE_USERDOING:
			LoadUserDoing();
			break;
		case LOAD_TYPE_USERBASIC:
			LoadUserBasic();
			break;
		case LOAD_TYPE_USERBASECOMMONVIEW:
			LoadUserBaseCommonView();
			break;
		case LOAD_TYPE_USERBASEFULLVIEW:
			LoadUserBaseFullView();
			break;
		default:
			MCE_WARN("LoadTask::handle Unknown type: "<<type_);
	}
}

void LoadTask::LoadUserPassport() {
	try {
		UserPassportDataIPtr data = ServiceI::instance().locateObject<UserPassportDataIPtr>(USER_PASSPORT, userid_);
		if(data)
		{
			UserPassportReaderAdapter::instance().setData(userid_, data->getData());
		}
	} catch (const MyUtil::NoSuchObjectException& e) {
		MCE_WARN("LoadTask::LoadUserPassport -> NoSuchUserPassportException ["<< userid_<<"]");
	}
}

void LoadTask::LoadUserRight() {
	try {
		UserRightDataIPtr data = ServiceI::instance().locateObject<UserRightDataIPtr>(USER_RIGHT, userid_);
		if(data)
		{
			UserRightReaderAdapter::instance().setData(userid_, data->getData());
		}
	} catch (const MyUtil::NoSuchObjectException& e) {
		MCE_WARN("LoadTask::LoadUserRight -> NoSuchUserRightException ["<<userid_<<"]");
	}
}

void LoadTask::LoadUserState() {
	try {
		UserStateDataIPtr data = ServiceI::instance().locateObject<UserStateDataIPtr>(USER_STATE, userid_);
		if(data)
		{
			UserStateReaderAdapter::instance().setData(userid_, data->getData());
		}
	} catch (const MyUtil::NoSuchObjectException& e) {
		MCE_WARN("LoadTask::LoadUserState -> NoSuchUserStateException ["<<userid_<<"]");
	}
}

void LoadTask::LoadUserNames() {
	try {
		UserNameDataIPtr data = ServiceI::instance().locateObject<UserNameDataIPtr>(USER_NAME, userid_);
		if(data)
		{
			UserNameReaderAdapter::instance().setData(userid_, data->getData());
		}
	} catch (const MyUtil::NoSuchObjectException& e) {
		MCE_WARN("LoadTask::LoadUserNames -> NoSuchUserNamesException ["<<userid_<<"]");
	}
}

void LoadTask::LoadUserLoginCount() {
	try {
		UserLoginCountDataIPtr data = ServiceI::instance().locateObject<UserLoginCountDataIPtr>(USER_LOGINCOUNT, userid_);
		if(data)
		{
			UserLoginCountReaderAdapter::instance().setData(userid_, data->getData());
		}
	} catch (const MyUtil::NoSuchObjectException& e) {
		MCE_WARN("LoadTask::LoadUserLoginCount -> NoSuchUserLoginCountException ["<<userid_<<"]");
	}
}

void LoadTask::LoadUserUrl() {
	try {
		UserUrlDataIPtr data = ServiceI::instance().locateObject<UserUrlDataIPtr>(USER_URL, userid_);
		if(data)
		{
			UserUrlReaderAdapter::instance().setData(userid_, data->getData());
		}
	} catch (const MyUtil::NoSuchObjectException& e) {
		MCE_WARN("LoadTask::LoadUserUrl -> NoSuchUserUrlException ["<<userid_<<"]");
	}
}

void LoadTask::LoadUserBorn() {
	try {
		UserBornDataIPtr data = ServiceI::instance().locateObject<UserBornDataIPtr> (USER_BORN, userid_);
		if(data)
		{
			UserBornReaderAdapter::instance().setData(userid_, data->getData());
		}
	} catch (const MyUtil::NoSuchObjectException& e) {
		MCE_WARN("LoadTask::LoadUserBorn -> NoSuchUserBornException ["<<userid_<<"]");
	}
}

void LoadTask::LoadUserStage() {
	try {
		UserStageDataIPtr data = ServiceI::instance().locateObject<UserStageDataIPtr> (USER_STAGE, userid_);
		if(data)
		{
			UserStageReaderAdapter::instance().setData(userid_, data->getData());
		}
	} catch (const MyUtil::NoSuchObjectException& e) {
		MCE_WARN("LoadTask::LoadUserStage -> NoSuchUserStageException ["<<userid_<<"]");
	}
}

void LoadTask::LoadUserDoing() {
	try {
		UserDoingDataIPtr data = ServiceI::instance().locateObject<UserDoingDataIPtr> (USER_DOING, userid_);
		if(data)
		{
			UserDoingReaderAdapter::instance().setData(userid_, data->getData());
		}
	} catch (const MyUtil::NoSuchObjectException& e) {
		MCE_WARN("LoadTask::LoadUserDoing -> NoSuchUserDoingException ["<<userid_<<"]");
	}
}

void LoadTask::LoadUserBasic() {
	LoadUserPassport();
	LoadUserState();
	LoadUserUrl();
	LoadUserNames();
	LoadUserBorn();
	LoadUserStage();
	LoadUserDoing();
}

void LoadTask::LoadUserBaseCommonView() {
	LoadUserPassport();
	LoadUserNames();
	LoadUserRight();
	LoadUserLoginCount();
	LoadUserState();
	LoadUserUrl();
	LoadUserBorn();
	LoadUserStage();
}

void LoadTask::LoadUserBaseFullView() {
	LoadUserPassport();
	LoadUserNames();
	LoadUserRight();
	LoadUserLoginCount();
	LoadUserState();
	LoadUserUrl();
	LoadUserBorn();
	LoadUserStage();
	LoadUserDoing();
}


