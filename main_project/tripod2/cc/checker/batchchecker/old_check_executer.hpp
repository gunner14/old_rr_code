/** 
 * 旧服务的check模板
 * xiaofeng.liang@renren-inc.com
 */
#ifndef XCE_TRIPOD_CHECKER_OLD_CHECK_EXECUTER_HPP_
#define XCE_TRIPOD_CHECKER_OLD_CHECK_EXECUTER_HPP_

#include <vector>

#include "boost/lexical_cast.hpp"

#include "tripod2/cc/core/producer.h"
#include "tripod2/cc/core/tripod_client.h"

#include "pc_model.h"

using namespace xce::util;
using namespace xce::tripod;
using namespace com::xiaonei::xce;

namespace xce {
namespace tripod {
namespace checker {

class OldCheckExecuterIF {
public:
  virtual bool consum(vector<int> ids) = 0;
};

template <class DATAI, class DATAIPTR, class DATAP>
class OldCheckExecuter : virtual public OldCheckExecuterIF {
public:
  OldCheckExecuter(string biz) : _biz(biz) {
    MCE_INFO("[OldCheckExecuter]Init, biz=" << _biz);
    _producer = ProducerFactory::GetInstance().Get(_biz);
  }

  bool consum(vector<int> ids) {
    MyUtil::Clock clock;
    vector<std::string> keys;
    for(vector<int>::iterator it = ids.begin(); it != ids.end(); ++it) {
      keys.push_back(boost::lexical_cast<string>(*it));
    }
    int uid = ids.front();

    String2ByteSeqMap map_db;
    StringMapType map_cache;
    vector<string> load_keys;
    //Db
    std::list<string> keys_list(keys.begin(), keys.end());
    _producer->Create(keys_list, &map_db, true);
    MCE_DEBUG("[OldCheckExecuter]Get_From_DB:" << uid
        << ", time_cost=" << clock.step()/1000 << " ms");
    //Compare
    for(vector<string>::iterator it = keys.begin(); it != keys.end(); ++it) {
      String2ByteSeqMap::iterator it_db = map_db.find(*it);
      if(it_db != map_db.end()) {
        DATAP pb_value;
        std::string str_value(it_db->second.begin(), it_db->second.end());
        std::istringstream in(str_value); 
        pb_value.ParseFromIstream(&in);
        DATAIPTR db_info = new DATAI;
        parse(pb_value, db_info);
        DATAIPTR cache_info = new DATAI;
        cache_info = get(boost::lexical_cast<int>(*it));
        if(!compare(db_info, cache_info)) {
          load_keys.push_back(*it);
        }
      } else {
        MCE_WARN("[OldCheckExecuter]Data_Miss, db_miss=" << (it_db == map_db.end()));
        //默认全缓存load
        load_keys.push_back(*it);
      }
    }  
    MCE_DEBUG("[OldCheckExecuter]Compare and load:" << uid
        << ", time_cost=" << clock.step()/1000 << " ms");
    //Load(如果有时间，可以把load拆出来，用另外一个线程去处理)
    for(vector<string>::iterator it = load_keys.begin(); it != load_keys.end(); ++it) {
      tribleLoad(boost::lexical_cast<int>(*it));
    }
    MCE_DEBUG("[OldCheckExecuter]Load:" << uid << ", count=" << load_keys.size()
        << ", time_cost=" << clock.step()/1000 << " ms");
    MCE_INFO("[OldCheckExecuter]Check_Done:" << _biz << ", id=" << uid
        << ", count=" << keys.size() << ", time_cost=" << clock.total()/1000 << " ms");
    return true;
  }

  void tribleLoad(int id) {
    bool success = true;
    int i = 0;
    while(i < 3) {
      try {
        MCE_INFO("[OldCheckExecuter]TRY_LOAD:" << _biz << ",id=" << id << ", try_count=" << i);
        load(id);
        break;
      } catch (std::exception e) {
        success = false;
        MCE_WARN("[OldCheckExecuter]LOAD_FAIL:" << _biz << ",id=" << id
            << ", try_count=" << i << ", exception:" << e.what());
      } catch (...) {
        success = false;
        MCE_WARN("[OldCheckExecuter]LOAD_FAIL:" << _biz << ",id=" << id
            << ", try_count=" << i << ", unknown exception");
      }
      i++;
    }
    if(!success) {
      MCE_WARN("[OldCheckExecuter]LOAD_FINAL_FAIL:" << _biz << ",id=" << id);
    }
  }

  template <typename T> bool checkItem(const int id, const string &field, T db, T cache) {
    if(db != cache) {
      MCE_WARN("Wrong " << _biz << "::" << field << ": id=" << id
          << ", db=" << db << ", cache=" << cache);
      return false;
    } else {
      return true;
    }
  }

  virtual bool compare(DATAIPTR db, DATAIPTR cache) = 0;
  virtual DATAIPTR get(int uid) = 0;
  virtual void load(int uid) = 0;
  virtual void parse(DATAP pb_data, DATAIPTR &data) = 0;

protected:
  Producer *_producer;  
  string _biz;
};


}}}// namespace xce::tripod

#endif
