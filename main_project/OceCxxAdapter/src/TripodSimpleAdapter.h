#ifndef __XCE_ADAPTER_TRIPODSIMPLEADAPTER_H__
#define __XCE_ADAPTER_TRIPODSIMPLEADAPTER_H__

#include <tripod-new/src/cpp/include/TripodClient.h>
#include <DBWriterAdapter.h>
#include <QueryRunner.h>
#include <cctype> //toupper
#include <algorithm> //transform

namespace xce {
namespace adapter {

using MyUtil::Str2StrMap; 
using com::renren::tripod::KeySeq;
using com::renren::tripod::DataMap;
using com::renren::tripod::TripodClient;
using com::xiaonei::xce::Statement;
using xce::dbmanager::DBWriterAdapter;

/**
 * DATA 由protobuf生成的数据类型
 * KEY 数据库表中主键的类型
 */
template <class DATA, class KEY = long>
class TripodSimpleAdapter {
public:
  /**
   * zkAddress zookeeper地址
   * namespaceId tripod cache namespace id
   * businessId business id
   * db 数据库源的名字
   * table 数据库表的名字
   * field 数据库表中主键的名字
   */
  TripodSimpleAdapter(const string& zkAddress, const string& namespaceId, const string& businessId,
      const string& db, const string& table, const string& field = "ID") :
        _db(db), _table(table), _field(field){
    transform(_field.begin(), _field.end(), _field.begin(), (int(*)(int))toupper);
    _tripodClient = new TripodClient(zkAddress, namespaceId, businessId); 
    //DBWriterAdapter::instance().init();
  }

  virtual ~TripodSimpleAdapter() {
    delete _tripodClient;
  }

  DATA get(KEY key) {
    DATA data;
    KeySeq keys, missedKeys;
    string keyStr = boost::lexical_cast<string>(key);
    keys.push_back(keyStr);
    DataMap dataMap = _tripodClient->get(keys, missedKeys);
    DataMap::const_iterator it = dataMap.find(keyStr);
    if(it != dataMap.end()) {
      string value(it->second.begin(), it->second.end());
      istringstream in(value);
      data.ParseFromIstream(&in);
    }
    return data;
  }

  void load(KEY key, int expiration = 0) {
    KeySeq keys;
    keys.push_back(boost::lexical_cast<string>(key));
    _tripodClient->load(keys, expiration, false);
  }

  void remove(KEY key, int timeout = 1000) {
    KeySeq keys;
    keys.push_back(boost::lexical_cast<string>(key));
    _tripodClient->deleteCache(keys, timeout);
  }

  void set(KEY key, const Str2StrMap& props) {
    //去掉map中的用户id
    Str2StrMap::iterator it = const_cast<Str2StrMap&>(props).find(_field);
    if(it != props.end()) {
      const_cast<Str2StrMap&>(props).erase(it);
    }

    Str2StrMap filter;
    filter[_field] = boost::lexical_cast<string>(key);
    Statement sql = Statement::buildUpdate(_table, filter, props);
    mysqlpp::Query query(0);
    sql.query(query);
    DBWriterAdapter::instance().write(_db, "", query.str());
    load(key);
  }

  void create(const Str2StrMap& props) {
    Str2StrMap::const_iterator it = props.find(_field);
    if (it != props.end()) {
      long key = boost::lexical_cast<long>(it->second);
      Str2StrMap filter;
      Statement sql = Statement::buildInsert(_table, props);
      mysqlpp::Query query(0);
      sql.query(query);
      DBWriterAdapter::instance().write(_db, "", query.str());
      load(key);
    } else {
      MCE_WARN("TripodSimpleAdapter::create, Exception[No " << _field << " item in props]");
    }
  }

private:
  const string _db; //数据库源
  const string _table; //表名
  string _field; //主键名
  TripodClient *_tripodClient;
};

}} //xce::adapter

#endif
