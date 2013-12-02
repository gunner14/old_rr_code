#include "gtest/gtest.h"
#include <iostream>
#include "OceCxxAdapter/src/VipIconDataTripodAdapter.h"

namespace xce {
namespace adapter {
namespace vipicondata {

TEST(VipIconDataTripodAdapter, GetVipIconDataSeqMap) {
  try {
    std::vector<int> ids, types;
    ids.push_back(35178);
    ids.push_back(35194);
    types.push_back(VIP);
    types.push_back(DISNEY);
    types.push_back(GIFT);
    std::map<int, xce::vipinfo::VipIconDataSeq> infos = VipIconDataTripodAdapter::instance().GetVipIconDataSeqMap(ids, types);
    std::cout << "result size:" << infos.size() << std::endl;
    for(std::map<int, xce::vipinfo::VipIconDataSeq>::const_iterator its = infos.begin(); its != infos.end(); ++its) {
      for(xce::vipinfo::VipIconDataSeq::const_iterator it = its->second.begin(); it != its->second.end(); ++it) {
        std::cout << "id:" << (*it)->id << std::endl;
        std::cout << "type:" << (*it)->type << std::endl;
        std::cout << "iconId:" << (*it)->iconId << std::endl;
        std::cout << "hideUrl:" << (*it)->hideUrl << std::endl;
        std::cout << "level:" << (*it)->level << std::endl;
        std::cout << "status:" << (*it)->status << std::endl;
      }
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

TEST(VipIconDataTripodAdapter, GetVipFeedDataSeqMap) {
  try {
    std::vector<int> ids, types;
    ids.push_back(35178);
    ids.push_back(35194);
    types.push_back(VIP);
    types.push_back(DISNEY);
    types.push_back(GIFT);
    std::map<int, xce::vipinfo::VipFeedDataSeq> infos = VipIconDataTripodAdapter::instance().GetVipFeedDataSeqMap(ids, types);
    std::cout << "result size:" << infos.size() << std::endl;
    for(std::map<int, xce::vipinfo::VipFeedDataSeq>::const_iterator its = infos.begin(); its != infos.end(); ++its) {
      for(xce::vipinfo::VipFeedDataSeq::const_iterator it = its->second.begin(); it != its->second.end(); ++it) {
        std::cout << "id:" << (*it)->member->id << std::endl;
        std::cout << "type:" << (*it)->member->type << std::endl;
        std::cout << "level:" << (*it)->member->level << std::endl;
        std::cout << "status:" << (*it)->member->status << std::endl;
        std::cout << "iconId:" << (*it)->icon->iconId << std::endl;
        std::cout << "iconUrl:" << (*it)->icon->hideUrl << std::endl;
        std::cout << "hatId:" << (*it)->hat->hatId << std::endl;
        std::cout << "hatUrl:" << (*it)->hat->hideUrl << std::endl;
        std::cout << "hatStatus:" << (*it)->hat->status << std::endl;
      }
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}}} //xce::user::adapter::userpassport
