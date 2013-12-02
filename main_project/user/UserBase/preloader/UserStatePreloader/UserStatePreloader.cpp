#include "UserStatePreloader.h"
#include <user/UserBase/hotback/oldwr/UserBaseDataI.cpp>
#include "GeneralPreloader/src/GeneralPreloader.h"

using namespace MyUtil;
using namespace com::xiaonei::xce;
using namespace std;
using namespace mysqlpp;
using namespace xce::generalpreloader;
using namespace xce::userstatepreloader;
using namespace xce::userbase;

void MyUtil::initialize() {
	ServiceI& service = ServiceI::instance();
	service.getAdapter()->add(&UserStatePreloader::instance(), service.createIdentity("M", ""));
	string ctlEndpoint = ServiceI::instance().getCommunicator()->getProperties()->getPropertyWithDefault("UserBase.Controller", "ControllerUserBase");
        int subInterval = ServiceI::instance().getCommunicator()->getProperties()->getPropertyAsIntWithDefault("Subscriber.Controller.Interval", 5);
        int timeout = ServiceI::instance().getCommunicator()->getProperties()->getPropertyAsIntWithDefault("UserBase.Proxy.Timeout", 300);

		int cluster = 100;
		GeneralPreloader<UserStateManagerPrx>::instance().initialize(
                new BatchUsersIdProducer,
                new UserStatePreloaderFactory,
                ctlEndpoint, 
				cluster,
				subInterval,
                timeout,
				PRX_DATAQUEUE_MAX_SIZE ,
                PRODUCE_BATCH_SIZE ,
                CONSUME_BATCH_SIZE ,
				WRITE_BATCH_SIZE ,
                CONSUME_BATCH_THREAD_SIZE,
				"USM",true);
}


ObjectResultPtr UserStatePreloaderFactory::create(Ice::Int minId,Ice::Int maxId) {
	MCE_INFO("UserStatePreloaderFactory::create group -> id from: "<<minId <<" to: "<<maxId);
	
	ObjectResultPtr results = new ObjectResult;
	
	BatchUserStatePreloaderResultHandler handler(results );
	Statement sql;
	sql << "SELECT id, state, star, level FROM " << TABLE_USER_STATE << " WHERE id >= "<< minId <<" AND id <= "<< maxId;
	QueryRunner(DB_USER_STATE, CDbRServer ).query(sql, handler);
	
	return results;
}

ObjectResultPtr UserStatePreloaderFactory::create(const MyUtil::IntSeq& ids) {
	MCE_DEBUG("UserStatePreloader::create group -> id size: "<<ids.size());
        
	ObjectResultPtr results = new ObjectResult;
	
	BatchUserStatePreloaderResultHandler handler(results );
	
	Statement sql;
	sql << "SELECT id, state, star, level FROM " << TABLE_USER_STATE << "WHERE id IN (  ";
	for (MyUtil::IntSeq::const_iterator it = ids.begin(); it != ids.end(); ++it) {
		if (it != ids.begin()) {
			sql << ",";
		}
		sql << *it;
	}
	sql << ")";

	QueryRunner(DB_USER_STATE,CDbRServer).query(sql, handler);

	return results;
}


BatchUserStatePreloaderResultHandler::BatchUserStatePreloaderResultHandler(
                MyUtil::ObjectResultPtr& results) :
        _results(results) {
}

bool BatchUserStatePreloaderResultHandler::handle(mysqlpp::Row& row) const {
	
	int id = (int) row["id"];
    MyUtil::ObjectMap::iterator it = _results->data.find(id);
    if (it == _results->data.end())
	{
		UserStateDataIPtr rs = new UserStateDataI;
		rs->parse(row);
		_results->data.insert(make_pair(id, rs));
    } else 
	{
		MCE_WARN("unreachable code segment " << __FILE__ << " " << __LINE__);
    }
    return true;
}

