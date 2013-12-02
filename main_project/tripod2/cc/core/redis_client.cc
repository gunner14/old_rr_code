#include "tripod2/cc/core/redis_client.h"

#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"

#include "base/stringdigest.h"
#include "LogWrapper.h"
#include "util/cpp/TimeCost.h"

namespace xce {
namespace tripod {


const int RedisClient::kWriteBatchSize = 900;
const int RedisClient::kReadBatchSize = 900;

 
RedisClient::RedisClient(const::string& address, int port, int timeout): address_(address), port_(port), timeout_(timeout), conn_(0), valid_(false) {
  MCE_INFO("RedisClient::RedisClient() address:" << address << " port:" << port
      << " timeout:" << timeout);
  MyUtil::Clock clock;
  struct timeval tv;
  tv.tv_sec = timeout / 1000;
  tv.tv_usec = 0;
  conn_ = redisConnectWithTimeout(address_.c_str(), port_, tv);
  //conn_ = redisConnect(address_.c_str(), port_);
  if (conn_->err) {
    MCE_WARN("the connect to the redis server is failed! err:" << conn_->err 
        << " reason:" << conn_->errstr << "\t address:" << address_ << "\tport:" << port
        << "\t conn_->fd:" << conn_->fd << " time_cost:" << clock.total() << " us");
    set_valid(false);
  //  conn_ = 0;
  }
  else {
    MCE_INFO("RedisClient::RedisClient() done. address:" << address << " port:" << port 
        << " timeout:" << timeout << " time_cost:" << clock.total() << " us");
    set_valid(true);
  }
}

RedisClient::~RedisClient() {
  MCE_INFO("RedisClient::~RedisClient() valid:" << std::boolalpha << valid_);
  if (conn_) {
    redisFree(conn_);
  }
}

bool RedisClient::Get(const std::string& key, std::string* value) {
  if (!value) {
    return false;
  }
  std::ostringstream otem;
  otem << "GET " << key;
  std::string command = otem.str();
  MCE_TRACE("RedisClient::Get() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  if (!reply) {
    set_valid(false);
    return false;
  }
  MCE_TRACE("reply->type:" << reply->type << "\t" << "reply->str:" << reply->str << "\t" << "reply->integer:" << reply->integer);
  if (reply->type == REDIS_REPLY_ERROR) {
    set_valid(false);
    freeReplyObject(reply);
    return false;
  }
  if (reply->type == REDIS_REPLY_STRING) {
    // 注意：需要显示指定字符串长度，避免将'\0'作为字符串结束符
    value->assign(reply->str, reply->len);
    MCE_TRACE("reply->len:" << reply->len);

    freeReplyObject(reply);
    return true;
  }
  freeReplyObject(reply);
  return false;
}

bool RedisClient::Set(const std::string& key, const std::string& value, int secs) {
  if (value.empty()) {
    return false;
  }
  if (secs < 0) {
    return false;
  }
  // 1. generate temp key
  // 2. set 
  // 3. rename 
  const std::string temp_key = key;
  std::ostringstream otem;
  if (secs > 0) {
    otem << "SETEX " << temp_key << " " << secs << " " << value;
  } else {
    otem << "SET " << temp_key << " " << value;
  }
  std::string command = otem.str();

  redisReply* reply = 0;
  // 在SET时使用参数模式显示指定字符串长度，避免将值中的‘\0'作为字符结束符
  if (secs > 0) {
    std::string secsStr = boost::lexical_cast<string>(secs);
    const char *argv[4];
    argv[0] = "SETEX";
    argv[1] = temp_key.c_str();
    argv[2] = secsStr.c_str();
    argv[3] = value.c_str();
    size_t lens[4] = { 5, temp_key.size(), secsStr.size(), value.size()};
    int argc = 4;
    MCE_DEBUG("RedisClient::Set() command:" << argv[0] << " " << argv[1] << " " << argv[2] << " " << argv[3]);
    reply = (redisReply*) redisCommandArgv(conn_, argc, argv, lens );
  }
  else {
    const char *argv[3];
    argv[0] = "SET";
    argv[1] = temp_key.c_str();
    argv[2] = value.c_str();
    size_t lens[3] = { 3, temp_key.size(), value.size()};
    int argc = 3;
    MCE_DEBUG("RedisClient::Set() command:" << argv[0] << " " << argv[1] << " " << argv[2]);
    reply = (redisReply*) redisCommandArgv(conn_, argc, argv, lens );
  }

  if (!reply) {
    set_valid(false);
    return false;
  }
  MCE_DEBUG("RedisClient::Set() address:" << address_ << "\tport:" << port_
            << "reply->type:" << reply->type << "\t" << "reply->str:" << reply->str << "\t" << "reply->integer:" << reply->integer);
  if (reply->type == REDIS_REPLY_ERROR) {
//    set_valid(false);
    freeReplyObject(reply);
    return false;
  }
  if (reply->type == REDIS_REPLY_STATUS) {
    static char *ok = "OK";
    bool result = strcmp(ok, reply->str) == 0 ? true : false;
    freeReplyObject(reply);
    return result;
  }
  freeReplyObject(reply);
  return false;
}

std::string RedisClient::Inc(const std::string& key, const std::string& step) {
  MCE_DEBUG("std::string RedisClient::Inc(const std::string& key, const std::string& step) key : " << key);
  const static size_t ARGC_INC = 3;
  const static string COM_INC = "INCRBY";
  std::string result;
  std::ostringstream otem;
  otem << COM_INC << " " << key << " " << step;
  std::string command = otem.str();

  redisReply* reply = 0;
  const char *argv[ARGC_INC];
  argv[0] = COM_INC.c_str();
  argv[1] = key.c_str();
  argv[2] = step.c_str();
  size_t lens[ARGC_INC] = { COM_INC.size(), key.size(), step.size()};
  reply = (redisReply*) redisCommandArgv(conn_, ARGC_INC, argv, lens);

  if (!reply) {
    set_valid(false);
    MCE_DEBUG("RedisClient::Inc key " << key << " set_valid(false);" );
    return result;
  }
  if (reply->type == REDIS_REPLY_INTEGER) {
    result = boost::lexical_cast<string>(reply->integer);
    MCE_DEBUG("RedisClient::Inc() result:" << result << " key : " << key);
    int ttl = Ttl(key);
    MCE_DEBUG("RedisClient::Inc() address:" << address_ << " port:" << port_
        << " ttl:" << ttl << " reply->type:" << reply->type
        << " reply->integer:" << reply->integer << " key:" << key << " step:" << step);
    if(ttl == -1) { //不应该存在永远不过期的key
      MCE_WARN("RedisClient::Inc() address:" << address_ << " port:" << port_
          << " ttl:-1 " << reply->integer << " key:" << key << " step:" << step);
      Expire(key, 0);
      result = "";
    } else if(ttl == -2) { //获取ttl失败，不设置过期，但视为inc操作失败
      result = "";
      MCE_DEBUG("RedisClient::Inc() key " << key << " (ttl == -2)");
    }
  } else {
    MCE_WARN("RedisClient::Inc() address:" << address_ << " port:" << port_
        << " reply->type:" << reply->type << " reply->str:" << reply->str 
        << " reply->integer:" << reply->integer << " key:" << key << " step:" << step);
  }
  freeReplyObject(reply);
  return result;
}

/**
 * 查看过期时间
 * 返回-1表示用不过期或者key不存在
 * 返回-2表示调用失败
 *
 */
int RedisClient::Ttl(const std::string& key) {
  const static size_t ARGC_TTL = 2;
  const static string COM_TTL = "TTL";
  int result = 0;
  std::ostringstream otem;
  otem << COM_TTL << " " << key;
  std::string command = otem.str();

  redisReply* reply = 0;
  const char *argv[ARGC_TTL];
  argv[0] = COM_TTL.c_str();
  argv[1] = key.c_str();
  size_t lens[ARGC_TTL] = { COM_TTL.size(), key.size()};
  reply = (redisReply*) redisCommandArgv(conn_, ARGC_TTL, argv, lens);

  if (!reply) {
    set_valid(false);
    MCE_DEBUG("RedisClient::Ttl key " << key << " set_valid(false)");
    return result;
  }
  if (reply->type == REDIS_REPLY_INTEGER) {
    result = reply->integer;
    MCE_DEBUG("RedisClient::Ttl() address:" << address_ << " port:" << port_
        << " reply->type:" << reply->type
        << " reply->integer:" << reply->integer << " key:" << key);
  } else {
    result = -2;
    MCE_WARN("RedisClient::Ttl() address:" << address_ << " port:" << port_
        << " reply->type:" << reply->type << " reply->str:" << reply->str 
        << " reply->integer:" << reply->integer << " key:" << key);
  }
  freeReplyObject(reply);
  return result;
}

std::map<std::string, bool> RedisClient::MultiGet(const StringListType& keys, StringMapType* values) {
  std::map<std::string, bool> result;
  if (!values) {
    return result;
  }
  vector<const char *> argv;
  vector<size_t> argvlen;
  argv.reserve(kWriteBatchSize+2);
  argvlen.reserve(kWriteBatchSize+2);
  std::ostringstream otem;
  otem << "RedisClient::MultiGet() command argv: keys:";
  BOOST_FOREACH(const std::string& key, keys) {
    otem << key << "  ";
  }
  MCE_TRACE(otem.str());

  StringListType::const_iterator iter = keys.begin();
  while (iter != keys.end()) {
    argv.clear();
    argvlen.clear();
    static const char rpushcmd[] = "MGET";
    argv.push_back(rpushcmd);
    argvlen.push_back(sizeof(rpushcmd)-1);
    std::ostringstream otem;
    otem << "MGET ";
 
    StringListType::const_iterator old_iter = iter;
    for (int i = 0; (iter != keys.end()) && (i<kReadBatchSize); ++i,++iter) {
      argv.push_back(iter->c_str());
      argvlen.push_back(iter->size());
    }
    redisReply* reply = 0;
    reply = (redisReply*) redisCommandArgv(conn_, argv.size(), &(argv[0]), &(argvlen[0]) );
    if (!reply) {
      set_valid(false);
      return result;
    }
    if (reply->type != REDIS_REPLY_ARRAY) {
      freeReplyObject(reply);
      return result;
    }
    size_t fetch_size = reply->elements;
    if (fetch_size > 0) {
      for (size_t i = 0; i < fetch_size && old_iter != iter; ++i, ++old_iter) {
        if(reply->element[i]->type == REDIS_REPLY_STRING) {
          result.insert(std::make_pair(*old_iter, true));
          values->insert(std::make_pair(*old_iter, std::string(reply->element[i]->str, reply->element[i]->len)));
        }
      }
    }
    freeReplyObject(reply);
  }
  return result;
}
// changed by jipeng.lin because the limit<0 it must be return the total list!!!
bool RedisClient::GetList(const std::string& key, StringListType* value, int begin, int limit) {
  if (!value) {
    return false;
  }
  if (begin < 0) {
    return false;
  }
  if (limit <= 0) {
    //return false;
    //add code!!! by jipeng.lin
    std::ostringstream output;
    output << "LLEN " << key;
    std::string com = output.str();
    MCE_TRACE("RedisClient::GetList : command:" << com << "\t" << "address_:" << address_ << "\tport:" << port_);
    redisReply* reply = 0;
    reply = (redisReply*) redisCommand(conn_, com.c_str());
    if (!reply) {
      set_valid(false);
      return false;
    }
    if (reply->type == REDIS_REPLY_INTEGER) {
      limit = reply->integer - begin;
      freeReplyObject(reply);
    } else {
      set_valid(false);
      freeReplyObject(reply);
      return false;
    }
    if (limit <= 0) {
      return false;
    }
  }
  int start_pos = begin;
  int stop_pos = start_pos + limit - 1;
  while (start_pos <= stop_pos) {
    std::ostringstream otem;
    otem << "LRANGE " << key << " ";
    int batch_end = start_pos + kReadBatchSize - 1;
    batch_end = batch_end < stop_pos ? batch_end : stop_pos;
    otem << start_pos << " " << batch_end;

    std::string command = otem.str();
    MCE_TRACE("RedisClient::GetList() command:" << command);
    // do redis command 
    redisReply* reply = 0;
    reply = (redisReply*) redisCommand(conn_, command.c_str());
    if (!reply) {
      set_valid(false);
      return false;
    }
    if (reply->type != REDIS_REPLY_ARRAY) {
      freeReplyObject(reply);
      return false;
    }
    size_t fetch_size = reply->elements;
    if (fetch_size > 0) {
      for (size_t i = 0; i < fetch_size; ++i) {
        std::string tempStr;
        tempStr.assign(reply->element[i]->str, reply->element[i]->len);
        value->push_back(tempStr);
      }
    }
    if (fetch_size == kReadBatchSize) {
      start_pos = ++batch_end;
    }
    if (fetch_size >= 0 && fetch_size < kReadBatchSize) {
      start_pos = stop_pos + 1;
    }
    freeReplyObject(reply);
  }
  if (value->size() == 0) {
    MCE_TRACE("RedisClient::GetList value->size() = 0");
    if (Exists(key) != kReplySucess) {
      return false;
    }
  }
  return true;
}

bool RedisClient::SetList(const std::string& key, const StringListType& list, int secs) {
  // 1. generate temp key
  // 2. set list by batch
  // 3. rename 
  if (list.empty()) {
    return false;
  }
  if (secs < 0) {
    return false;
  }

  const std::string temp_key = GeneratorTempKey(key);
  MCE_DEBUG("key:" << key << "\ttemp_key:" << temp_key);

  vector<const char *> argv;
  vector<size_t> argvlen;
  argv.reserve(kWriteBatchSize+2);
  argvlen.reserve(kWriteBatchSize+2);

  StringListType::const_iterator iter = list.begin();
  while (iter != list.end()) {
    argv.clear();
    argvlen.clear();

    static const char rpushcmd[] = "RPUSH";
    argv.push_back(rpushcmd);
    argvlen.push_back(sizeof(rpushcmd)-1);
 
    argv.push_back(temp_key.c_str());
    argvlen.push_back(temp_key.size());

    std::ostringstream otem;
    otem << "RPUSH " << temp_key << " ";

    for (int i = 0; i < kWriteBatchSize && iter != list.end(); ++i) {
      argv.push_back(iter->c_str());
      argvlen.push_back(iter->size());
      otem << *iter++ << " ";
    }

    std::string command = otem.str();
//    MCE_DEBUG("RedisClient::SetList() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
//    for (int tempi = 0; tempi < argv.size(); ++tempi) {
//      MCE_DEBUG("RedisClient::SetList() command argv:" << argv[tempi] << "\t" << "argv length:" << argvlen[tempi]);
//    }

    redisReply* reply = 0;
    reply = (redisReply*) redisCommandArgv(conn_, argv.size(), &(argv[0]), &(argvlen[0]) );
    if (!reply) {
      set_valid(false);
      return false;
    }
    if (reply) {
      freeReplyObject(reply);
    }
  }
  if (!Rename(temp_key, key)) {
    return false;
  }
  if (secs > 0) {
    return Expire(key,secs);
  }
  return true;
}

bool RedisClient::GetHash(const std::string& key, const StringListType& fields, StringMapType* hash_value) {
  if (fields.empty()) {
    return false;
  }
  if (!hash_value) {
    return false;
  }
  std::ostringstream otem;
  otem << "HMGET " << key << " ";
  BOOST_FOREACH(const std::string& field, fields) {
    otem << field << " ";
  }
  std::string command = otem.str();
  MCE_TRACE("RedisClient::GetHash() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  if (!reply) {
    set_valid(false);
    return false;
  }

  if (reply->type != REDIS_REPLY_ARRAY) {
    freeReplyObject(reply);
    return false;
  }

  StringListType::const_iterator iter = fields.begin();
  for (size_t i = 0; i < reply->elements && iter != fields.end(); ++i, ++iter) {
    if (reply->element[i]->str != 0) {
      std::string tempStr;
      tempStr.assign(reply->element[i]->str, reply->element[i]->len);
      hash_value->insert(std::make_pair(*iter, tempStr));
    }
  }
  freeReplyObject(reply);
  return true;
}

bool RedisClient::HMSet(const std::string& key, const StringMapType& hash_value) {
  if (hash_value.empty()) {
    return false;
  }
  
  vector<const char *> argv;
  vector<size_t> argvlen;
  argv.reserve(2*kWriteBatchSize+2);
  argvlen.reserve(2*kWriteBatchSize+2);

  StringMapType::const_iterator iter = hash_value.begin();
  while (iter != hash_value.end()) {
    argv.clear();
    argvlen.clear();

    static const char hmsetcmd[] = "HMSET";
    argv.push_back(hmsetcmd);
    argvlen.push_back(sizeof(hmsetcmd)-1);

    argv.push_back(key.c_str());
    argvlen.push_back(key.size());

    std::ostringstream otem;
    otem << "HMSET " << key << " ";

    for(int i = 0; i < kWriteBatchSize && iter != hash_value.end(); ++i, ++iter) {
      argv.push_back((iter->first).c_str());
      argvlen.push_back((iter->first).size());
      argv.push_back((iter->second).c_str());
      argvlen.push_back((iter->second).size());
      otem<< iter->first << " " << iter->second << " ";
    }
    
    std::string command = otem.str();
    MCE_DEBUG("RedisClient::HMSet() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
    for(int tempi = 0; tempi < argv.size(); ++tempi) {
      MCE_DEBUG("RedisClient::HMSet() command argv:" << argv[tempi] << "\t" << "argv length:" << argvlen[tempi]);
    }

    redisReply* reply = 0;
    reply = (redisReply*) redisCommandArgv(conn_, argv.size(), &(argv[0]), &(argvlen[0]));
    if (!reply) {
      set_valid(false);
      return false;
    }
    if(reply) {
      freeReplyObject(reply);
    }
  }
  int ttl = Ttl(key);
  if (ttl == -1) {
    MCE_WARN("RedisClient::HMSet() address:" << address_ << " port:" << port_ << " ttl:-1 key:" << key);
    Expire(key, 0);
    return false;
  } else if (ttl == -2) {
    MCE_DEBUG("RedisClient::HMSet() key " << key << " (ttl == -2)");
    return false;
  }
  return true;
}

bool RedisClient::HDel(const std::string& key, const StringListType& fields) {
  if (fields.empty()) {
    MCE_DEBUG("RedisClient::HDel() the fields is empty");
    return false;
  }
  std::ostringstream otem;
  otem << "HDEL " << key << " ";
  BOOST_FOREACH(const std::string& field, fields) {
    otem << field << " ";
  }
  std::string command = otem.str();
  MCE_DEBUG("RedisClient::HDel() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  if (!reply) {
    set_valid(false);
    return false;
  }
  if (reply->type != REDIS_REPLY_INTEGER) {
    freeReplyObject(reply);
    return false;
  }
  MCE_DEBUG("reply->type:" << reply->type << "\treply->integer:" << reply->integer);
  
  freeReplyObject(reply); 
  return true;
}

bool RedisClient::HGetAll(const std::string& key, StringMapType* hash_value) {
  if (!hash_value) {
    return false;
  }

  std::ostringstream otem;
  otem << "HGETALL " << key;
  std::string command = otem.str();
  MCE_DEBUG("RedisClient::HGetAll() command:" << command << "\t" << "address_:" << address_ << "\tport: " << port_);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  
  if (!reply) {
    set_valid(false);
    return false;
  }
  if (reply->type != REDIS_REPLY_ARRAY) {
    freeReplyObject(reply);
    return false;
  }
  if ((reply->elements % 2) != 0) {
    freeReplyObject(reply);
    return false;
  } 
  
  MCE_DEBUG("HGETALL has return the elements :"<< reply->elements); 
  for (size_t i = 0; i < reply->elements; i+=2) {
    if((reply->element[i]->str != 0)&&(reply->element[i+1]->str != 0)) {
      std::string tempStr1;
      tempStr1.assign(reply->element[i]->str, reply->element[i]->len);
      std::string tempStr2;
      tempStr2.assign(reply->element[i+1]->str, reply->element[i+1]->len);
      hash_value->insert(std::make_pair(tempStr1, tempStr2));
    }
  }
  freeReplyObject(reply);
  return true;
}

bool RedisClient::HIncrBy(const std::string& key, const std::string& field, const std::string& step) {
  std::ostringstream otem;
  otem << "HINCRBY " << key << " " << field << " " << step;
  std::string command = otem.str();
  MCE_DEBUG("RedisClient::HIncrBy() command:" << command << "\t" << "address_" << address_ << "\tport:" << port_);

  redisReply* reply = 0;
  const char *argv[4];
  argv[0] = "HINCRBY";
  argv[1] = key.c_str();
  argv[2] = field.c_str();
  argv[3] = step.c_str();

  size_t lens[4] = {7, key.size(), field.size(), step.size()};
  int argc = 4;
  
  MCE_DEBUG("RedisClient::HIncrBy() command:" << argv[0] << " " << argv[1] << " " << argv[2] << " " << argv[3]);
  MCE_DEBUG("RedisClient::HIncrBy() command argv lens:" << lens[0] << " " << lens[1] << " " << lens[2] << " " << lens[3]);

  reply = (redisReply*) redisCommandArgv(conn_, argc, argv, lens);
  if (!reply) {
    set_valid(false);
    return false;
  }
  MCE_DEBUG( otem.str() <<"reply->type:" << reply->type << "\t" << "reply->str:" << reply->str << "\t" << "reply->integer:" << reply->integer);
  if (reply->type == REDIS_REPLY_ERROR) {
    set_valid(false);
    freeReplyObject(reply);
    return false;
  }
  
  if (reply->type == REDIS_REPLY_INTEGER) {
    freeReplyObject(reply);
    return true;
  } 
  
  freeReplyObject(reply);
  return false;
}

bool RedisClient::SetHash(const std::string& key, const StringMapType& hash_value, int secs) {
  if (hash_value.empty()) {
    return false;
  }

  if (secs < 0) {
    return false;
  }

  // 1. generate temp key
  // 2. set hash by batch
  // 3. rename 
  const std::string temp_key = GeneratorTempKey(key);
  MCE_DEBUG("key:" << key << "\ttemp_key:" << temp_key);

  vector<const char *> argv;
  vector<size_t> argvlen;
  argv.reserve(2*kWriteBatchSize+2);
  argvlen.reserve(2*kWriteBatchSize+2);

  StringMapType::const_iterator iter = hash_value.begin();
  while (iter != hash_value.end()) {
    argv.clear();
    argvlen.clear();

    static const char hmsetcmd[] = "HMSET";
    argv.push_back(hmsetcmd);
    argvlen.push_back(sizeof(hmsetcmd)-1);
 
    argv.push_back(temp_key.c_str());
    argvlen.push_back(temp_key.size());

    std::ostringstream otem;
    otem << "HMSET " << temp_key << " ";

    for (int i =0; i < kWriteBatchSize && iter != hash_value.end(); ++i, ++iter) {
      argv.push_back( (iter->first).c_str() );
      argvlen.push_back( (iter->first).size() );
      argv.push_back( (iter->second).c_str() );
      argvlen.push_back( (iter->second).size() );
      otem << iter->first << " " << iter->second << " ";
    }

    std::string command = otem.str();
    MCE_DEBUG("RedisClient::SetHash() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
    for (int tempi = 0; tempi < argv.size(); ++tempi) {
      MCE_DEBUG("RedisClient::SetHash() command argv:" << argv[tempi] << "\t" << "argv length:" << argvlen[tempi]);
    }

    redisReply* reply = 0;
    reply = (redisReply*) redisCommandArgv(conn_, argv.size(), &(argv[0]), &(argvlen[0]) );
    if (!reply) {
      set_valid(false);
      return false;
    }
    if (reply) {
      freeReplyObject(reply);
    }
  }
  if (!Rename(temp_key, key)) {
    return false;
  }
  if (secs > 0) {
    return Expire(key,secs);
  }
  return true;
}

int RedisClient::Exists(const std::string& key) {
  std::ostringstream otem;
  otem << "EXISTS " << key;
  std::string command = otem.str();
  MCE_TRACE("RedisClient::Exists() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  if (!reply) {
    MCE_TRACE("RedisClient::Exists() redisCommand return NULL!!!");
    set_valid(false);
    return kReplyConnectionError;
  }
  if (reply->type == REDIS_REPLY_ERROR) {
    freeReplyObject(reply);
    MCE_TRACE("reply->type:" << reply->type << "\treply->str:" << reply->str <<"\treply->integer:" << reply->integer);
    return kReplyConnectionError;
  }
  if (reply->type == REDIS_REPLY_INTEGER) {
    MCE_TRACE("reply->type:" << reply->type << "\treply->integer:" << reply->integer);
    int ret = (reply->integer == 1 ? kReplySucess : kReplyMiss);
    freeReplyObject(reply);
    return ret;
  }
  return kReplyMiss;
}

bool RedisClient::Remove(const std::string& key) {
  std::ostringstream otem;
  otem << "DEL " << key;
  std::string command = otem.str();
  MCE_DEBUG("RedisClient::Remove() command:" << command);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  if (!reply) {
    return false;
  }
  if (reply->type == REDIS_REPLY_ERROR) {
    freeReplyObject(reply);
    return false;
  }
  freeReplyObject(reply);
  return true;
}

int RedisClient::Size(const std::string& key) {
  std::string type = Type(key);
  if (type == kRedisNoneType) {
    MCE_TRACE("The result of the redis Type is none string!!!");
    return  -1;
  }
  std::stringstream otem;
  if (type == kRedisStringType) {
    otem << "STRLEN " << key;
  }
  else if (type == kRedisListType) {
    otem << "LLEN " << key;
  }
  else if (type == kRedisHashType) {
    otem << "HLEN " << key;
  }
  std::string command = otem.str();
  MCE_TRACE("RedisClient::Size() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  if (!reply) {
    set_valid(false);
    return -1;
  }
  MCE_TRACE("reply->type:" << reply->type << "\t" << "reply->integer:" << reply->integer);
  if (reply->type == REDIS_REPLY_ERROR) {
    set_valid(false);
    freeReplyObject(reply);
    return -1;
  }
  if (reply->type == REDIS_REPLY_INTEGER) {
    int size = reply->integer;
    freeReplyObject(reply);
    return size;
  }
  freeReplyObject(reply);
  return -1;
}


bool RedisClient::Rename(const std::string& old_name, const std::string& new_name) {
  std::ostringstream otem;
  otem << "RENAME " << old_name << " " << new_name;
  std::string command = otem.str();
  MCE_DEBUG("RedisClient::Rename() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  if (!reply) {
    set_valid(false);
    return false;
  }
  if (reply->type == REDIS_REPLY_ERROR) {
    set_valid(false);
    freeReplyObject(reply);
    return false;
  }
  freeReplyObject(reply);
  return true;
}

bool RedisClient::Expire(const std::string& key, int secs) {
  std::ostringstream otem;
  otem << "EXPIRE " << key << " " << secs;
  std::string command = otem.str();
  MCE_DEBUG("RedisClient::Expire() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  if (!reply) {
    set_valid(false);
    return false;
  }
  if (reply->type == REDIS_REPLY_ERROR) {
    set_valid(false);
    freeReplyObject(reply);
    return false;
  }
  if (reply->type == REDIS_REPLY_INTEGER && reply->integer == 1) {
    freeReplyObject(reply);
    return true;
  }
  freeReplyObject(reply);
  return false;
}

bool RedisClient::ZSet(const std::string& key, const StringMapType& set_with_scores, int secs) {
  if(set_with_scores.empty()) {
    return false;
  } 
  if (secs < 0) {
    return false;
  }
  const std::string temp_key = GeneratorTempKey(key);
  MCE_DEBUG("key: " << key << "\ttemp_key: " << temp_key);

  vector<const char *> argv;
  vector<size_t> argvlen;
  argv.reserve(2*kWriteBatchSize+2);
  argvlen.reserve(2*kWriteBatchSize+2);

  StringMapType::const_iterator iter = set_with_scores.begin();
  while (iter != set_with_scores.end()) {
    argv.clear();
    argvlen.clear();

    static const char zaddcmd[] = "ZADD";
    argv.push_back(zaddcmd);
    argvlen.push_back(sizeof(zaddcmd)-1);

    argv.push_back(temp_key.c_str());
    argvlen.push_back(temp_key.size());

    std::ostringstream otem;
    otem << "ZADD " << temp_key << " ";
    for (int i = 0; i < kWriteBatchSize && iter != set_with_scores.end(); ++i, ++iter) {
      argv.push_back(iter->first.c_str());
      argvlen.push_back(iter->first.size());
      argv.push_back(iter->second.c_str());
      argvlen.push_back(iter->second.size());
      otem << iter->first << " " << iter->second << " "; 
    }
    std::string command = otem.str();
    redisReply* reply = 0;
    reply = (redisReply*) redisCommandArgv(conn_, argv.size(), &(argv[0]), (&argvlen[0]) );
    if (!reply) {
      set_valid(false);
      return false;
    }
    if (reply->type != REDIS_REPLY_INTEGER) {
      freeReplyObject(reply);
      return false;
    }
    if (reply) {
      freeReplyObject(reply);
    }    
  }
  if (!Rename(temp_key, key)) {
    return false; 
  }
  if (secs > 0) {
    return Expire(key, secs);
  } 
  return true; 
}


bool RedisClient::ZAdd(const std::string& key, const StringMapType& set_with_scores) {
  if (set_with_scores.empty()) {
    return false;
  }
  
  vector<const char *> argv;
  vector<size_t> argvlen;
  argv.reserve(2*kWriteBatchSize+2);
  argvlen.reserve(2*kWriteBatchSize+2);

  StringMapType::const_iterator iter = set_with_scores.begin();
  while (iter != set_with_scores.end()) {
    argv.clear();
    argvlen.clear();
    
    static const char zaddcmd[] = "ZADD";
    argv.push_back(zaddcmd);
    argvlen.push_back(sizeof(zaddcmd) - 1);
    
    argv.push_back(key.c_str());
    argvlen.push_back(key.size());
    
    std::ostringstream otem;
    otem << "ZADD " << key << " ";
  
    for(int i = 0; i < kWriteBatchSize && iter != set_with_scores.end(); ++i, ++iter) {
      argv.push_back((iter->first).c_str());
      argvlen.push_back((iter->first).size());
      argv.push_back((iter->second).c_str());
      argvlen.push_back((iter->second).size());
      otem << iter->first << " " << iter->second << " ";
    }
    
    std::string command = otem.str();
    MCE_DEBUG("RedisClient::ZADD() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
    for(int tempi = 0; tempi < argv.size(); ++tempi) {
      MCE_DEBUG("RedisClient::ZADD() command argv:" << argv[tempi] << "\t" << "argv length:" << argvlen[tempi]);
    }

    redisReply* reply = 0;
    reply = (redisReply*) redisCommandArgv(conn_, argv.size(), &(argv[0]), &(argvlen[0]));
    if (!reply) {
      set_valid(false);
      return false;
    }
    if (reply->type != REDIS_REPLY_INTEGER) {
      freeReplyObject(reply);
      return false;
    }
    if(reply) {
      freeReplyObject(reply);
    }
  }
  int ttl = Ttl(key);
  if (ttl == -1) {
    MCE_WARN("RedisClient::ZAdd() address:" << address_ << " port:" << port_
        << " ttl:-1  key:" << key);
    Expire(key, 0);
    return false;
  } else if (ttl == -2) {
    MCE_DEBUG("RedisClient::ZAdd() key " << key << " (ttl == -2)" );
    return false;
  }
  return true;
}

bool RedisClient::ZRemRangeByRank(const std::string& key, int begin, int end) {
  std::ostringstream otem;
  otem << "ZREMRANGEBYRANK " << key << " " << begin << " " << end;
  std::string command = otem.str();
  MCE_DEBUG("RedisClient::ZRemRangeByRank() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  if (!reply) {
    set_valid(false);
    return false;
  }
  MCE_DEBUG("reply->type:" << reply->type << "\t" << "reply-str:" << reply->str << "\t" << "reply->integer:" << reply->integer);
  if (reply->type == REDIS_REPLY_ERROR) {
    //set_valid(false);
    freeReplyObject(reply);
    return false;
  }
  if (reply->type == REDIS_REPLY_INTEGER) {
    freeReplyObject(reply);
    return true;
  }
  freeReplyObject(reply);
  return false;
}

bool RedisClient::ZRemRangeByScore(const std::string& key, const std::string& min, const std::string& max) {
  std::ostringstream otem;
  otem << "ZREMRANGEBYSCORE " << key << " " << min << " " << max;
  std::string command = otem.str();
  MCE_DEBUG("RedisClient::ZRemRangeByScore() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  if (!reply) {
    set_valid(false);
    return false;
  }
  MCE_DEBUG("reply->type:" << reply->type << "\t" << "reply-str:" << reply->str << "\treply->integer:" << reply->integer);
  if (reply->type == REDIS_REPLY_ERROR) {
//    set_valid(false);
    freeReplyObject(reply);
    return false;
  }
  if (reply->type == REDIS_REPLY_INTEGER) {
    freeReplyObject(reply);
    return true;
  }
  freeReplyObject(reply);
  return false;
}

bool RedisClient::ZRange(const std::string& key, int begin, int limit, StringListType* zset_value) {
  if (!zset_value) {
    return false;
  }
  if (begin < 0) {
    return false;
  } else if (limit == 0) {
    return true;
  }
  std::stringstream output;
  int end = 0;
  if (limit < 0) {
    end = limit;
    output << "ZRANGE " << key << " " << begin  << " " << end;
  } else {
    end = begin + limit - 1; 
    output << "ZRANGE " << key << " " << begin  << " " << end;
  }
  std::string command = output.str();
  MCE_TRACE("RedisClient::ZRange() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  if (!reply) {
    set_valid(false);
    return false;
  }
  if (reply->type != REDIS_REPLY_ARRAY) {
    freeReplyObject(reply);
    return false;
  }
  size_t fetch_size = reply->elements;
  if (fetch_size > 0) {
    MCE_TRACE("RedisClient::ZRange(const std::string& key " << key << " size: " << fetch_size);
    for (size_t i = 0; i < fetch_size; ++i) {
      std::string tempStr;
      tempStr.assign(reply->element[i]->str, reply->element[i]->len);
      zset_value->push_back(tempStr); 
    }
  }
  if (zset_value->size() == 0) {
    MCE_TRACE("RedisClient::ZRange() zset_value->size() = 0");
    if (Exists(key) != kReplySucess) {
      freeReplyObject(reply);
      return false;
    }
  }
  freeReplyObject(reply);
  return true;
}

std::string RedisClient::Type(const std::string& key) {
  std::ostringstream otem;
  otem << "TYPE " << key;
  std::string command = otem.str();
  MCE_TRACE("RedisClient::Type() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  if (!reply) {
    set_valid(false);
    return kRedisNoneType;
  }
  MCE_TRACE("reply->type:" << reply->type << "\treply->str:" << reply->str);
  std::string result;
  if (reply->type == REDIS_REPLY_ERROR) {
    MCE_DEBUG("The Type() operator have return the REDIS_REPLY_ERROR");
    set_valid(false);
    freeReplyObject(reply);
    return kRedisNoneType;
  }
  if (reply->type == REDIS_REPLY_STATUS) {
    result = std::string(reply->str);
    freeReplyObject(reply);
    return result;
  }
  freeReplyObject(reply);
  return kRedisNoneType;
}



std::string RedisClient::Ping() {
  static std::string command = "PING";
  MCE_TRACE("RedisClient::Ping() command:" << command << "\t" << "address_:" << address_ << "\tport:" << port_);
  redisReply* reply = 0;
  reply = (redisReply*) redisCommand(conn_, command.c_str());
  if (!reply) {
    set_valid(false);
    return kRedisNoneType;
  }
  MCE_TRACE("reply->type:" << reply->type << "\treply->str:" << reply->str);
  std::string result;
  if (reply->type == REDIS_REPLY_ERROR) {
    set_valid(false);
    freeReplyObject(reply);
    return result;
  }
  if (reply->type == REDIS_REPLY_STATUS) {
    result = std::string(reply->str);
    freeReplyObject(reply);
    return result;
  }
  freeReplyObject(reply);
  return result;
}

// TODO: generate the random string value
std::string RedisClient::GeneratorTempKey(const std::string& key) {
  return xce::MD5(key);
}

//-------

RedisClientPool::RedisClientPool(const std::string address, int port, int timeout, int core_size, int max_size) 
  : CacheClientPool(address, port, timeout, core_size, max_size) {

}

RedisClientPool::~RedisClientPool() {
  MCE_DEBUG("RedisClientPool::~RedisClientPool()");
}

CacheClient* RedisClientPool::Create() {
  CacheClient* client = new RedisClient(address(), port(), timeout());
  if (client->IsValid()) {
    return client;
  }
  else {
    MCE_WARN("RedisClientPool::Create :: create invalid client");
    Destroy(client);
  }
  return NULL;
}

// ----------
boost::mutex CacheClientPoolFactory::mutex_;
std::map<std::pair<std::string, int>, CacheClientPoolPtr> CacheClientPoolFactory::pool_map_;

CacheClientPoolPtr CacheClientPoolFactory::GetPool(const std::string& address, int port, int conn_time_out) {
  boost::lock_guard<boost::mutex> lock(mutex_);
  std::map<std::pair<std::string, int>, CacheClientPoolPtr>::const_iterator iter = pool_map_.find(std::make_pair(address, port));
  if (iter != pool_map_.end()) {
    return iter->second;
  }
  else {
    CacheClientPoolPtr pool;
    pool.reset(new RedisClientPool(address, port, conn_time_out));
    if (pool) {
//      pool_map_.insert(std::make_pair(address, port), pool);
      pool_map_[std::make_pair(address, port)] = pool;
      return pool;
    }
    return CacheClientPoolPtr();
  }
}

}} // xce::tripod


