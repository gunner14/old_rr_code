#ifndef XCE_ACCOUNT_ACCOUNTFILTERPRELOADER_H_
#define XCE_ACCOUNT_ACCOUNTFILTERPRELOADER_H_

#include <ServiceI.h>
#include <QueryRunner.h>
#include "GeneralPreloader/src/GeneralPreloaderFactory.h"
#include <AccountFilter.h>

namespace xce {
namespace account {

const static std::string TABLE = "user_passport";
const static std::string DB_SOURCE = "user_passport_preloader";

const static int PRODUCE_BATCH_SIZE = 1000;
const static int CONSUME_BATCH_SIZE = 2000;
const static int CONSUME_BATCH_THREAD_SIZE = 5;
const static int WRITE_BATCH_SIZE = 3000;
const static int PRX_DATAQUEUE_MAX_SIZE = 100000;

class AccountFilterPreloader : virtual public Ice::Object,
  virtual public MyUtil::Singleton<AccountFilterPreloader> {
public:
  AccountFilterPreloader () {}
  virtual ~AccountFilterPreloader () {}
};

class AccountFilterPreloaderFactory : virtual public xce::generalpreloader::MinMaxFactory {
public:
  AccountFilterPreloaderFactory() {}
  virtual ~AccountFilterPreloaderFactory() {}
  virtual MyUtil::ObjectResultPtr create(const MyUtil::IntSeq& ids);
  virtual MyUtil::ObjectResultPtr create(Ice::Int minId,Ice::Int maxId);
};

class BatchAccountFilterPreloaderResultHandler : public com::xiaonei::xce::ResultHandler {
public:
  BatchAccountFilterPreloaderResultHandler (MyUtil::ObjectResultPtr& results);
  virtual bool handle(mysqlpp::Row& row) const;
private:
  MyUtil::ObjectResultPtr results_;
};

}
}

#endif
