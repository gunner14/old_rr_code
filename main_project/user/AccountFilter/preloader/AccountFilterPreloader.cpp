#include "AccountFilterPreloader.h"
#include "GeneralPreloader/src/GeneralPreloader.h"

using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace std;
using namespace mysqlpp;
using namespace xce::generalpreloader;
using namespace xce::account;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&AccountFilterPreloader::instance(), service.createIdentity("AF", ""));
	string ctlEndpoint = ServiceI::instance().getCommunicator()->getProperties()->getPropertyWithDefault("AccountFilter.Controller", "ControllerAccountFilter");
  int subInterval = ServiceI::instance().getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Subscriber.Controller.Interval", 5);
  int timeout = ServiceI::instance().getCommunicator()->getProperties()->getPropertyAsIntWithDefault("AccountFilter.Proxy.Timeout", 300);

  int cluster= 100;

  GeneralPreloader<xce::account::AccountFilterPrx>::instance().initialize(
      new BatchUsersIdProducer,
      new AccountFilterPreloaderFactory,
      ctlEndpoint,
      cluster,
      subInterval,
      timeout,
      PRX_DATAQUEUE_MAX_SIZE ,
      PRODUCE_BATCH_SIZE ,
      CONSUME_BATCH_SIZE ,
      WRITE_BATCH_SIZE ,
      CONSUME_BATCH_THREAD_SIZE);
}

ObjectResultPtr AccountFilterPreloaderFactory::create(Ice::Int minId,Ice::Int maxId) {
  MCE_INFO("AccountFilterPreloaderFactory::create group -> id from: "<<minId <<" to: "<<maxId);
  ObjectResultPtr results = new ObjectResult;
  BatchAccountFilterPreloaderResultHandler handler(results );
  Statement sql;
  sql << "SELECT id, account FROM " << TABLE << " WHERE id >= "<< minId <<" AND id <= "<< maxId;
  QueryRunner(DB_SOURCE, CDbRServer ).query(sql, handler);
  return results;
}

ObjectResultPtr AccountFilterPreloaderFactory::create(const MyUtil::IntSeq& ids) {
  MCE_DEBUG("AccountFilterPreloader::create group -> id size: "<<ids.size());
  ObjectResultPtr results = new ObjectResult;
  BatchAccountFilterPreloaderResultHandler handler(results );
  Statement sql;
  sql << "SELECT id, account FROM " << TABLE << "WHERE id IN (";
  for (MyUtil::IntSeq::const_iterator it = ids.begin(); it != ids.end(); ++it) {
    if (it != ids.begin()) {
      sql << ",";
    }
    sql << *it;
  }
  sql << ")";
  QueryRunner(DB_SOURCE,CDbRServer).query(sql, handler);
  return results;
}

BatchAccountFilterPreloaderResultHandler::BatchAccountFilterPreloaderResultHandler(
    MyUtil::ObjectResultPtr& results) :
  results_(results) {
}

bool BatchAccountFilterPreloaderResultHandler::handle(mysqlpp::Row& row) const {
  int id = (int) row["id"];
  MyUtil::ObjectMap::iterator it = results_->data.find(id);
  if (it == results_->data.end()) {
    xce::account::DBAccountPtr rs = new xce::account::DBAccount;
    rs->id = id;
    rs->account = (string)row["account"];
    results_->data.insert(make_pair(id, rs));
  } else {
    MCE_WARN("unreachable code segment " << __FILE__ << " " << __LINE__);
  }
  return true;
}

