#include "tripod2/cc/client/tripod_cache_client.h"
#include <boost/lexical_cast.hpp>

namespace xce {
namespace tripod {
namespace client {

using boost::lexical_cast;
using boost::bad_lexical_cast;
using std::map;
using std::string;

TripodCacheClient::TripodCacheClient(const std::string& ns, const std::string& biz) {
  client_.reset(new TripodClient(ns,biz));
}

TripodCacheClient::~TripodCacheClient() {
  if (client_) {
    client_.reset();
  }
}

bool TripodCacheClient::Get(const std::string& key, std::string* value) {
  return client_->GetAndIgnoreMiss(key,value);
}

std::map<std::string, bool> TripodCacheClient::MultiGet(const StringListType& keys, StringMapType* values) {
  map<string, bool> result;
  for (StringListType::const_iterator c_it = keys.begin();
      c_it != keys.end(); ++c_it) {
    result.insert(pair<string, bool>(*c_it, false));
  }
  map<string, bool> tmp = client_->MultiGetAndIgnoreMiss(keys,values);
  // the bool value in tmp is all true
  for (map<string, bool>::const_iterator c_it = tmp.begin();
      c_it != tmp.end(); ++c_it) {
    result.erase(c_it->first);
    result.insert(pair<string, bool>(c_it->first, c_it->second));
  }
  return result;
}

bool TripodCacheClient::Set(const std::string& key, const std::string& value, int secs) {
  return client_->Set(key, value, secs);
}

bool TripodCacheClient::Remove(const std::string& key) {
  return client_->Remove(key);
}
// 计数器接口
bool TripodCacheClient::IncLong(const std::string& key, long step, long* value) {
  try {
    std::string strStep = lexical_cast<std::string>(step);
    std::string strResult = client_->Inc(key, strStep);
    if (strResult.empty()) {
      return false;
    }
    *value = lexical_cast<long>(strResult);
    return true;
  } catch (bad_lexical_cast &) {
    return false;
  }
}

bool TripodCacheClient::GetLong(const std::string& key, long* value) {
  std::string strValue;
  if (!client_->GetAndIgnoreMiss(key, &strValue)) {
    *value = ERROR_COUNT;
    return false;
  }
  try {
    long res = lexical_cast<long>(strValue);
    *value = res;
    return true;
  } catch (bad_lexical_cast &) {
    *value = ERROR_COUNT;
    return false;
  }
}

bool TripodCacheClient::SetLong(const std::string& key, long value, int secs) {
  if (secs <= 0) {
    return false;
  }
  std::string strValue;
  try {
    strValue = lexical_cast<std::string>(value);
  } catch (bad_lexical_cast &) {
    return false;
  }
  return client_->Set(key, strValue, secs);
}
// zSet接口
bool TripodCacheClient::ZAdd(const std::string& key, const StringMapType& set_with_scores) {
  return client_->ZAdd(key, set_with_scores);
}

bool TripodCacheClient::ZRemRangeByRank(const std::string& key, int begin, int limit) {
  if (begin < 0)
    return false;
  if (limit <= 0)
    return true;
  return client_->ZRemRangeByRank(key, begin, begin + limit -1);
}

bool TripodCacheClient::ZRemRangeByScore(const std::string& key, const std::string& min, const std::string& max) {
  return client_->ZRemRangeByScore(key, min, max);
} 

bool TripodCacheClient::ZRange(const std::string& key, int begin, int limit, StringListType* zset_value) {
  return client_->ZRange(key, zset_value, begin, limit);
}

bool TripodCacheClient::ZSet(const std::string& key, const StringMapType& set_with_scores, int secs) {
  if (secs <= 0) {
    return false;
  }
  return client_->ZSet(key, set_with_scores, secs);
}

bool TripodCacheClient::GetList(const std::string& key, int begin, int limit, StringListType* value) {
  return client_->GetListAndIgnoreMiss(key, value, begin, limit);
}

bool TripodCacheClient::SetList(const std::string& key, const StringListType& value, int secs) {
  if (secs <= 0) {
    return false;
  }
  return client_->SetList(key, value, secs);
}

}}}
