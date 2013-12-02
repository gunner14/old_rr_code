#ifndef XCE_TRIPOD_TICKET_TICKETMANAGER_H_
#define XCE_TRIPOD_TICKET_TICKETMANAGER_H_

#include "TripodTicket.h"
#include "Ice/Ice.h"
#include "IceUtil/Mutex.h"
#include "UtilCxx/src/TaskManager.h"
#include "IceExt/src/ServiceI.h"
#include "IceExt/src/TempMutex.h"
#include "tripod2/cc/core/tripod_client.h"
#include "DbCxxPool/src/QueryRunner.h"

namespace xce {
namespace tripod {
namespace ticket {

  //服务配置
  const string SERVICE_IDENTITY = "M";
  const string SERVER_CONFIG_PREFIX = "TripodTicketManager";
  const string CONTROLLER = "Controller";
  const string NAMESPACE_DEFAULT = "xce_ticket";

  //TicketManagerI
  //--------------------------------------------------------------
  class TicketManagerI: virtual public TicketManager, public MyUtil::Singleton<TicketManagerI> {
  public:
    TicketManagerI();
    void init(const string& ns, const string& biz);
    string createTicket(const MyUtil::Str2StrMap& props, int expiredTime, const Ice::Current& = Ice::Current());
    bool destroyTicket(const string& ticket, const Ice::Current& = Ice::Current());
    bool destroyTicketById(long id, const MyUtil::IntSeq& types, const Ice::Current& = Ice::Current());

    bool isValid(const Ice::Current& = Ice::Current());
    void setValid(bool newState, const Ice::Current& = Ice::Current());

  private:
    xce::tempmutex::TempMutexManagerPtr _tempMutexManager;
    xce::tripod::TripodClient* _tripodClient;
    string _table;
    string _biz;
  };

  class BatchValidTicketHandlerI: public com::xiaonei::xce::ResultHandler {
  public:
    BatchValidTicketHandlerI(MyUtil::Str2IntMap& results);
    virtual bool handle(mysqlpp::Row& row) const;
  private: 
    MyUtil::Str2IntMap& _results;
  };

}  //namespace ticket
}  //namespace tripod
}  //namespace xce
#endif
