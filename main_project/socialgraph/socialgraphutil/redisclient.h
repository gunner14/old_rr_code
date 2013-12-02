#ifndef REDIS_CLIENT_H_
#define REDIS_CLIENT_H_

#include <vector>
#include <string>

#include "hiredis.h"
#include <boost/shared_ptr.hpp>

namespace xce {
namespace socialgraph {

typedef std::vector<std::string> StrList;

class RedisClient {
 public:
	RedisClient(const std::string& ip, int port, int timeout = 5000);
	virtual ~RedisClient();

	bool IsValid() const {		//justify status of client
		return valid_ == true;
	}

	void SetValid(bool v) {
		valid_ = v;
	}

	/* @brief Get size of list
   * @param key 
   */
	int GetListSize(const std::string& key) const;
	
	/* @brief Get list according to key
   * @param key
   * @param value 
   * @param begin position started
   * @param limit	length of value 
   */
	void GetList(const std::string& key, StrList& value, int begin = 0, int limit = 0) const;

	/* @brief Set value of list or expire time*/
	bool SetList(const std::string& key, const StrList& value) const;
	bool SetList(const std::string& key, const StrList& value, int expiretime) const;

	/* @brief Set expire time
   * @param key 
   * @param timeout
   */
	bool SetExpire(const std::string& key, int timeout) const; 

	/* @brief del key
   * @param key 
   */
	bool Remove(const std::string& key) const;

 private:
	redisContext *rc_;
	std::string ip_;		// ip of redis server
	int port_;		// port of redis server
	int timeout_;		// timeout of connection
	bool valid_;		// status of client
};

typedef boost::shared_ptr<RedisClient> RedisClientPtr;		//define RedisClientPtr

}}

#endif
