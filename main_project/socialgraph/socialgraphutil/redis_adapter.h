#ifndef REDIS_ADAPTER_H_
#define REDIS_ADAPTER_H_

#include <string>

#include "socialgraph/socialgraphutil/redisclient.h"
#include "socialgraph/socialgraphutil/objectpool.h"
#include "UtilCxx/src/Singleton.h"

namespace xce {
namespace socialgraph {
/*@brief RedisAdapter consist of RedisClient and ObjectPool used by application*/
class RedisAdapter : public MyUtil::Singleton<RedisAdapter> {
 public:
	virtual ~RedisAdapter() {
		if (pool_ptr_ != 0) {
			delete pool_ptr_;
		}
	}
	/*@brief Initialize parameters of adapter*/
	void Init(const std::string& ip, int port, int timeout, size_t core_size, size_t max_size);

	int GetListSize(const std::string& key) const;
	void GetList(const std::string& key, StrList& value, int begin = 0, int limit = 0) const;

	bool SetList(const std::string& key, const StrList& value) const;
	bool SetList(const std::string& key, const StrList& value, int expiretime) const;
	bool SetExpire(const std::string& key, int timeout) const; 

	bool Remove(const std::string& key) const;

	friend class MyUtil::Singleton<RedisAdapter>;

 private:
	RedisAdapter() { pool_ptr_ = 0; }

 private:
	std::string ip_;		//ip of Redis server
	int port_;		//port of Redis server
	int timeout_;		//time of Redis connection
	size_t core_size_;		//core size of connection pool
	size_t max_size_;		//max size of connection pool
	ObjectPool<RedisClient> *pool_ptr_;		//pointer of connection pool
};

}
}

#endif 
