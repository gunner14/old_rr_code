#include <iostream>
#include "../RelationNotifyAdapter.h"
#include "../BuddyCoreLogicAdapter.h"

//using namespace ::com::xiaonei::service::buddy;

int main(int argc, char *argv[]) {
  int arg1, arg2, arg3;

  if (argc < 4) {
    printf("Usage: %s type, from, target\n", argv[0]);
    exit(1);
  }

  arg1 = 0;
  arg2 = 0;
  arg3 = 0;

  if (argc >= 2) {
    arg1 = atoi(argv[1]);
  }

  if (argc >= 3) {
    arg2 = atoi(argv[2]);
  }

  if (argc >= 4) {
    arg3 = atoi(argv[3]);
  }

  using namespace ::wap::notify;
  try {

    int from = arg2;
    int target = arg3;

    ::com::xiaonei::service::buddy::Applyship apply;
    apply.applicant = from;
    apply.accepter = target;


    ::com::xiaonei::service::buddy::Friendship buddy;
    buddy.from = from;
    buddy.to = target;

    ::com::xiaonei::service::buddy::Blockship block;
    block.from = from;
    block.to = target;


    std::map<std::string, std::string> abc;

    FriendRelationType type;
    switch (arg1) {
    case 0:
      type = Become;
      ::com::xiaonei::service::buddy::BuddyCoreLogicAdapter::instance().acceptApply(apply, abc);
      break;
    case 1:
      type = Refuse;
      ::com::xiaonei::service::buddy::BuddyCoreLogicAdapter::instance().denyApply(apply, abc);

      break;
    case 2:
      type = ::wap::notify::Request;
      ::com::xiaonei::service::buddy::BuddyCoreLogicAdapter::instance().addApply(apply, abc);

      break;
    case 3:
      type = Remove;
      ::com::xiaonei::service::buddy::BuddyCoreLogicAdapter::instance().removeFriend(buddy, abc);

      break;
    case 4:
      type = Black;
      ::com::xiaonei::service::buddy::BuddyCoreLogicAdapter::instance().addBlock(block, abc);

      break;
    case 5:
      type = UnBlack;
      ::com::xiaonei::service::buddy::BuddyCoreLogicAdapter::instance().removeBlock(block, abc);
      break;
    }
    //    RelationNotifyAdapter::instance().notify(type, from, target);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

