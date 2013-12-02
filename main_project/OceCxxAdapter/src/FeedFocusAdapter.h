
#ifndef __FEEDFOCUSADAPTER_H_
#define __FEEDFOCUSADAPTER_H_

#include "Singleton.h"
#include "AdapterI.h"
#include "RFeed.h"

namespace xce {
namespace feed {

using namespace MyUtil;

class FeedFocusAdapter: public MyUtil::AdapterI,
    public MyUtil::AdapterISingleton<MyUtil::XceFeedChannel, FeedFocusAdapter> {
public:
  FeedFocusAdapter() {
    _managers.resize(cluster() > 0 ? cluster() : 1);
    _managersOneway.resize(cluster() > 0 ? cluster() : 1);
  }

  FeedDataSeq GetFocusFeedData(int user, int begin,int limit);
  FeedDataSeq GetFocusFeedDataByStype(int user, int begin,int limit,const vector<int> & stypes);
  IntSeq GetFocuses(int user);
  int GetFocusFeedIncCount(int user);

  bool AddFocuses(int user, int target);
  /** 
   * AddFocusWithCheck验证user和target是否是好友关系，AddFocuses不进行验证。
   * 成功返回值0：添加成功
   * 失败返回值1：关注数量已达上限，添加失败
   * 失败返回值2：已关注过该好友，添加失败
   * 失败返回值3：欲添加的用户非当前用户的好友，添加失败
   * 失败返回值4：其它原因导致的添加失败
   */
  int AddFocusWithCheck(int user, int target);
  void DelFocuses(int user, int target);

  FocusManagerPrx getManager(int id);
  FocusManagerPrx getManagerOneway(int id);

protected:
  virtual string name() {
    return "FeedFocus";
  }
  virtual string endpoints() {
    return "@FeedFocus";
  }
  virtual size_t cluster() {
    return 5;
  }

  vector<FocusManagerPrx> _managersOneway;
  vector<FocusManagerPrx> _managers;
};

}
;
}
;
#endif /* __FEEDFOCUSADAPTER_H_ */
