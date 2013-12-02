#include "TalkCheckAddFriendAdapter.h"

using namespace MyUtil;
using namespace com::renren::antispam::friendcode::generation;
using namespace std;
using namespace talk::adapter;

CheckAddFriendPrx TalkCheckAddFriendAdapter::getManager(int id) {
  return locate<CheckAddFriendPrx> (_managers, "CheckAddFriend", id, TWO_WAY);
}

CheckAddFriendPrx TalkCheckAddFriendAdapter::getManagerOneway(int id) {
  return locate<CheckAddFriendPrx> (_managersOneway, "CheckAddFriend", id, ONE_WAY);
}

bool TalkCheckAddFriendAdapter::isNeedCode(int userId){
  return getManager(userId)->isNeedCode(userId);
}


