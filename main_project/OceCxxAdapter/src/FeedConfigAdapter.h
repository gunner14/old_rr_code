#ifndef FEEDDISPATCHERADAPTER_H_
#define FEEDDISPATCHERADAPTER_H_

#include "RFeed.h"
#include <vector>
#include "Singleton.h"
#include "AdapterI.h"
#include "FeedConfig.h"

namespace xce {
namespace feed {

using namespace MyUtil;

class ConfigManagerAdapter: public MyUtil::ReplicatedClusterAdapterI<ConfigManagerPrx>,
		public MyUtil::Singleton<ConfigManagerAdapter> {
public:
  ConfigManagerAdapter():MyUtil::ReplicatedClusterAdapterI<ConfigManagerPrx> ("ControllerFeedConfigR",120,300,new XceFeedControllerChannel()) {
  }
  
  void SetSendConfig(int uid, int type, int appid, bool newsconfig);
  bool GetSendConfig(int uid, int type,int appid);

  void SetRecvConfig(int uid, int type, int appid, bool newsconfig);
  MyUtil::IntArray FilterRecvConfig(MyUtil::IntArray target,int type,int appid);

  UserConfigDict GetUserBlockedAppDict(int uid); 

protected:

	ConfigManagerPrx getManager(Ice::Long id);
	ConfigManagerPrx getManagerOneway(Ice::Long id);

};

};
};

#endif /*BUDDYCOREADAPTER_H_*/
