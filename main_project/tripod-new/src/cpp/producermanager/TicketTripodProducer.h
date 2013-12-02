#ifndef __XCE_TRIPOD_TICKET_TICKETTRIPODPRODUCER_H__
#define __XCE_TRIPOD_TICKET_TICKETTRIPODPRODUCER_H__
 
#include "TripodProducer.h"
#include "tripod_service/Ticket/share/TicketShare.h"

namespace xce {
namespace tripod {
namespace ticket {

using namespace std;
using namespace com::renren::tripod;

class TicketTripodProducer : virtual public TripodProducer {
public:
  TicketTripodProducer() : _table(TABLE_T_TICKET) {}
  TicketTripodProducer(string table) : _table(table) {}
  virtual ~TicketTripodProducer() {}
  virtual DataMap create(const KeySeq& keys, bool isMaster);
  virtual KeySeq createHotKeys(const string& beginKey, int limit);
  virtual KeySeq createKeys(const string& beginKey, int limit);
protected:
  const string _table;
};

class TTicketTripodProducer : virtual public TicketTripodProducer {
public:
  TTicketTripodProducer() : TicketTripodProducer(TABLE_T_TICKET) {}
};

class PTicketTripodProducer : virtual public TicketTripodProducer {
public:
  PTicketTripodProducer() : TicketTripodProducer(TABLE_P_TICKET) {}
};

}
}
}
#endif
