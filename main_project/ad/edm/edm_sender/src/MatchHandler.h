/*
 * MatchHandlerParamter.h
 *
 *  Created on: 2012-2-20
 *      Author: sunzz
 */

#ifndef MATCHHANDLERPARAMTER_H_
#define MATCHHANDLERPARAMTER_H_

#include "EdmStruct.h"
#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>
#include "feed/FeedMemcProxy/client/user_profile_client.h"
#include "../../../util/src/AdCommonUtil.h"
//#include "AdEffect.h"

namespace xce {
namespace ad {

using namespace MyUtil;
using namespace std;


class MatchHandlerParamter {
public:
  MatchHandlerParamter(const UserProfile& userProfile, int user_status):
                       user_status_(user_status), userProfile_(userProfile){
  }
  int count() const {
    return 1;
  }
  unsigned int PickCount() const {  // 返回还需要选取的广告数
    return count() - selected_groups_.size();
  }
  int user_id() {return userProfile_.id();}


  set<SelectedAdGroupPtr> selected_groups() {
    return selected_groups_;
  }
  void set_selected_groups(set<SelectedAdGroupPtr> selected_groups) {
    selected_groups_ = selected_groups;
  }
  UserProfile userProfile() {
    return userProfile_;
  }
  int userStatus() {
    return user_status_;
  }
private:
  int user_status_;
  UserProfile userProfile_;
  set<SelectedAdGroupPtr> selected_groups_;

};

class EdmMatchHandler : public IceUtil::Shared {
  public:
    virtual bool handle(MatchHandlerParamter& para) = 0;
};

typedef IceUtil::Handle<EdmMatchHandler> EdmMatchHandlerPtr;

}
}
#endif /* MATCHHANDLERPARAMTER_H_ */
