/*
 * =====================================================================================
 *
 *       Filename:  TripodLikeCache.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年01月11日 10时57分42秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef __TRIPOD_LIKE_CACHE_H__
#define __TRIPOD_LIKE_CACHE_H__

#include <ILikeIt.h>
#include <Ice/Ice.h>
#include "UtilCxx/src/TaskManager.h"
#include "IceExt/src/ServiceI.h"
#include "app/ILikeIt/share/ILikeItUtil.h"
#include "OceCxxAdapter/src/BuddyByIdReplicaAdapter.h"
#include "OceCxxAdapter/src/UserCacheAdapter.h"
#include "tripod-new/src/cpp/include/TripodClient.h"

namespace xce {
namespace tripodlike {

using namespace com::renren::tripod;
using namespace xce::ilikeit;
// ====================从数据库加载missing keys的线程===============

static const string TRIPOD_NAMESPACE = "test";
static const string TRIPOD_BUSINESS_NAME = "ILikeIt";
static const string TRIPOD_ZK_ADDR = "UserZooKeeper1:2181,UserZooKeeper2:2181,UserZooKeeper3:2181,UserZooKeeper4:2181,UserZooKeeper5:2181/Tripod";
static const int ILIKEIT_CACHE = 1;

class ReloadFromTripodThread : public IceUtil::Thread {
public:
  ReloadFromTripodThread(const string& businessName);
  void add(const KeySeq& keys);
  void add(const string& key);
  virtual void run();
private:
  IceUtil::Monitor<IceUtil::Mutex> mutex_;
  KeySeq keysToReload_;
  TripodClient* tripodClient_;
};

typedef IceUtil::Handle<ReloadFromTripodThread> ReloadFromTripodThreadPtr;

// ============== ILikeItCacheI主类 =============================
class ILikeItCacheI : virtual public ILikeItCache,
		public MyUtil::Singleton<ILikeItCacheI>  {
public:
  void init(const string& businessName);
	virtual ::Ice::Int getLikeCount(const GlobalIdPtr& gid, const Ice::Current& ctr=Ice::Current());

  virtual ::MyUtil::IntSeq getLikeCache(const GlobalIdPtr& gid, const Ice::Current& ctr=Ice::Current());

	/* @brief 	获取按照查看的用户进行个性排序的喜欢过一个对象的用户列表
	 * @param	gid		对象的GlobalId
	 * @param	uid		查看该对象的用户id
	 * @param	limit		显示详情的用户数量
	 * @param	showStrangers	是否显示陌生人(非自己及好友)的详情
	 * @return	符合条件的用户资料信息包
	 */
  virtual LikeInfoPtr getLikeDetail(const GlobalIdPtr& gid, ::Ice::Int uid, ::Ice::Int limit, bool showStrangers, const Ice::Current& ctr=Ice::Current());
  virtual LikeInfo2Ptr getLikeDetail2(const GlobalIdPtr& gid, ::Ice::Int uid, ::Ice::Int limit, bool showStrangers, const Ice::Current& ctr=Ice::Current());

	/* @brief	批量获取按照查看的用户进行个性排序的顶过一组对象的用户列表
	 * @param	gids		一组对象的gids列表
	 * @param	uid		查看该对象的用户id
	 * @param	limit		显示详情的用户数量
	 * @param	detailNum	按照limit数量显示用户资料的对象(gid)的条数(其他的无论limit是多少均不显示用户详情)
	 * @return      符合条件的用户资料信息包列表,排列顺序和gids相同
	 */
  virtual LikeInfoSeq getLikeDetailBatch(const GlobalIdSeq& gids, ::Ice::Int uid, ::Ice::Int limit, ::Ice::Int detailNum, const Ice::Current& ctr=Ice::Current());
  virtual LikeInfo2Seq getLikeDetailBatch2(const GlobalIdSeq& gids, ::Ice::Int uid, ::Ice::Int limit, ::Ice::Int detailNum, const Ice::Current& ctr=Ice::Current());

	/* @brief 	获取喜欢一个对象的用户总数和用户id列表(有最大长度限制)
	 * @param	gid		对象的GlobalId
	 * @param	limit		列表最多显示的用户数量
	 * @return	符合条件的用户总数和用户列表	 */
	virtual LikeInfoSimplePtr getLikeInfoSimple(const GlobalIdPtr& gid, ::Ice::Int limit, const Ice::Current& ctr=Ice::Current());

	/* @brief 	批量获取喜欢一组对象的用户总数和用户id列表(有最大长度限制)
	 * @param	gids		一组对象的GlobalId
	 * @param	limit		列表最多显示的用户数量
	 * @return	相应的符合条件的用户总数和用户列表组，和gid在数组中的位置对应	 */
	virtual LikeInfoSimpleSeq getLikeInfoSimpleBatch(const GlobalIdSeq& gids, int limit, const Ice::Current& ctr=Ice::Current());

	/* @brief	一个用户是否like过一个对象
	 * @param	gid		对象的GlobalId
	 * @param	uid		用户id
	 * @return	1-该用户like过该对象; 0-没有 */
	virtual int isLiked(const GlobalIdPtr& gid, int uid, const Ice::Current& ctr=Ice::Current());

	/* @brief	一个用户是否like过一组对象
	 * @param	gids		这组对象的GlobalId
	 * @param	uid		用户id
	 * @return	该用户依次是否like过这组对象 */
	virtual MyUtil::IntSeq isLikedBatch(const GlobalIdSeq& gids, int uid, const Ice::Current& ctr=Ice::Current());

	virtual void load(const GlobalIdSeq& gids, const Ice::Current& ctr=Ice::Current());

	virtual void setData(const ::MyUtil::ObjectResultPtr& objects, const Ice::Current& ctr=Ice::Current());

	virtual void addLikeship(const LikeshipPtr& ships, const Ice::Current& ctr=Ice::Current());
	
	virtual void removeLikeship(const LikeshipPtr& ships, const Ice::Current& ctr=Ice::Current());	

	virtual void setValid(bool newState, const Ice::Current& ctr=Ice::Current());

	virtual bool isValid(const Ice::Current& ctr=Ice::Current());

private:
  template<class Type, class TypePtr>
  vector<TypePtr> getLikeInfoBatch(const GlobalIdSeq& gids, ::Ice::Int uid, ::Ice::Int limit, ::Ice::Int detailNum);

  template<class Type, class TypePtr>
  TypePtr getLikeInfo(const GlobalIdPtr& gid, ::Ice::Int uid, ::Ice::Int limit, bool showStrangers);
	bool belongsMe(const GlobalIdPtr& gid);
	bool getFriendList(vector<int>& friends, int uid);
	void getCountAndSort(vector<int>& usersSort, int& selfCount, int& friendCount, const vector<int>& users, int selfId, 
      const vector<int>& friendList, bool withFriends, bool showStrangers, int limit);
	UserDetail2Seq getUserDetail(const vector<int>& uids);
  void assign(UserDetail2Seq& userDetails, const UserDetail2Seq& userDetail2Seq);
  void assign(UserDetailSeq& userDetails, const UserDetail2Seq& userDetail2Seq);
  map<long, LikeDataPtr> getWithKeys(const vector<string>& keys);
  map<long, LikeDataPtr> getWithKeys(const GlobalIdSeq& gids);
  LikeDataPtr getWithKey(const GlobalIdPtr& gid);
  TripodClient* tripodClient_;
  ReloadFromTripodThreadPtr reloadThreadPtr_;
};


class TripodLikeData : public Ice::Object, virtual public IceUtil::Mutex {
public:
  TripodLikeData(const LikeDataPtr& ptr);
  TripodLikeData(const string& byteArray);
  LikeDataPtr parseTo() const;
  string serialize() const;
  vector<int> getUserIds() const;
private:
  vector<int> userIds_;
};

typedef IceUtil::Handle<TripodLikeData> TripodLikeDataPtr;

class TripodLikeUtils {
public:
  static vector<string> getKeySeq(const vector<long>& gids);
  static vector<string> getKeySeq(long gid);
  static vector<string> getKeySeq(const GlobalIdSeq& gids);
  static vector<long> getLongSeq(const vector<string>& keys);
  static vector<long> getLongSeq(const GlobalIdSeq& gids);
};

// ============== Factory部分 ================================
class ILikeItCacheFactoryI: public MyUtil::ServantFactory {
public:
  //下面这两个函数都给空实现就可以了，因为ObjectCache不会调用locate方法
  virtual Ice::ObjectPtr create(Ice::Long gidLong) {
    return 0;
  }
  virtual MyUtil::ObjectResultPtr create(const MyUtil::LongSeq& gidLongs) {
    MyUtil::ObjectResultPtr ptr = new MyUtil::ObjectResult;
    return ptr;
  }
};


}
}

#endif
