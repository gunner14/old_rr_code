#include "ManagerI.h"
#include "IceLogger.h"
#include "MessageType.h"
#include "Request.h"
#include "Server.h"
#include "ClientBuffer.h"
using namespace com::xiaonei::talk::common;
using namespace com::xiaonei::talk::util;
using namespace talk::http;
using namespace MyUtil;

void ManagerI::deliver(const MessageSeq& mseq, const Ice::Current&) {
	//MCE_INFO("ManagerI::deliver --> deliver a message to jsclient size=" << mseq.size());
	for (size_t i = 0; i < mseq.size(); ++i) {
		ClientPoolManager::instance().getClientPool(mseq.at(i)->to->index).pushMsg(mseq.at(i)->to->index, mseq.at(i));
	}
}

bool ManagerI::indexExist(Ice::Long index, const Ice::Current&){
	return Server::instance().indexExist(index);
}

bool ManagerI::sessionExist(Ice::Long index, const Ice::Current&){
  JidPtr j = ClientPoolManager::instance().getClientPool(index).getJid(index);
  return j == 0 ? false : true;
}


void ManagerI::kick(const JidPtr& jid, const Ice::Current&){

}

DeliverStatList ManagerI::GetDeliverStat(const Ice::Current&) {
  return Server::instance().getDeliverStat();
}
void ManagerI::ClearDeliverStat(const Ice::Current&) {
  Server::instance().clearDeliverStat();
}



