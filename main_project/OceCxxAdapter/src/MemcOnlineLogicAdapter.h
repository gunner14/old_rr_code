#ifndef MEMCONLINELOGICADAPTER_H_ 
#define MEMCONLINELOGICADAPTER_H_

#include "AdapterI.h"
#include "MemcOnlineLogic.h"

#include "Singleton.h"

namespace com {
namespace xiaonei {
namespace talk {
namespace memc{
namespace online{
using namespace MyUtil;
using namespace com::xiaonei::talk::common;

class MemcOnlineLogicAdapter : public AdapterI, public AdapterISingleton<MyUtil::TalkChannel, MemcOnlineLogicAdapter> {
public:
  MemcOnlineLogicAdapter() {
		_managersOneway.resize(cluster() > 0 ? cluster() : 1);
		_managers.resize(cluster() > 0 ? cluster() : 1);
	}
	

	AvatarNMap GetUsersOnlineInfo(const MyUtil::IntSeq& uids);
	AvatarNSeq GetUserOnlineInfo(int userid);
	int GetUserOnlineType(int userid);
	int GetUserOnlineStat(int userid);
	void SetUserOnlineInfo(int userid, const AvatarNSeq& seq);
	//void AvatarOnline(const JidPtr& jid, const string& show, const string& ver, const string& ext);
//===================================UDP
	
	void UDP_SetUserOnlineInfo(int userid, const AvatarNSeq& seq);
	void AvatarNOnline(const AvatarNPtr& avatarn);
	void AvatarNOffline(const JidPtr& jid);


protected:
	virtual string name() {
		return "MemcOnlineLogic";
	}
	virtual string endpoints() {
		return "@MemcOnlineLogic";
	}
	;
	virtual size_t cluster() {
		return 10;
	}

	MemcOnlineLogicManagerPrx getManagerOneway(int id);
	MemcOnlineLogicManagerPrx getManager(int id);
	MemcOnlineLogicManagerPrx getManagerUdp(int id);

	vector<MemcOnlineLogicManagerPrx> _managersOneway;
	vector<MemcOnlineLogicManagerPrx> _managers;
	vector<MemcOnlineLogicManagerPrx> _udpmanagers;
};

}
;
}
;
}
;
};
};

#endif /*TALKCACHEADAPTER_H_*/
