#ifndef __XCE_ADAPTER_TICKET_TRIPODTICKETADAPTER_H__
#define __XCE_ADAPTER_TICKET_TRIPODTICKETADAPTER_H__

#include "AdapterI.h"
#include "Singleton.h"
#include "TripodTicket.h"
#include "tripod-new/src/cpp/include/TripodClient.h"
#include "Ticket.pb.h"


namespace xce {
namespace adapter {
namespace ticket {

enum Type {
Web = 0, 
Wap,
PlatForm,
Client,
MobileClient
};

using namespace std;

//接口
class TicketAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::tripod::ticket::TicketManagerPrx> {
public:
  TicketAdapter(const string& address, const string& space, const string& type); 
  virtual ~TicketAdapter();
  string createTicket(const MyUtil::Str2StrMap& props, unsigned expiredTime);
  long verifyTicket(const string& ticket, const MyUtil::IntSeq& types); //验票，通过后返回展示用户id
  xce::tripoddata::ticket::TicketData queryTicket(const string& ticket);
  bool destroyTicket(const string& ticket);
  bool destroyTicketById(long id, const MyUtil::IntSeq& types); //删除一个展示用户id所有对应域下的票

protected:
  virtual void initialize();

private:
  com::renren::tripod::TripodClient* _tripodClient;
};

//T票使用此接口
class TemporaryTicketAdapter : public TicketAdapter, public MyUtil::Singleton<TemporaryTicketAdapter> {
public:
  TemporaryTicketAdapter() : TicketAdapter("tdtest2:2181/Tripod", "test", "TTicket") {}
};

//P票使用此接口
class PersistentTicketAdapter : public TicketAdapter, public MyUtil::Singleton<PersistentTicketAdapter>  {
public:
  PersistentTicketAdapter() : TicketAdapter("tdtest2:2181/Tripod", "test", "PTicket") {}
};

}  //namespace ticket
}  //namespace adapter
}  //namespace xce

#endif
