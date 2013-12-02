#ifndef __FOLLOW_RELATION_WRITER_I_H__
#define __FOLLOW_RELATION_WRITER_I_H__

#include "Ice/Ice.h"
#include "IceExt/src/ServiceI.h"
#include "UtilCxx/src/Singleton.h"
#include "IceUtil/IceUtil.h"
#include "FollowRelation.h"
#include <time.h>

// FollowRelationWriter 同步写入或删除relation_center源中的member表和relation_center_user源中的user_member表中的数据
// 如果对member表的操作失败，则不再执行对user_member表的操作
// 如果DB中存在相应记录，会更新时间列

namespace xce {
namespace buddy {

const int TASK_LEVEL_FOLLOWRELATION_WRITER_ADD = 1;
const int TASK_LEVEL_FOLLOWRELATION_WRITER_REMOVE = 2;

/*****************************************************************************/

class FollowRelationWriterI: virtual public FollowRelationWriter, virtual public MyUtil::Singleton<
    FollowRelationWriterI> {
public:
  //add FollowRelation
  virtual void add(const MyUtil::GlobalIdPtr& gid, ::Ice::Int uid, const Ice::Current& current = Ice::Current());
  //remove FollowRelation
  virtual void remove(const MyUtil::GlobalIdPtr& gid, ::Ice::Int uid, const Ice::Current& current = Ice::Current());
private:
  friend class MyUtil::Singleton<FollowRelationWriterI>;

  std::string getTimeStr() {
    time_t now;
    time(&now);

//    //使用MYSQL函数
//    ostringstream timeStr;
//    timeStr << "FROM_UNIXTIME(" << now << ")";
//    return timeStr.str();

    // 使用c++函数
    struct tm * time_info = localtime(&now);
    char time_char[22];
    strftime(time_char, sizeof(time_char), "'%F %T'", time_info);
    return std::string(time_char);
  }
};//class FollowRelationWriterI

/*****************************************************************************/

//add FollowRelation task
class AddTask: virtual public MyUtil::Task {
public:
  AddTask(Ice::Long gid, ::Ice::Int uid, const std::string timeStr, ::Ice::Int level =
      TASK_LEVEL_FOLLOWRELATION_WRITER_ADD) :
    MyUtil::Task(level), gid_(gid), uid_(uid), timeStr_(timeStr) {
  }
  virtual void handle();
private:
  bool addToMemberTable();
  bool addToUserMemberTable();

  Ice::Long gid_;
  Ice::Int uid_;
  std::string timeStr_;
};//class AddTask

//remove FollowRelation task
class RemoveTask: virtual public MyUtil::Task {
public:
  RemoveTask(Ice::Long gid, ::Ice::Int uid, ::Ice::Int level = TASK_LEVEL_FOLLOWRELATION_WRITER_REMOVE) :
    MyUtil::Task(level), gid_(gid), uid_(uid) {
  }
  virtual void handle();
private:
  bool removeFromMemberTable();
  bool removeFromUserMemberTable();

  Ice::Long gid_;
  Ice::Int uid_;
};//class RemoveTask

class ServiceSchemaI: public MyUtil::Descriptor {
public:
  virtual void configure(const Ice::PropertiesPtr& props);
};

}//namespace buddy
}//namespace xce

#endif

