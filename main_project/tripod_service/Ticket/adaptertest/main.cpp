#include "OceCxxAdapter/src/TripodTicketAdapter.h"
#include <iostream>

using namespace std;
using namespace MyUtil;
using namespace xce::tripod::ticket;
using namespace xce::adapter::ticket;
using namespace com::renren::tripod;
using namespace xce::tripoddata::ticket;

int main() {
  long id = 35178;
  long realId = 35178;
  int type = 1;
  string ip = "10.10.10.10";
  int expiredTime = 100;
  
  MyUtil::Str2StrMap info;
  info[CTID] = boost::lexical_cast <string>(id);
  info[CTREALID] = boost::lexical_cast <string>(realId);
  info[CTTYPE] = boost::lexical_cast <string>(type);
  info[CTIP] = ip;

  MyUtil::IntSeq types;
  types.push_back(0);
  types.push_back(1);
  types.push_back(type);
  string ticket;

  TemporaryTicketAdapter::instance().destroyTicketById(id, types);
  ticket = TemporaryTicketAdapter::instance().createTicket(info, expiredTime);
  id = TemporaryTicketAdapter::instance().verifyTicket(ticket, types);
  TemporaryTicketAdapter::instance().destroyTicket(ticket);
  cout << "ticket=" << ticket << ", id=" << id << endl;
  
  PersistentTicketAdapter::instance().destroyTicketById(id, types);
  ticket = PersistentTicketAdapter::instance().createTicket(info, expiredTime);
  id = PersistentTicketAdapter::instance().verifyTicket(ticket, types);
  PersistentTicketAdapter::instance().destroyTicket(ticket);
  cout << "ticket=" << ticket << ", id=" << id << endl;

  return 0;
}
