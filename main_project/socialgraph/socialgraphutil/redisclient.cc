#include "socialgraph/socialgraphutil/redisclient.h"

#include <iostream>
#include <sstream>

#include <boost/foreach.hpp>
#include <boost/format.hpp>

namespace xce {
namespace socialgraph {

RedisClient::RedisClient(const std::string& ip, int port, int timeout):ip_(ip), port_(port), timeout_(timeout) {
	rc_ = 0;

	struct timeval tv;
	tv.tv_sec = timeout / 1000;		//seconds
	tv.tv_usec = 0;

	rc_ = redisConnectWithTimeout(ip_.c_str(), port_, tv);
	if (rc_->err) {
		SetValid(false);
		rc_ = false;
	} 
	else {
		SetValid(true);
	}
}

RedisClient::~RedisClient() {
	if (rc_) {
		redisFree(rc_);
	}
}

int RedisClient::GetListSize(const std::string& key) const {
	int length = -1;
	boost::format fmt("LLEN %1%");
	fmt % key;

	redisReply* reply = 0;
	reply = (redisReply*)redisCommand(rc_, fmt.str().c_str());
	if (!reply) {
		return length;
	}
	if (reply->type == REDIS_REPLY_INTEGER) {
		if (0 != reply->integer) 
			length = reply->integer;
	}
	freeReplyObject(reply);

	return length;
}

void RedisClient::GetList(const std::string& key, StrList& value, int begin, int limit) const {
	// 1. seek start and end
	// 2. set list
	int start_pos = 0;
	int end_pos = -1;
	if (begin > 0) {
		start_pos = begin;
	}
	if (limit > 0) {
		end_pos = start_pos + limit;
	}
	boost::format fmt("LRANGE %1% %2% %3%");
	fmt % key % start_pos % end_pos;

	redisReply* reply = 0;
	reply = (redisReply*)redisCommand(rc_, fmt.str().c_str());
	if (!reply) {
		return;
	}
	if (reply->type == REDIS_REPLY_ARRAY) {
		StrList tmplist;
		for (size_t i = 0; i < reply->elements; ++i) {
			tmplist.push_back(reply->element[i]->str);
		}
		tmplist.swap(value);
	}
	freeReplyObject(reply);
}

bool RedisClient::SetList(const std::string& key, const StrList& value) const {
	if (value.empty()) {
		return true;
	}
	
	std::ostringstream command;
	command << "RPUSH " << key;
	BOOST_FOREACH(const std::string& e, value) {
		command << " " << e;
	}
	redisReply* reply = 0;
	reply = (redisReply*)redisCommand(rc_, command.str().c_str());
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

bool RedisClient::SetList(const std::string& key, const StrList& value, int expiretime) const {
	bool r1 = SetList(key, value);
	bool r2 = SetExpire(key, expiretime);

	if (r1 && r2) {
		return true;
	}

	return false;
}

bool RedisClient::Remove(const std::string& key) const {
	boost::format fmt("DEL %1%");
	fmt % key;

	redisReply* reply = 0;
	reply = (redisReply*)redisCommand(rc_, fmt.str().c_str());
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

bool RedisClient::SetExpire(const std::string& key, int timeout) const {
	boost::format fmt("EXPIRE %1% %2%");
	fmt % key % timeout;

	redisReply* reply = 0;
	reply = (redisReply*)redisCommand(rc_, fmt.str().c_str());
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

}}
