#include "TripodTicketAdapter.h"
#include "tripod_service/Ticket/share/TicketShare.cpp"

using namespace MyUtil;
using namespace xce::tripod::ticket;
using namespace xce::adapter::ticket;
using namespace com::renren::tripod;
using namespace xce::tripoddata::ticket;

void TicketAdapter::initialize() {
}

TicketAdapter::TicketAdapter(const string& address, const string& space, const string& type) :
    MyUtil::ReplicatedClusterAdapterI<TicketManagerPrx> ("Controller" + type, 120, 300) {
  _tripodClient = new TripodClient(address, space, type);
}

TicketAdapter::~TicketAdapter() {
  delete _tripodClient;
}

string TicketAdapter::createTicket(const Str2StrMap& props, unsigned expiredTime) {
  return getProxy(0)->createTicket(props, expiredTime);
}

long TicketAdapter::verifyTicket(const string& ticket, const IntSeq& types) {
  long result = -1;
  if(TicketUtil::isSTicketLegal(ticket)) {
    KeySeq keys, missedKeys;
    keys.push_back(ticket);
    DataMap dataMap = _tripodClient->get(keys, missedKeys);
    DataMap::const_iterator itm = dataMap.find(ticket);
    if(itm != dataMap.end()) {
      TicketData data;
      string value(itm->second.begin(), itm->second.end());
      istringstream in(value);
      data.ParseFromIstream(&in);
      int type = data.type();
      for(IntSeq::const_iterator it = types.begin(); it != types.end(); ++it) {
        if(type == *it) {
          result = data.id();
          break;
        }
      }
    }
  }
  return result;
}

TicketData TicketAdapter::queryTicket(const string& ticket) {
  TicketData data;
  if(TicketUtil::isSTicketLegal(ticket)) {
    KeySeq keys, missedKeys;
    keys.push_back(ticket);
    DataMap dataMap = _tripodClient->get(keys, missedKeys);
    DataMap::const_iterator it = dataMap.find(ticket);
    if(it != dataMap.end()) {
      string value(it->second.begin(), it->second.end());
      istringstream in(value);
      data.ParseFromIstream(&in);
    }
  }
  return data;
}

bool TicketAdapter::destroyTicket(const string& ticket) {
  int id = TicketUtil::getId(ticket);
  return getProxy(id)->destroyTicket(ticket);
}

bool TicketAdapter::destroyTicketById(long id, const IntSeq& types) {
  return getProxy(id)->destroyTicketById(id, types);
}
