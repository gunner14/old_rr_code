/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file FeedRedisClient.h
* @Synopsis  
* @author di.liu@renren-inc.com
* @version 1.0
* @date 2012-08-28
*/
#ifndef __XCE_FEED_REDIS_CLIENT_H__
#define __XCE_FEED_REDIS_CLIENT_H__

#include <string>
#include <list>
#include <map>
#include "boost/thread/mutex.hpp"
#include "boost/shared_ptr.hpp"
#include "hiredis.h"
#include "LogWrapper.h"

#define  kWriteBatchSize  100
#define  kReadBatchSize  100
#define  kMaxListSize  200

namespace  xce {
namespace feed {

typedef std::list<std::string> StringListType;
typedef std::map<std::string, std::string> StringMapType;

class TimeLog {
public:
	TimeLog(const std::string log = ""): log_(log) {
		gettimeofday(&begin_, NULL);
	}

	~TimeLog() {
#ifdef STDOUT
    std::cout << log_ << " timeuse:" << getTime() << std::endl;
#else
		MCE_INFO(log_ << " timeuse:" << getTime());
#endif
	}

	float getTime() {
		gettimeofday(&end_, NULL);
		float timeuse = 1000000 * (end_.tv_sec - begin_.tv_sec) + end_.tv_usec
			- begin_.tv_usec;
		timeuse /= 1000;
		return timeuse;
	}

private:
	timeval begin_, end_;
	std::string  log_;
};

class RedisBinArgv {
public:

  RedisBinArgv(std::string key);
  bool PushArg(const char *value, size_t len);
  void SetCommand(std::string cmd);

  int count() const {
    return count_;
  }

  const char ** argv() {
    return argv_;
  }

  const size_t * argvlen() const{
    return argvlen_;
  }

  std::string key() const {
    return key_;
  }

 
private:
  int count_;
  std::string key_;
  const char *argv_[kWriteBatchSize + 2];
  size_t argvlen_[kWriteBatchSize + 2];

  
};

class RedisBinReply {
public:
  RedisBinReply(std::string key);
  bool PushReply(const char *reply, size_t len);

  int count() {
    return count_;
  }

  const char **replyv() {
    return replyv_;
  }

  size_t *replyvlen() {
    return replyvlen_;
  }

  std::string key() {
    return key_;
  }

private:
  int count_;
  std::string key_;
  const char *replyv_[kReadBatchSize];
  size_t replyvlen_[kReadBatchSize];
};

//class RedisClientPool;

class RedisClient {
public:
	
	RedisClient(const std::string& address, const int port, const long timeout = 1000);	
	//RedisClient(const std::string& address, const int port, RedisClientPool* owner, const long timeout = 1000);	
	~RedisClient();

//		bool Get(const std::string& key, std::string* value) = 0;
		// argc secs is the expire time from now
		// if secs == 0, no expire!
//		bool Set(const std::string& key, const std::string& value, int secs = 0) = 0;
		//成功返回结果字符串，失败返回空字符串
//		std::string Inc(const std::string& key, const std::string& step);
	//	virtual std::map<std::string, bool> MultiGet(const StringListType& keys,StringMapType* values);

	//template <class T> bool GetList(const std::string& key, RedisBinReply* reply_list, int begin = 0, int limit = 0);
  bool Push2List(RedisBinArgv& cmd_argv);

  template <class T>
  bool GetList(const std::string& key, std::vector<T>& ret_vec, int begin = 0, int limit = 1);
//

  bool Del(const std::string& key);

//	virtual bool GetHash(const std::string& key, const StringListType& fields, StringMapType* hash_value);
//	virtual bool HMSet(const std::string& key, const StringMapType& hash_value);
//	virtual bool HGetAll(const std::string& key, StringMapType* hash_value);
//	virtual bool HDel(const std::string& key, const StringListType& fields);
//	virtual bool HIncrBy(const std::string& key, const std::string& field, const std::string& step);
//	virtual bool SetHash(const std::string& key, const StringMapType& hash_value, int secs = 0);
//	virtual int  Exists(const std::string& key);
//	virtual bool Remove(const std::string& key);
//	virtual int  Size(const std::string& key);
	/////bool IsValid();
	// for monitor
  void SetValid(bool v) {
    valid_ = v;
  }
  bool IsValid() const {
    return valid_;
  }

  const std::string& address() const {
    return address_;
  }

  int port() const {
    return port_;
  }

	bool Ping();
private:
  bool CheckErr(const redisReply* reply, const redisContext* rc, std::string log);

private:
	std::string address_;
	int port_;
	long timeout_;
	redisContext* rc_;
	bool valid_;
//  RedisClientPool* owner_;
	static const std::string kEmptyString;
	
		
};

#include "generic_method/GetList.cpp"
template <>
inline bool RedisClient::GetList<std::string>(const std::string& key, std::vector<std::string>& ret_vec, int begin, int limit);
#include "generic_method/GetStrList.cpp"


}
}



#endif
