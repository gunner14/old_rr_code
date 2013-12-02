#ifndef __XCE_TRIPOD_USERDESC_USERTIMETTRIPODPRODUCER_H__
#define __XCE_TRIPOD_USERDESC_USERTIMETTRIPODPRODUCER_H__
 
#include "app/Utils/src/DBUtils.h"
#include "QueryRunner.h"
#include "UserDesc.pb.h"
#include "TripodProducer.h"

namespace xce {
namespace tripod {
namespace userdesc {

using namespace std;
using namespace com::renren::tripod;

const string DB_TRIPOD_USERTIME = "user_time";
const string TABLE_TRIPOD_USERTIME = "user_time";

const string UTID = "id";
const string UTREGISTERTIME = "registertime";
const string UTACTIVETIME = "activatetime";
const string UTLASTLOGINTIME = "lastlogintime";
const string UTLASTLOGOUTTIME = "lastlogouttime";

class UserTimeTripodProducer : virtual public TripodProducer {
public:
  UserTimeTripodProducer() {}
  virtual ~UserTimeTripodProducer() {}

  virtual DataMap create(const KeySeq& keys, bool isMaster);
  virtual KeySeq createHotKeys(const string& beginKey, int limit);
  virtual KeySeq createKeys(const string& beginKey, int limit);
};

class BatchUserTimeHandlerI: public com::xiaonei::xce::ResultHandler {
public:
  BatchUserTimeHandlerI(com::renren::tripod::DataMap& results);
  virtual bool handle(mysqlpp::Row& row) const;
private: 
  com::renren::tripod::DataMap& _results;
};

}
}
}
#endif
