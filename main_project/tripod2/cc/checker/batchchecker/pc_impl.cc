/** 
 * 具体的生产者消费者
 * xiaofeng.liang@renren-inc.com
 */

#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "boost/shared_ptr.hpp"
#include "boost/algorithm/string.hpp"

#include "tripod2/cc/producermanager/producer_factory.h"
#include "tripod2/cc/core/producer.h"
#include "tripod2/cc/core/tripod_client.h"
#include "DbCxxPool/src/ConnectionPoolManager.h"
#include "util/cpp/TimeCost.h"
#include "DbCxxPool/src/QueryRunner.h"

#include "pc_impl.h"

using namespace MyUtil;
using namespace std;
using namespace xce::tripod;
using namespace xce::util;
using namespace com::xiaonei::xce;

namespace xce {
namespace tripod {
namespace checker {

//FileProducer 从文件产生id，文件格式需要是一行一个id
//--------------------------------------------------------------
FileProducer::FileProducer(Queue<int> *queue, int band, int batch, string file) :
    ProducerThread<int>(queue, band, batch), _finish(false) {
  MCE_INFO("[FileProducer]Init, file=" << file
        << ", band=" << band << ", batch=" << batch);
  fin.open(file.c_str(), ios_base::in);
  if(!fin) {
    MCE_WARN("[FileProducer]File_Error, file=" << file
        << ", band=" << band << ", batch=" << batch);
    _finish = true;
  }
}

FileProducer::~FileProducer() {
  if(fin) {
    fin.close();
  }
}

void FileProducer::produce(vector<int> *ids, int batch) {
  MyUtil::Clock clock;
  if(!_finish) {
    int id;
    while(fin >> id) {
      ids->push_back(id);
      if(--batch <= 0) {
        break;
      }
    }
    if(batch > 0) {
      _finish = true;
    }
  }
  MCE_INFO("[FileProducer]produce, count=" << ids->size()
      << ", time_cost=" << clock.total()/1000 << " ms");
}

//StdinProducer 从标准输入获取id，输入格式需要是一行一个id
//--------------------------------------------------------------
StdinProducer::StdinProducer(Queue<int> *queue, int band, int batch) :
    ProducerThread<int>(queue, band, batch), _finish(false) {
  MCE_INFO("[StdinProducer]Init, band=" << band << ", batch=" << batch);
}

void StdinProducer::produce(vector<int> *ids, int batch) {
  MyUtil::Clock clock;
  if(!_finish) {
    int id;
    while(cin >> id) {
      ids->push_back(id);
      if(--batch <= 0) {
        break;
      }
    }
    if(batch > 0) {
      _finish = true;
    }
  }
  MCE_INFO("[StdinProducer]produce, count=" << ids->size()
      << ", time_cost=" << clock.total()/1000 << " ms");
}

//CreateKeysResultHandler DB处理类
//--------------------------------------------------------------
CreateKeysResultHandler::CreateKeysResultHandler(const string &key, vector<int> *keyseq) :
  _key(key), _keyseq(keyseq) {}

bool CreateKeysResultHandler::handle(mysqlpp::Row& row) const {
  _keyseq->push_back((int)row[_key.c_str()]);
  return true;
}

//DBProducer 从数据库获取ID，可以指定范围
//--------------------------------------------------------------
DbProducer::DbProducer(Queue<int> *queue, int band, int batch, int min, int max) :
    ProducerThread<int>(queue, band, batch), _min(min), _max(max) {
  MCE_INFO("[DbProducer]Init, band=" << band << ", batch=" << batch
      << ", min=" << min << ", max=" << max);
}

void DbProducer::produce(vector<int> *ids, int batch) {
  MyUtil::Clock clock;
  Statement sql;
  sql << "SELECT id FROM user_passport WHERE id > " << mysqlpp::quote << _min;
  if(_max > 0 && _max < 0x7fffffff) { //有上限
    sql << " AND id <= " << mysqlpp::quote << _max;
  }
  sql << " LIMIT " << batch;
  CreateKeysResultHandler handler("id", ids);
  QueryRunner("user_passport", CDbRServer).query(sql, handler);
  if(ids->size()) {
    _min = ids->back();
  }
  ostringstream oss;
  oss << "[DbProducer]produce, count=" << ids->size();
  if(ids->size()) {
    oss << " front= " << ids->front() << " back= " << ids->back();
  }
  oss << " time_cost=" << clock.total()/1000 << " ms";
  MCE_INFO(oss.str());
}

//BatchChecker 批量从DB和Cache获取数据，对比，对不一致的Load
//--------------------------------------------------------------
BatchChecker::BatchChecker(Queue<int> *queue, int batch, string ns, string biz) :
    ConsumerThread<int>(queue, batch), _ns(ns), _biz(biz) {
  MCE_INFO("[BatchChecker]Init, batch=" << batch << ", ns=" << _ns << ", biz=" << _biz)
  _producer = ProducerFactory::GetInstance().Get(_biz);
  _client.reset(new TripodClient(_ns, _biz));
}

bool BatchChecker::consum(vector<int> ids) {
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
  MCE_DEBUG("[BatchChecker]Get_From_DB:" << uid << ", time_cost=" << clock.step()/1000 << " ms");
  //Cache
  _client->MultiGetAndIgnoreMiss(keys_list, &map_cache);
  MCE_DEBUG("[BatchChecker]Get_From_Cache:" << uid << ", time_cost=" << clock.step()/1000 << " ms");
  //Compare
  for(vector<string>::iterator it = keys.begin(); it != keys.end(); ++it) {
    MCE_DEBUG("[BatchChecker]Check:" << *it);
    String2ByteSeqMap::iterator it_db = map_db.find(*it);
    StringMapType::iterator it_cache = map_cache.find(*it);
    if(it_db != map_db.end() && it_cache != map_cache.end()) {
      string db_value(it_db->second.begin(), it_db->second.end());
      if(it_cache->second != db_value) {
        if(_biz == "user_logincount") { //在一定范围内误差不去load
          bool load = true;
          try {
            int count_db = boost::lexical_cast<int>(db_value);
            int count_cache = boost::lexical_cast<int>(it_cache->second);
            if(abs(count_db - count_cache) < 10) {
              load = false;
            }
          } catch (std::exception e) {
            MCE_WARN("[BatchChecker]Convert_ERROR " << e.what());
          }
          if(load) {
            MCE_WARN("[BatchChecker]Data_Wrong, id=" << *it << " db=" << db_value
                << " cahce=" << it_cache->second);
            load_keys.push_back(*it);
          }
        } else {
          MCE_WARN("[BatchChecker]Data_Wrong, id=" << *it << " db=" << db_value
              << " cahce=" << it_cache->second);
          load_keys.push_back(*it);
        }
      }
    } else {
      MCE_WARN("[BatchChecker]Data_Miss, id=" << *it << "db_miss=" << (it_db == map_db.end())
          << ", cache_miss=" << (it_cache == map_cache.end()))
      load_keys.push_back(*it);
    }
  }  
  MCE_DEBUG("[BatchChecker]Compare and load:" << uid << ", time_cost=" << clock.step()/1000 << " ms");
  //Load(如果有时间，可以把load拆出来，用另外一个线程去处理)
  for(vector<string>::iterator it = load_keys.begin(); it != load_keys.end(); ++it) {
    load(*it);
  }
  MCE_DEBUG("[BatchChecker]Load:" << uid << ", count=" << load_keys.size()
      << ", time_cost=" << clock.step()/1000 << " ms");
  MCE_INFO("[BatchChecker]Check_Done:" << uid << ", ns=" << _ns << ", biz=" << _biz
      << ", count=" << keys.size() << ", time_cost=" << clock.total()/1000 << " ms");
  return true;
}

//load三次，防止超时，打不同的log，对于三次都load失败的需要查原因处理
void BatchChecker::load(string key) {
  bool success = false;
  for(int i = 0; i < 3; i++) {
    MCE_DEBUG("[BatchChecker]Load key=" << key);
    success = _client->Load(key, 30*24*3600, true); // 过期时间默认一个月
    if(success) {
      break;
    } else {
      MCE_WARN("[BatchChecker]Load_Fail, key=" << key << ", load_count=" << i);
    }
  }
  if(!success) {
    MCE_WARN("[BatchChecker]Load_Final_Fail, key=" << key);
  }
}

//SimpleLoader 不Check，只做Load
//--------------------------------------------------------------
SimpleLoader::SimpleLoader(Queue<int> *queue, int batch, string ns, string biz) :
    ConsumerThread<int>(queue, batch), _ns(ns), _biz(biz) {
  MCE_INFO("[SimpleLoader]Init, batch=" << batch << ", ns=" << _ns << ", biz=" << _biz)
  _client.reset(new TripodClient(_ns, _biz));
}

//load三次，防止超时，打不同的log，对于三次都load失败的需要查原因处理
bool SimpleLoader::consum(vector<int> ids) {
  MyUtil::Clock clock;
  int uid = ids.front();
  for(vector<int>::iterator it = ids.begin(); it != ids.end(); ++it) {
    string key = boost::lexical_cast<string>(*it);
    bool success = false;
    for(int i = 0; i < 3; i++) {
      MCE_INFO("[SimpleLoader]Load key=" << key);
      success = _client->Load(key, 30*24*3600, false); // 过期时间默认一个月
      if(success) {
        break;
      } else {
        MCE_WARN("[SimpleLoader]Load_Fail, key=" << key << " load_count=" << i);
      }
    }
    if(!success) {
      MCE_WARN("[SimpleLoader]Load_Final_Fail, key=" << key);
    }
  }
  MCE_INFO("[SimpleLoader]:" << uid << ", count=" << ids.size()
      << ", time_cost=" << clock.step()/1000 << " ms");
}


}}}// namespace xce::tripod::checker
