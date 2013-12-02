//功能：给SocialGraph组推荐产品线提供取好友ID列表功能
//开发人：熊君武
//邮件：junwu.xiong@renren-inc.com
//时间：2012-01-11

#include <iostream>
#include <vector>
#include <map>
#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>
//#include "socialgraph/socialgraphbuddybyid/src/socialgraphbuddybyidI.h"
#include "OceCxxAdapter/src/SocialGraphBuddyByIdAdapter.h"
#include "socialgraph/socialgraphutil/redis_adapter.h"

using namespace MyUtil;
using namespace xce::socialgraph;
using namespace std;


int main() 
{  

  //redis configuration and initialization
 // std::string ip = "10.3.16.85";
 // int port = 6379;
 // int timeout = 5000;
 // int core_size = 10;
 // int max_size = 100;
 // RedisAdapter::instance().Init(ip, port, timeout, core_size, max_size);

  //int frd_count = RedisAdapter::instance().GetListSize(boost::lexical_cast<std::string>("256404211"));
  //if(frd_count != -1) {
  //  std::cout << "redis frd_cout " << frd_count << std::endl; 
  //} else {
  //  std::cout << "could not find from redis" << std::endl;
  //}
  //RedisAdapter::instance().Remove(boost::lexical_cast<std::string>("256404211"));

  ////BuddyZKController::GetInstance();
  typedef std::map<int, int> Int2IntMap;
  typedef std::map<int, std::vector<int> > Int2IntSeqMap;
  typedef std::vector<int> IntVec;
  std::cout << "Input hostId: " << std::endl;
  int hostId=256404211;
  //std::cout << "Testing getFriendCount" << std::endl;
  ////std::cin >> hostId;
  //int frdCount =  SocialGraphBuddyByIdAdapter::instance().getFriendCount(hostId);
  //std::cout << "hostId " << hostId << " frdCount " << frdCount << std::endl;

  
  IntVec hostIds;
  hostIds.push_back(256404211);  
  hostIds.push_back(368025283);  
  hostIds.push_back(238111132);  
  hostIds.push_back(231815969);  
  hostIds.push_back(231815965);  
  hostIds.push_back(202911262);  
  for(int i = 1; i < 500; ++i) {
    hostIds.push_back(i*10+2);
  }
  std::cout << "Testing getFriendCounts" << std::endl;
  Int2IntMap my_map1 = SocialGraphBuddyByIdAdapter::instance().getFriendCounts(hostIds);
  BOOST_FOREACH(Int2IntMap::value_type val, my_map1) {
    std::cout << "hostId " << val.first << " frdCount " << val.second << std::endl; 
  }   

//  std::cout << "Testing getFriendList" << std::endl;
//  IntVec frdList;
//  ( SocialGraphBuddyByIdAdapter::instance().getFriendList(hostId,-1) ).swap(frdList);
//  //frdList = SocialGraphBuddyByIdAdapter::instance().getFriendList(hostId,0);
//  //frdList = SocialGraphBuddyByIdAdapter::instance().getFriendList(hostId,10);
//  //frdList = SocialGraphBuddyByIdAdapter::instance().getFriendList(hostId,10000);
//  BOOST_FOREACH(int i, frdList) {
//    std::cout << "frdList " << i << std::endl;
//  }
//  
//  std::cout << "frdList size " << frdList.size() << std::endl;
  
//  std::cout << std::endl;
//
//  std::cout << "Testing getFriendListN" << std::endl;
//  IntVec frdList1;
//  int begin=-1;
//  int limit=-1;
//  std::cout << "Input begin: " << std::endl;
//  std::cin >> begin;
//  std::cout << "Input limit: " << std::endl;
//  std::cin >> limit;
//  ( SocialGraphBuddyByIdAdapter::instance().getFriendListN(hostId,begin,limit) ).swap(frdList1);
//  BOOST_FOREACH(int i, frdList1) {
//    std::cout << "frdList1 " << i << std::endl;
//  }
  
//  std::cout << "Testing getFriendLists" << std::endl;
//  Int2IntSeqMap my_map2;
//  int index=0;
//  my_map2 = SocialGraphBuddyByIdAdapter::instance().getFriendLists(hostIds);
//  BOOST_FOREACH(Int2IntSeqMap::value_type val, my_map2) {
//    std::cout << " hostId " << val.first << " frdLists " << std::endl;
//    IntVec tmp;
//    (val.second).swap(tmp);
//    BOOST_FOREACH(int i, tmp) {
//      std::cout << " " << i;
//    }
//    std::cout << std::endl;
//    std::cout << "The " << ++index << "-th hostId " << val.first << " frdLists " << tmp.size() << std::endl;
//  }
//  std::cout << std::endl;
//

  //std::string key = "238";
  //      StrList idlist = boost::assign::list_of("22")("33")("55");
  //      RedisAdapter::instance().SetList(key, idlist);
//  std::string key = "256404211";
//  RedisAdapter::instance().Remove(boost::lexical_cast<std::string>("256404211"));
//  std::vector<std::string> list = boost::assign::list_of("66")("77")("88");
//  RedisAdapter::instance().SetList(key, list);
//  std::cout << "Testing AddFriend" << std::endl;
//  int frd_count = RedisAdapter::instance().GetListSize(boost::lexical_cast<std::string>("256404211"));
//  if(frd_count != -1) {
//    std::cout << "redis frd_cout " << frd_count << std::endl; 
//  } else {
//    std::cout << "could not find from redis" << std::endl;
//  }
//  //RedisAdapter::instance().Remove(boost::lexical_cast<std::string>("256404211"));
//  int guestId = 66;
//  std::cout << "Testing AddFriend:" << hostId << std::endl;
//  SocialGraphBuddyByIdAdapter::instance().AddFriend(hostId, guestId);
//  //RedisAdapter::instance().Remove(boost::lexical_cast<std::string>(hostId));
//  frd_count = RedisAdapter::instance().GetListSize(boost::lexical_cast<std::string>(hostId));
//  std::cout << "redis add frd_cout " << frd_count << std::endl;
//
//  std::cout << "Testing RemoveFriend" << std::endl;
//  SocialGraphBuddyByIdAdapter::instance().RemoveFriend(hostId, guestId);
//  frd_count = RedisAdapter::instance().GetListSize(boost::lexical_cast<std::string>("256404211"));
//  std::cout << "redis remove frd_cout " << frd_count << std::endl;
  return 0;
}
