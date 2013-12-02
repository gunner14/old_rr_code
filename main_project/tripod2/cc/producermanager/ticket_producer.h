#ifndef XCE_TRIPOD_TICKET_PRODUCER_H_
#define XCE_TRIPOD_TICKET_PRODUCER_H_

#include "tripod2/cc/core/producer.h"
#include "tripod_service/Ticket/share/ticket_const.h"
#include "DbCxxPool/src/QueryRunner.h"
#include <string>

namespace xce {
namespace tripod {

class TicketProducer : virtual public Producer {
 public:
  TicketProducer(std::string table) : _table(table) {}
  virtual bool Create(const std::string& key, ByteSeq* value, bool use_master);
  std::map<std::string, bool> Create(const StringListType& keys, String2ByteSeqMap* values, bool use_master);
 protected:
  const std::string _table;
};

class TTicketProducer : virtual public TicketProducer {
 public:
  TTicketProducer() : TicketProducer(xce::tripod::ticket::TABLE_T_TICKET) {}
};

class PTicketProducer : virtual public TicketProducer {
 public:
  PTicketProducer() : TicketProducer(xce::tripod::ticket::TABLE_P_TICKET) {}
};

class BatchTicketHandlerI: public com::xiaonei::xce::ResultHandler {
public:
  BatchTicketHandlerI(String2ByteSeqMap& results);
  virtual bool handle(mysqlpp::Row& row) const;
private: 
  String2ByteSeqMap& _results;
};


}} // xce::tripod

#endif
