#include "IMIdGeneratorManagerI.h"
#include "QueryRunner.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"


using namespace std;
using namespace xce::xntalk;
using namespace com::xiaonei::xce;

void MyUtil::initialize() {
  ServiceI& service = ServiceI::instance();
  service.getAdapter()->add(&IMIdGeneratorManagerI::instance(), service.createIdentity("M", ""));
    
//  ServiceI& service = ServiceI::instance();
  int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("Service.Mod");
  int interval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Service.Interval", 5);
  xce::serverstate::ServerStateSubscriber::instance().initialize("ControllerIMIdGenerator", 
      &IMIdGeneratorManagerI::instance(), mod, interval, new XceFeedControllerChannel());
  MCE_INFO("MyUtil::initialize. mod:" << mod << " interval:" << interval);

  IMIdGeneratorManagerI::instance().InitalizeIdItems();
}

/*说明：初始化序列容器
 */
void IMIdGeneratorManagerI::InitalizeIdItems() {   
  for (int i=0; i<BUFFER_MAXSIZE; ++i) {
    id_items_[i] = new IdItem();
    IceUtil::Mutex::Lock lock(id_items_[i]->mutex_); 
    id_items_[i]->current_index_ = -1; 
    id_items_[i]->top_index_ = -1; 
    id_items_[i]->count_ = 0; 
  }
}

/*说明：创建新的类型
 *参数1：创建的类型
 *参数2：起始id
 *参数3：类型描述
 *返回值：创建结果
 */
Ice::Long IMIdGeneratorManagerI::createType(Ice::Int type, Ice::Long id, const string &note,  const Ice::Current& current) {
  Statement sql;
  sql << "INSERT INTO " << ID_TABLE <<"(type, id, note) VALUES (" << type <<", "<< id << ",'" << note <<"')";
  try {
    QueryRunner(DB_INSTANCE, CDbWServer, ID_TABLE).store(sql);
  } catch (std::exception &e) {
    MCE_WARN("IMIdGeneratorManagerI::createType: "<<e.what());
    return false;    
  } catch (...) {
    MCE_WARN("IMIdGeneratorManagerI::createType: exception");
    return false;
  }  
  MCE_INFO("IMIdGeneratorManagerI::createType  Message---->succeed to insert  type:"<<type<<"   id:"<<id << "   note:"<<note);
  return true;
}

/*说明：根据类型获取新的id
 *参数1：类型
 *返回值：返回新的唯一id
 */
Ice::Long IMIdGeneratorManagerI::getId(Ice::Int type,  const Ice::Current &current) {
  if (type >= BUFFER_MAXSIZE || type<0)
    return -1;
  {
    IceUtil::Mutex::Lock lock(id_items_[type]->mutex_);
    if (++(id_items_[type]->count_) > 1000) {
      MCE_WARN("IMIdGeneratorManagerI::getId --> IdSeqDb is blocked");
      --(id_items_[type]->count_);
      return -1;
    }
  }
  {
    IceUtil::Mutex::Lock lock(id_items_[type]->mutex_);
    while (1) {
      MCE_DEBUG("current_index:" << id_items_[type]->current_index_ << ", topIndex:" << id_items_[type]->top_index_
          << " count:" <<id_items_[type]->count_ );
      if (id_items_[type]->current_index_ != -1 && id_items_[type]->current_index_ < id_items_[type]->top_index_) {
        --id_items_[type]->count_;
        return id_items_[type]->current_index_++;
      }
      {
        Statement sql;
//        sql << "update " << ID_TABLE << " SET id = id +"            << ID_SEQ_STEP <<" WHERE type=" <<type;
        sql << "INSERT INTO " << ID_TABLE << "(type, id) VALUES(" << type << ","<<(BEGIN_ID+ID_SEQ_STEP)<<") ON DUPLICATE KEY UPDATE id=id+" << ID_SEQ_STEP;
        QueryRunner(DB_INSTANCE, CDbWServer, ID_TABLE).store(sql);
      }
      {
        Statement sql;
        sql << "SELECT id FROM " << ID_TABLE << "  WHERE type=" << type;
        mysqlpp::StoreQueryResult res = QueryRunner(DB_INSTANCE, CDbRServer, ID_TABLE).store(sql);
        if (!res) {
          --id_items_[type]->count_;
          return -1;
        }
        for (size_t i = 0; i < res.num_rows(); ++i) {
          mysqlpp::Row row = res.at(i);
          id_items_[type]->top_index_ = (Ice::Long) row["id"];
          id_items_[type]->current_index_ = id_items_[type]->top_index_ - ID_SEQ_STEP;
          MCE_DEBUG("top_index:" << id_items_[type]->top_index_);
          break;
        }
      }
    }
  }
}
