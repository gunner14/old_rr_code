#include "socialgraph/socialgraphutil/redis_adapter.h"

namespace xce {
namespace socialgraph {

void RedisAdapter::Init(const std::string& ip, int port, int timeout, size_t core_size, size_t max_size) {
	ip_ = ip;
	port_ = port;
	timeout_ = timeout;
	core_size_ = core_size;
	max_size_ = max_size;
	pool_ptr_ = new ObjectPool<RedisClient>(core_size_, max_size_);
	ObjectFactory<RedisClient> *f = new ThreeArgsFactory<RedisClient, std::string, int, int>(ip, port, timeout);
	pool_ptr_->SetFactory(f);
}

int RedisAdapter::GetListSize(const std::string& key) const {
	int result = -1;
	int loop = 0;
	while (++loop <= 5) {
		RedisClientPtr p = pool_ptr_->BorrowObject();
		
		if (p->IsValid()) {
			result = p->GetListSize(key);
			pool_ptr_->ReturnObject(p);
			break;
		} else {
			pool_ptr_->Detach();
		}
	}
	return result;
}

void RedisAdapter::GetList(const std::string& key, StrList& value, int begin, int limit) const {
	int loop = 0;
	while (++loop <= 5) {
		RedisClientPtr p = pool_ptr_->BorrowObject();
		
		if (p->IsValid()) {
			p->GetList(key, value, begin, limit);	
			pool_ptr_->ReturnObject(p);
			break;
		} else {
			pool_ptr_->Detach();
		}
	}
}

bool RedisAdapter::SetList(const std::string& key, const StrList& value) const {
	bool result = false;
	int loop = 0;
	while (++loop <= 5) {
		RedisClientPtr p = pool_ptr_->BorrowObject();
		
		if (p->IsValid()) {
			result = p->SetList(key, value);
			pool_ptr_->ReturnObject(p);
			break;
		} else {
			pool_ptr_->Detach();
		}
	}
	return result;
}

bool RedisAdapter::SetList(const std::string& key, const StrList& value, int expiretime) const {
	bool result = false;
	int loop = 0;
	while (++loop <= 5) {
		RedisClientPtr p = pool_ptr_->BorrowObject();
		
		if (p->IsValid()) {
			result = p->SetList(key, value, expiretime);
			pool_ptr_->ReturnObject(p);
			break;
		} else {
			pool_ptr_->Detach();
		}
	}
	return result;
}

bool RedisAdapter::SetExpire(const std::string& key, int timeout) const {
	bool result = false;
	int loop = 0;
	while (++loop <= 5) {
		RedisClientPtr p = pool_ptr_->BorrowObject();
		
		if (p->IsValid()) {
			result = p->SetExpire(key, timeout);
			pool_ptr_->ReturnObject(p);
			break;
		} else {
			pool_ptr_->Detach();
		}
	}
	return result;
}

bool RedisAdapter::Remove(const std::string& key) const {
	bool result = false;
	int loop = 0;
	while (++loop <= 5) {
		RedisClientPtr p = pool_ptr_->BorrowObject();
		
		if (p->IsValid()) {
			result = p->Remove(key);
			pool_ptr_->ReturnObject(p);
			break;
		} else {
			pool_ptr_->Detach();
		}
	}
	return result;
}

}}
