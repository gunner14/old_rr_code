#ifndef SINGLE_TABLE_TRIPOD_CLIENT_H_
#define SINGLE_TABLE_TRIPOD_CLIENT_H_

#include <string>
#include <sstream>
#include "boost/shared_ptr.hpp"
#include "tripod2/cc/core/tripod_client.h"
#include "boost/foreach.hpp"
#include "LogWrapper.h"


namespace xce {
namespace tripod {

/**
 * DATA 由protobuf生成的数据类型
 * KEY 数据库表中主键的类型
 */
template<class DATA,class KEY_TYPE= long>
class SingleTableTripodClient {

 private:
  boost::shared_ptr<TripodClient> tripod_client_;

 public:
  SingleTableTripodClient(const std::string& namespace_id, const std::string& biz_id) {
    tripod_client_.reset(new TripodClient(namespace_id,biz_id));
  }
 
  bool Load(const KEY_TYPE& key, int secs,  bool use_master= false) {
    return tripod_client_->Load(boost::lexical_cast<std::string>(key),secs,use_master);
  }
 
  bool Remove(const KEY_TYPE& key) {
    return tripod_client_->Remove(boost::lexical_cast<std::string>(key));
  }

  bool Get(const KEY_TYPE& key, DATA* value, int secs = 0) {
    std::string str_value;
    bool result = tripod_client_->Get(boost::lexical_cast<std::string>(key),&str_value,secs);
    if (!result) {
      return false;
    }
    std::istringstream in(str_value); 
    value->ParseFromIstream(&in);
    return true;
  }

  std::map<KEY_TYPE,bool> MultiGet(const std::list<KEY_TYPE>& keys, std::map<KEY_TYPE,DATA>* values, int secs = 0) {
    StringListType str_keys;
    typename std::list<KEY_TYPE>::const_iterator key_iter = keys.begin();
    for(; key_iter != keys.end() ; ++key_iter) {
      str_keys.push_back(boost::lexical_cast<std::string>(*key_iter));
    }
    StringMapType str_values;
    std::map<std::string,bool> str_result;
    str_result = tripod_client_->MultiGet(str_keys,&str_values,secs);
    std::map<KEY_TYPE,bool> result;
    std::map<std::string,bool>::iterator iter = str_result.begin();
    for(;iter!= str_result.end();++iter) {
      result.insert(std::make_pair(boost::lexical_cast<KEY_TYPE>(iter->first),iter->second )); 
      if(iter->second) {
        std::istringstream in(str_values[iter->first]); 
        DATA data;
        data.ParseFromIstream(&in);
        values->insert(std::make_pair(boost::lexical_cast<KEY_TYPE>(iter->first),data) ); 
      } 
    }
    return result;
  }

};

}}//namespace

#endif




