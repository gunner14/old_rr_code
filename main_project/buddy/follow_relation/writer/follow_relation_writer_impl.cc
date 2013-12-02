#include "TaskManager.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"
#include "follow_relation_writer_impl.h"
#include "follow_relation_cache_adapter.h"
#include "follow_resource_cache_adapter.h"
#include "UtilCxx/src/GlobalId.h"
#include "util/cpp/TimeCost.h"
#include "QueryRunner.h"
#include "Monitor/client/service/ServiceMonitor.h"
#include "util/cpp/InvokeMonitor.h"

int main() {
  return 0;
}

void MyUtil::initialize() {
  using namespace xce::buddy;
  using namespace MyUtil;
  using namespace xce::serverstate;

  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&FollowRelationWriterI::instance(), service.createIdentity("M", ""));

  //----------------------------Monitor-----------------------------------------
  ServiceMonitorManager::instance().start();

  TaskManager::instance().config(TASK_LEVEL_FOLLOWRELATION_WRITER_ADD, ThreadPoolConfig(1, 100));
  TaskManager::instance().config(TASK_LEVEL_FOLLOWRELATION_WRITER_REMOVE, ThreadPoolConfig(1, 100));

  //----------------------------ServiceSchema-----------------------------------
  service.registerXceObserver(new ServiceSchemaI);

  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("FollowRelationWriter.Mod");
  int Interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault(
      "FollowRelationWriter.Interval", 5);
  // register to Controller Service
  ServerStateSubscriber::instance().initialize("ControllerFollowRelationWriter", &FollowRelationWriterI::instance(),
      mod, Interval);
}

/*****************************************************************************/

namespace xce {
namespace buddy {

void FollowRelationWriterI::add(const MyUtil::GlobalIdPtr& gid, ::Ice::Int uid, const Ice::Current& current) {
  using namespace MyUtil;
  long globalId = GlobalIdTransfer::get(gid);
  MCE_INFO("FollowRelationWriterI::add:" << globalId << " uid:" << uid);
  ostringstream msg;
  msg << " gid:" << globalId << " uid:" << uid;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);

  std::string timeStr = getTimeStr();
  MCE_DEBUG("FollowRelationWriterI::add: timeStr = " << timeStr);

  MyUtil::TaskManager::instance().execute(new AddTask(globalId, uid, timeStr));
}

void FollowRelationWriterI::remove(const MyUtil::GlobalIdPtr& gid, ::Ice::Int uid, const Ice::Current& current) {
  using namespace MyUtil;
  long globalId = GlobalIdTransfer::get(gid);
  MCE_INFO("FollowRelationWriterI::remove:" << globalId << " uid:" << uid);
  ostringstream msg;
  msg << " gid:" << globalId << " uid:" << uid;
  InvokeClient ic = InvokeClient::create(current, msg.str(), InvokeClient::INVOKE_LEVEL_INFO);
  // 同时删除按gid和按uid进行散表的两份表
  MyUtil::TaskManager::instance().execute(new RemoveTask(globalId, uid));
}

/*********************ServiceSchema*****************************************/
void ServiceSchemaI::configure(const Ice::PropertiesPtr& props) {
  using namespace MyUtil;
  TaskManager::instance().config(TASK_LEVEL_FOLLOWRELATION_WRITER_ADD, ThreadPoolConfig(1, 5));
  TaskManager::instance().config(TASK_LEVEL_FOLLOWRELATION_WRITER_REMOVE, ThreadPoolConfig(1, 5));
}

/*****************************************************************************/

bool AddTask::addToMemberTable() {
  using namespace com::xiaonei::xce;
  bool result = false;

  Statement sql;
  sql << "INSERT INTO member_" << gid_ % 100 << " VALUES( " << gid_ << "," << uid_ << ", " << timeStr_
      << ") ON DUPLICATE KEY UPDATE followtime=VALUES(followtime)";

  ostringstream pattern;
  pattern << "member_" << (gid_ % 100);
  try {
    MyUtil::TimeCost tc = MyUtil::TimeCost::create("add followrelation to DB:", MyUtil::TimeCost::LOG_LEVEL_DEBUG);
    QueryRunner("relation_center", CDbWServer, pattern.str()).execute(sql);
    //  更新Cache中的数据
    MCE_DEBUG("FollowRelationCacheAdapter::instance().reload gid:" << gid_);
    FollowRelationCacheAdapter::instance().reload(gid_);
    result = true;
  } catch (std::exception& e) {
    MCE_ERROR("add FollowRelation into DB Failure! gid:" << gid_ << " uid:" << uid_ << " timeStr:" << timeStr_
        << " reason:" << e.what());
  }

  return result;
}

bool AddTask::addToUserMemberTable() {
  using namespace com::xiaonei::xce;
  bool result = false;

  Statement sql;
  sql << "INSERT INTO user_member_" << uid_ % 100 << " VALUES( " << gid_ << "," << uid_ << "," << timeStr_
      << ") ON DUPLICATE KEY UPDATE followtime=VALUES(followtime);";

  ostringstream pattern;
  pattern << "user_member_" << (uid_ % 100);
  try {
    MyUtil::TimeCost tc = MyUtil::TimeCost::create("add followrelation to DB user_member:",
        MyUtil::TimeCost::LOG_LEVEL_DEBUG);
    QueryRunner("relation_center_user", CDbWServer, pattern.str()).execute(sql);
    //  更新Cache中的数据
    MyUtil::GlobalIdPtr globalId = MyUtil::GlobalIdTransfer::get(gid_);
    int type = globalId->type;
    MCE_DEBUG("FollowResourceCacheAdapter::instance().reload uid:" << uid_ << " type:" << type);
    FollowResourceCacheAdapter::instance().reload(uid_, type);
    result = true;
  } catch (std::exception& e) {
    MCE_ERROR("add FollowRelation into DB user_member Failure! gid:" << gid_ << " uid:" << uid_ << " timeStr:"
        << timeStr_ << " reason:" << e.what());
  }

  return result;
}

// ?? 超时?
void AddTask::handle() {
  if (addToMemberTable() == true) {
    addToUserMemberTable();
  }
}

bool RemoveTask::removeFromMemberTable() {
  using namespace com::xiaonei::xce;
  Statement sql;
  bool result = false;

  sql << "DELETE FROM member_" << gid_ % 100 << " WHERE object_id = " << gid_ << " and userid = " << uid_;

  ostringstream pattern;
  pattern << "member_" << (gid_ % 100);
  try {
    MyUtil::TimeCost tc = MyUtil::TimeCost::create("remove followrelation from DB:", MyUtil::TimeCost::LOG_LEVEL_DEBUG);
    QueryRunner("relation_center", CDbWServer, pattern.str()).execute(sql);
    //  更新Cache中的数据
    MCE_DEBUG("FollowRelationCacheAdapter::instance().reload gid:" << gid_);
    FollowRelationCacheAdapter::instance().reload(gid_);
    result = true;
  } catch (std::exception& e) {
    MCE_ERROR("remove FollowRelation from DB Failure:" << gid_ << " uid:" << uid_ << " reason:" << e.what());
  }
  return result;
}

bool RemoveTask::removeFromUserMemberTable() {
  using namespace com::xiaonei::xce;
  bool result = false;

  Statement sql;
  sql << "DELETE FROM user_member_" << uid_ % 100 << " WHERE object_id = " << gid_ << " and userid = " << uid_;
  ostringstream pattern;
  pattern << "user_member_" << (uid_ % 100);
  try {
    MyUtil::TimeCost tc = MyUtil::TimeCost::create("remove followrelation from DB user_member:",
        MyUtil::TimeCost::LOG_LEVEL_DEBUG);
    QueryRunner("relation_center_user", CDbWServer, pattern.str()).execute(sql);
    //  更新Cache中的数据
    MyUtil::GlobalIdPtr globalId = MyUtil::GlobalIdTransfer::get(gid_);
    int type = globalId->type;
    MCE_DEBUG("FollowResourceCacheAdapter::instance().reload uid:" << uid_ << " type:" << type);
    FollowResourceCacheAdapter::instance().reload(uid_, type);
    result = true;
  } catch (std::exception& e) {
    MCE_ERROR("remove FollowRelation from DB user_member Failure:" << gid_ << " uid:" << uid_ << " reason:" << e.what());
  }

  return result;
}

void RemoveTask::handle() {
  if (removeFromMemberTable() == true) {
    removeFromUserMemberTable();
  }
}

}//namespace buddy
}//namespace xce


