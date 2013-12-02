#ifndef AD_MATCHN_ADAPTER_H_
#define AD_MATCHN_ADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "AdEngine.h"

namespace xce {
namespace ad {

using namespace MyUtil;

class AdMatchNAdapter: public MyUtil::AdapterI,
    public MyUtil::AdapterISingleton<MyUtil::AdChannel, AdMatchNAdapter> {
public:
  AdMatchNAdapter() {
    _managers.resize(cluster() > 0 ? cluster() : 1);
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
  }

  AdResultMap GetAds(const string & user_profile, const string & referer, int rotate_index, bool hide = false);
  void Click(int uid, const string & ip, const string & text, const string & referer, const string & click_url);

  void BindUser(int userid, Ice::Long creativeid, Ice::Long zoneid);
  void UnbindUser(int userid, Ice::Long zoneid);
  void NotifyBindUser(int userid, Ice::Long creativeid, Ice::Long zoneid);
  void NotifyUnbindUser(int userid, Ice::Long zoneid);

private:
  AdMatchManagerPrx getManager(int id);
  AdMatchManagerPrx getManagerOneway(int id);

  virtual string name() {
    return "M";
  }
  virtual string endpoints() {
    return "@AdMatchN";
  }
  virtual size_t cluster() {
    return 10;
  }

  vector<AdMatchManagerPrx> _managersOneway;
  vector<AdMatchManagerPrx> _managers;
};

}
}


#endif // AD_MATCHN_ADAPTER_H_
