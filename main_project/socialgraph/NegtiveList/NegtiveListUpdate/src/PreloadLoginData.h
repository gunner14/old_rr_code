#ifndef _PRELOAD_NEGTIVE_USER_H__
#define _PRELOAD_NEGTIVE_USER_H__

#include <string>
#include <map>
#include <set>
#include <vector>
#include <bitset>
#include <sstream>
#include <fstream>

#include "ObjectCacheI.h"
#include "QueryRunner.h"

#include "ReadFriendListUtil.h"
#include "SortByFriendRankUtil.h"
#include "ReverseMapUtil.h"
#include "MyTasks.h"
#include "socialgraph/socialgraphutil/clogging.h" // for clog

using namespace xce::socialgraph;
extern readfriend::ReadFriendListUtil* g_readfriend_ptr;
extern sortbyrank::SortByFriendRankUtil* g_sortbyrank_ptr;
 
namespace xce {
namespace logindata{
const static int begin = 0;
const static int end = 10;

const static int NEGTIVEDAYS = 15;
const static int ACTIVEDAYS = 15;

class PreloadLoginData {
public:
	void loadFromDB();

private:
	void loadFirstTable(const std::string& tableName);      //从数据库中加载本月数据
	void loadSecondTable(const std::string& tableName);     //从数据库中加载上月数据
	void buildBitMap();                                     //获得用户登录的bitmap
	void buildNegtiveUserList();                            //获得不活跃用户列表
	void getFriendsOfNegtiveList(std::map<int, std::vector<int> >& negtiveFriendsMap);
                                                            //获得不活跃用户的好友列表
	void getUserNegtiveListMap(const std::map<int, std::vector<int> >& negtiveFriendsMap, std::map<int, std::vector<int> >& userNegtiveListMap);
                                                            //获得用户的不活跃好友列表
	void sortUserNegtiveListByRank(std::map<int, std::vector<int> >& userNegtiveListMap);
                                                            //根据好友亲密度将列表排序
	void appendSingleRelationAndsetCache(const std::map<int, std::vector<int> >& userNegtiveListMap);
                                                            //更新Cache

	std::map< int, std::vector<int> > _userLoginMap;
	std::map< int, std::bitset<32> > _userLoginBitMap;
	std::vector<int> _negtiveUserList;
};

//********************************************************************************************

class FirstLoginReadHandler : public com::xiaonei::xce::ResultHandler {
public:
	FirstLoginReadHandler(int& last, std::map< int, std::vector<int> >& userLoginMap): _last(last), _userLoginMap(userLoginMap) {}	
	virtual bool handle(mysqlpp::Row& row) const;	

private:
	int& _last;
	std::map< int, std::vector<int> >& _userLoginMap;
};

//********************************************************************************************

class SecondLoginReadHandler : public com::xiaonei::xce::ResultHandler {
public:
	SecondLoginReadHandler(int& last, std::map< int, std::vector<int> >& userLoginMap): _last(last), _userLoginMap(userLoginMap) {}	
	virtual bool handle(mysqlpp::Row& row) const;	

private:
	int& _last;
	std::map< int, std::vector<int> >& _userLoginMap;
};

}
}

#endif
