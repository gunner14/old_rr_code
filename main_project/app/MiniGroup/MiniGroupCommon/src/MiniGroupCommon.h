/*
 * =====================================================================================
 *
 *       Filename:  MiniGroupCommon.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/25/2011 04:05:50 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef __MINIGROUP_COMMON_H__
#define __MINIGROUP_COMMON_H__

#include "MiniGroup.h"

#include <map>
#include <Ice/Ice.h>

#include "QueryRunner.h"
#include "Singleton.h"
#include "MiniGroup.pb.h"
#include "User2MiniGroup.pb.h"

namespace xce {
namespace mngp {
  using namespace std;
  using namespace com::xiaonei::xce;

  const string DB_SOURCE = "minigroup";
  const Ice::Int MINIGROUP_INFO = 0;
  const Ice::Int USER_2_MINIGROUP_INFO = 1;
  const string TABLE_MINIGROUP = "minigroup";
  const string TABLE_MEMBER_MINIGROUP = "member_minigroup";
  const string TABLE_MINIGROUP_ID = "id";
  const string TABLE_MINIGROUP_NAME = "name";
  const string TABLE_MINIGROUP_ICON = "icon";
  const string TABLE_MEMBER_MINIGROUP_MNGP_ID = "minigroup_id";
  const string TABLE_MEMBER_MINIGROUP_MEMBER_ID = "member_id";
  const string TABLE_MEMBER_MINIGROUP_RESTRICT = "access_id >= 1 and access_id <= 3 and (is_left = 1 or is_left = 0)";
  const string TABLE_MEMBER_MINIGROUP_SORT = " order by member_id, is_left desc, update_time desc";
  const int TABLE_MOD = 100;

  const static int TASK_RELOAD_BY_USERID = 1;
  const static int TASK_RELOAD_BY_MINIGROUPID = 2;

  const int PRODUCE_BATCH_SIZE = 500;
  const int CONSUME_BATCH_SIZE = 500;
  const int CONSUME_BATCH_THREAD_SIZE = 5;
  const int WRITE_BATCH_SIZE = 1000;
  const int PRX_DATAQUEUE_MAX_SIZE = 50000;

  class MiniGroupInfo : virtual public MiniGroup, virtual public IceUtil::Mutex {
    public:
      MiniGroupInfo(long id, const string& name, const string& icon);
      MiniGroupInfo(long id, const string& name, const string& icon, const MyUtil::Str2StrMap& extraInfo);
      MiniGroupInfo(const MiniGroupPtr& ptr);
      void setId(long id);
      void setName(const string& name);
      void setIcon(const string& icon);
      long getId();
      string serialize();
      string getName();
      string getIcon();
      MyUtil::Str2StrMap getExtraInfo();
      void addToExtraInfo(const string& key, const string& value);
  };

  typedef IceUtil::Handle<MiniGroupInfo> MiniGroupInfoPtr;

  class User2MiniGroupInfo : public User2MiniGroup, virtual public IceUtil::Mutex {
    public:
      User2MiniGroupInfo(int user_id, const MyUtil::LongSeq& minigroup_ids);
      User2MiniGroupInfo(const User2MiniGroupPtr& ptr);
      void setJoinedMiniGroups(const MyUtil::LongSeq& minigroup_ids);
      void addJoinedMiniGroups(const MyUtil::LongSeq& minigroup_ids, bool addDirectly = false);
      void removeJoinedMiniGroups(const MyUtil::LongSeq& minigroup_ids);
      void clearJoinedMiniGroups();
      int  getUserId();
      string serialize();
      MyUtil::LongSeq getJoinedMiniGroupIds();
  };

  typedef IceUtil::Handle<User2MiniGroupInfo> User2MiniGroupInfoPtr;

  class MiniGroupInfoResultHandlerI : public com::xiaonei::xce::ResultHandler {
    public: 
      MiniGroupInfoResultHandlerI(vector<MiniGroupInfoPtr>& minigroupinfo_ptrs);
      virtual bool handle(mysqlpp::Row&) const;
    private:
      vector<MiniGroupInfoPtr>& minigroupinfo_ptrs_;
  };

  class User2MiniGroupInfoResultHandlerI : public com::xiaonei::xce::ResultHandler {
    public: 
      User2MiniGroupInfoResultHandlerI(vector<User2MiniGroupInfoPtr>& user2minigroup_ptrs);
      virtual bool handle(mysqlpp::Row&) const;
    private:
      vector<User2MiniGroupInfoPtr>& user2minigroupinfo_ptrs_;
  };  

  class UserIdsProducerHandlerI : public com::xiaonei::xce::ResultHandler {
    public:
      UserIdsProducerHandlerI(vector<int>& ids, int& max_id);
      virtual bool handle(mysqlpp::Row&) const;
    private:
      vector<int>& result_ids_;
      int& max_id_;
  };

  template<typename V>
  class DBUtil {
    public:
      template<typename T>
      static string getTableName(T id, T mod, const string& prefix);
      //将一个vector中的元素拼接成str, 例如vector中含有[0, -1, 3], 结果为"0, -1, 3"
      template<typename T>
      static string getIdsStr(const vector<T>& ids);
      //将一个数据(int 或者 long型的，按照mod划分成不同的数据，例如输入为[1, 3, 2, 8, 22, 10, 4, 12, 3, 4], mod = 10, 结果为
      //{0->[10], 1->[1], 2->[2, 22, 12], 3->[3, 3], 4->[4, 4], 8->[8]}
      template<typename T>
      static map<T, vector<T> > seperateIdsByMod(int mod, const vector<T>& ids);
    private:
      V value;
  };

  template<typename T>
  class BatchIdResultHandler : public com::xiaonei::xce::ResultHandler {
    public:
      BatchIdResultHandler(const string& idColumnName, vector<T>& ids);
      virtual bool handle(mysqlpp::Row&) const;
    private:
      string _idColumnName;
      vector<T>& _ids;
  };

  template<typename T, typename TPtr, typename SPtr>
  MyUtil::ObjectResultPtr parse(const MyUtil::ObjectResultPtr& sourceData);

  MyUtil::ObjectResultPtr parse(const MiniGroupSeq& miniGroups);
}
}
 
#endif
