#ifndef XNTALK_LOGINFEED_ADAPTER_H__
#define XNTALK_LOGINFEED_ADAPTER_H__ 

#include "Singleton.h"
#include "AdapterI.h"
#include "XNTalkLoginFeed.h"
namespace xce {
namespace xntalk {

class LoginFeedAdapter: public MyUtil::AdapterI, public MyUtil::AdapterISingleton<MyUtil::TalkChannel, LoginFeedAdapter> {
//class IMIdGeneratorAdapter: public MyUtil::ReplicatedClusterAdapterI<IMIdGeneratorManagerPrx>, public MyUtil::Singleton<IMIdGeneratorAdapter> {
public:
  
  LoginFeedAdapter() {
    _managers.resize(cluster() > 0 ? cluster() : 1);
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
  }
  
  //IMIdGeneratorAdapter() :MyUtil::ReplicatedClusterAdapterI<IMIdGeneratorManagerPrx> ("ControllerIMIdGenerator",120,300,new MyUtil::XceFeedControllerChannel) {}
  void SendFeed(int id);
  void ReloadConfig();
  void SendInstallFeed(int uid, string version);

	LoginFeedManagerPrx getManager(int id);
	LoginFeedManagerPrx getManagerOneway(int id);
private:
 
  virtual std::string name() {
    return "M";
  }
  virtual std::string endpoints() {
    return "@LoginFeed";
  }
  virtual size_t cluster() {
    return 1;
  }
  std::vector<LoginFeedManagerPrx> _managersOneway;
  std::vector<LoginFeedManagerPrx> _managers;
};

};
};
#endif

