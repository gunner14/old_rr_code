#include "UgcMessageRecordAdapter.h"
using namespace renren::com::message::client;


MessageAdapterPrx UgcMessageRecordAdapter::getManager(int id) {
  return locate<MessageAdapterPrx>(_managers, "M", id, TWO_WAY, 500);
}

MessageAdapterPrx UgcMessageRecordAdapter::getManagerOneway(int id) {
  return locate<MessageAdapterPrx>(_managersOneway, "M", id, ONE_WAY, 300);
}

void UgcMessageRecordAdapter::saveMessge(const MessageVO &msg) {
  getManagerOneway(msg.senderId)->saveMessge(msg);
}


