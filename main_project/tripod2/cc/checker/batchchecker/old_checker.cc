/*
 * 旧服务的checker
 * xiaofeng.liang@renren-inc.com
 */

#include <vector>
#include <fstream>
#include <iostream>

#include "boost/shared_ptr.hpp"
#include "boost/algorithm/string.hpp"

#include "tripod2/cc/producermanager/producer_factory.h"
#include "tripod2/cc/core/producer.h"
#include "tripod2/cc/core/tripod_client.h"
#include "DbCxxPool/src/ConnectionPoolManager.h"
#include "util/cpp/TimeCost.h"
#include "DbCxxPool/src/QueryRunner.h"
#include "OceCxxAdapter/src/UserPassportTripodAdapter.h"
#include "OceCxxAdapter/src/UserPassportAdapter.h"

#include "pc_impl.h"
#include "old_checker.h"

using namespace MyUtil;
using namespace std;
using namespace xce::tripod;
using namespace xce::util;
using namespace com::xiaonei::xce;
using namespace xce::adapter::userpassport;

namespace xce {
namespace tripod {
namespace checker {

//OldCheckExecuter 为旧的服务而写的Check
////--------------------------------------------------------------
//OldChecker::OldChecker(Queue<int> *queue, int batch, string biz) :
//    ConsumerThread<int>(queue, batch), _biz(biz) {
//  MCE_INFO("[OldChecker]Init, batch=" << batch << ", biz=" << _biz)
//}

//bool OldCheckExecuter::consum(vector<int> ids) {
//  MyUtil::Clock clock;
//  vector<std::string> keys;
//  for(vector<int>::iterator it = ids.begin(); it != ids.end(); ++it) {
//    keys.push_back(boost::lexical_cast<string>(*it));
//  }
//  int uid = ids.front();
//
//  String2ByteSeqMap map_db;
//  StringMapType map_cache;
//  vector<string> load_keys;
//  //Db
//  std::list<string> keys_list(keys.begin(), keys.end());
//  _producer->Create(keys_list, &map_db, true);
//  MCE_DEBUG("[OldCheckExecuter]Get_From_DB:" << uid << ", time_cost=" << clock.step()/1000 << " ms");
//  //Compare
//  for(vector<string>::iterator it = keys.begin(); it != keys.end(); ++it) {
//    String2ByteSeqMap::iterator it_db = map_db.find(*it);
//    if(it_db != map_db.end()) {
//      PbUserPassport pb_value;
//      std::string str_value(it_db->second.begin(), it_db->second.end());
//      std::istringstream in(str_value); 
//      pb_value.ParseFromIstream(&in);
//      UserPassportInfoPtr db_info = new UserPassportInfo();
//      parse(pb_value, db_info);
//      UserPassportInfoPtr cache_info = new UserPassportInfo();
//      cache_info = getFromCache(*it);
//      if(!compare(db_info, cache_info)) {
//        load_keys.push_back(*it);
//      }
//    } else {
//      MCE_WARN("[OldCheckExecuter]Data_Miss, db_miss=" << (it_db == map_db.end()));
//      //默认全缓存load
//      load_keys.push_back(*it);
//    }
//  }  
//  MCE_DEBUG("[OldCheckExecuter]Compare and load:" << uid << ", time_cost=" << clock.step()/1000 << " ms");
//  //Load(如果有时间，可以把load拆出来，用另外一个线程去处理)
//  for(vector<string>::iterator it = load_keys.begin(); it != load_keys.end(); ++it) {
//    load(*it);
//  }
//  MCE_DEBUG("[OldCheckExecuter]Load:" << uid << ", count=" << load_keys.size()
//      << ", time_cost=" << clock.step()/1000 << " ms");
//  MCE_INFO("[OldCheckExecuter]Check_Done:" << uid << ", ns=" << _ns << ", biz=" << _biz
//      << ", count=" << keys.size() << ", time_cost=" << clock.total()/1000 << " ms");
//  return true;
//}

//load三次，防止超时，打不同的log，对于三次都load失败的需要查原因处理
//void OldCheckExecuter::load(string key) {
//  bool success = false;
//  for(int i = 0; i < 3; i++) {
//    MCE_INFO("[OldCheckExecuter]Load key=" << key);
//    try {
//      UserPassportAdapter::instance().reload(boost::lexical_cast<int>(key));
//      success = true;
//    } catch (...) {
//    }
//    if(success) {
//      break;
//    } else {
//      MCE_WARN("[OldCheckExecuter]Load_Fail, key=" << key << ", load_count=" << i);
//    }
//  }
//  if(!success) {
//    MCE_WARN("[OldCheckExecuter]Load_Final_Fail, key=" << key);
//  }
//}
//
      //PbUserPassport pb_value;
      //std::string str_value(it_db->second.begin(), it_db->second.end());
      //std::istringstream in(str_value); 
      //pb_value.ParseFromIstream(&in);
      //UserPassportInfoPtr db_info = new UserPassportInfo();
      //UserPassportUtil::parse(pb_value, db_info);
      //UserPassportInfoPtr cache_info = new UserPassportInfo();
      //try {
      //  cache_info = UserPassportAdapter::instance().getUserPassport(boost::lexical_cast<int>(*it));
      //} catch (...) {
      //}
      //if(db_info->id() != cache_info->id())
      //{
      //  std::cout<<"debug wrong id; source " << db_info->id() << " des " << cache_info->id() << std::endl;
      //  load_keys.push_back(*it);
      //}
      //if(db_info->status() != cache_info->status())
      //{
      //  std::cout<<"debug wrong id " << db_info->id() << " status; source " << db_info->status()<< " des " << cache_info->status()<< std::endl;
      //  load_keys.push_back(*it);
      //}
      //if(db_info->safestatus() != cache_info->safestatus())
      //{
      //  std::cout<<"debug wrong id " << db_info->id() << " safestatus; source " << db_info->safestatus() << " des " << cache_info->safestatus() << std::endl;
      //  load_keys.push_back(*it);
      //}
      //if(db_info->account() != cache_info->account())
      //{
      //  std::cout<<"debug wrong id " << db_info->id()<< " account; source " << db_info->account() << " des " << cache_info->account() << std::endl;
      //  load_keys.push_back(*it);
      //}
      //if(db_info->passwordmd5() != cache_info->passwordmd5())
      //{
      //  std::cout<<"debug wrong id " << db_info->id() << " passwordmd5; source " << db_info->passwordmd5() << " des " << cache_info->passwordmd5() << std::endl;
      //  load_keys.push_back(*it);
      //}
      //if(db_info->domain() != cache_info->domain())
      //{
      //  std::cout<<"debug wrong id " << db_info->id() << " domain; source" << db_info->domain() << " des " << cache_info->domain() << std::endl;
      //  load_keys.push_back(*it);
      //}
}}}// namespace xce::tripod::checker
