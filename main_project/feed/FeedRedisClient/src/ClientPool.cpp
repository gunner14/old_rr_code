#include "ClientPool.h"
#include <sstream>
#include "LogWrapper.h"

namespace xce {
namespace feed {

RedisClientPool::RedisClientPool(const std::string& address, int port, long timeout, size_t max_size):address_(address), port_(port), timeout_(timeout), max_size_(max_size),used_(0) { }

RedisClientPool::~RedisClientPool() { 
#ifdef STDOUT
  std::cout <<"RedisClientPool::~RedisClientPool() --> used:" << used_ << " free_clients size:" << free_clients_.size() << " address:" << address_ << " port:" << port_ << std::endl;
#else
  MCE_INFO("RedisClientPool::~RedisClientPool() --> used:" << used_ << " free_clients size:" << free_clients_.size() << " address:" << address_ << " port:" << port_);
#endif


  // boost::mutex::scoped_lock lock(free_clients_mutex_);
  boost::lock_guard<boost::mutex> lock(free_clients_mutex_);
  std::deque<RedisClientPtr>::iterator it = free_clients_.begin();
  for (; it != free_clients_.end(); ++it) {
    //DestroyClient(*it);
    if (*it) {
      (*it).reset();
    }
  }
}

RedisClientPtr RedisClientPool::Create() {
  RedisClientPtr client(new RedisClient(address_, port_, timeout_));
  if (!client) {
    return client; //NULL
  }

  if (client->IsValid()) {
    return client;
  } else {
    // DestroyClient(client);
    if (client) {
      client.reset();
    }
   
  }    
  return client; // NULL
}

//void RedisClientPool::DestroyClient(RedisClientPtr client) {
//  if (client) {
//    client.reset();
//    std::cout << "RedisClientPool::DestroyClient." << std::endl;
//  }
//}

RedisClientPtr RedisClientPool::GetClient() {
  RedisClientPtr client;

  boost::lock_guard<boost::mutex> lock(free_clients_mutex_);
  if (free_clients_.empty()) {
    if (used_ < max_size_)  {
      client = Create();
    }

    ///////////////////
    else {

      MCE_WARN("RedisClientPool::GetClient --> used:" << used_ << " free_clients size:" << free_clients_.size() << " address:" << address_ << " port:" << port_);
      return client;
    }
    //////////////////
    
  } else {
#ifdef STDOUT
    std::cout <<"RedisClientPool::GetClient --> used:" << used_ << " free_clients size:" << free_clients_.size() << " address:" << address_ << " port:" << port_ << std::endl;
#else
    MCE_INFO("RedisClientPool::GetClient --> used:" << used_ << " free_clients size:" << free_clients_.size() << " address:" << address_ << " port:" << port_);
#endif
    client = free_clients_.front();
    free_clients_.pop_front();
  }

  if (client != NULL) {
    used_++;
  } else {
    MCE_WARN("RedisClientPool::GetClient --> client is NULL!");
  }

  return client;
}

void RedisClientPool::ReleaseClient(RedisClientPtr client) {
  if (client != NULL) {
    if (client->IsValid()) {
#ifdef STDOUT
      std::cout <<"RedisClientPool::ReleaseClient --> return client. used:" << used_ << " free_clients size:" << free_clients_.size() << " address:" << address_ << " port:" << port_ << std::endl;
#else
      MCE_INFO("RedisClientPool::ReleaseClient --> return client. used:" << used_ << " free_clients size:" << free_clients_.size() << " address:" << address_ << " port:" << port_);
#endif
      //boost::mutex::scoped_lock lock(free_clients_mutex_);
      boost::lock_guard<boost::mutex> lock(free_clients_mutex_);
      free_clients_.push_back(client);
      used_--;
    } else {
      //DestroyClient(client);
      client.reset();
      //boost::mutex::scoped_lock lock(free_clients_mutex_);
      boost::lock_guard<boost::mutex> lock(free_clients_mutex_);
      used_--;
    }
  }
}



std::string RedisClientPool::Dump() {
  //boost::mutex::scoped_lock lock(free_clients_mutex_);
  boost::lock_guard<boost::mutex> lock(free_clients_mutex_);
  std::ostringstream oss;
  oss << "RedisClientPool::Dump --> address:" << address_  << " port:" << port_ << " max_size:" << max_size_ << " used:" << used_ << " free_clients.size:" << free_clients_.size();
  return oss.str();
}

//-------------------------------------------------------------------------------
//boost::mutex RedisClientPoolFactory::mutex_;
//std::map<std::pair<std::string, int>, RedisClientPoolPtr> RedisClientPoolFactory::pool_map_;

RedisClientPoolPtr RedisClientPoolFactory::GetPool(const std::string& address, int port) {
	//boost::lock_guard<boost::mutex> lock(mutex_);


  {
    ReadLock rl(&rwlock_);
    std::map<std::pair<std::string, int>, RedisClientPoolPtr>::const_iterator iter = pool_map_.find(std::make_pair(address, port));
    if (iter != pool_map_.end()) {
      return iter->second;
    }
  }

	{

		RedisClientPoolPtr pool(new RedisClientPool(address, port));

    WriteLock wl(&rwlock_);
		if (pool) {
			pool_map_[std::make_pair(address, port)] = pool;
		} 
    MCE_WARN("RedisClientPoolFactory::GetPool --> pool is NULL!");
    return pool;
	}
}

RedisClientPtr RedisClientPoolFactory::GetClient(const std::string& address, int port) {
  RedisClientPoolPtr pool = GetPool(address, port);
  RedisClientPtr client;
  if (pool) {
    client = pool->GetClient();
  }
  return client;

}


void RedisClientPoolFactory::ReleaseClient(RedisClientPtr client) {
	if (!client) {
		return;
	}
	std::string addr = client->address();
	int port = client->port();

	//boost::lock_guard<boost::mutex> lock(mutex_);
  WriteLock wl(&rwlock_);

	std::map<std::pair<std::string, int>, RedisClientPoolPtr>::const_iterator iter = pool_map_.find(std::make_pair(addr, port));

	if (iter != pool_map_.end()) {
		return iter->second->ReleaseClient(client);
	} else {
#ifdef STDOUT
		std::cout << "RedisClientPoolFactory::ReleaseClient --> ClientPool can not be found! addr:" << addr << " port:" << port << std::endl; 
#else
		MCE_WARN("RedisClientPoolFactory::ReleaseClient --> ClientPool can not be found! addr:" << addr << " port:" << port); 
#endif
	}

}

void RedisClientPoolFactory::Dump() {
  ostringstream oss;
  oss << " RedisClientPoolFactory::Dump -->" << std::endl;
  {
    //boost::lock_guard<boost::mutex> lock(mutex_);
    ReadLock rl(&rwlock_);
    int i = 1;
    std::map<std::pair<std::string, int>, RedisClientPoolPtr>::const_iterator it = pool_map_.begin();
    for(; it != pool_map_.end(); it++, i++) {
      oss << i << " addr:" << it->first.first << " port:" << it->first.second << " used:" << it->second->used() << " free size:" << it->second->GetFreeClientsSize() << std::endl; 
    }
  }
  //MCE_INFO(oss.str()); 
//#ifdef STDOUT
  std::cout << oss.str();
//#endif


}


}
}

//void pp() {
//  xce::feed::RedisClientPoolFactory::Dump();
//}

/*
struct stu {
  int a;
  long b;
};


int main() {
//	xce::feed::RedisClientPoolPtr pool = xce::feed::RedisClientPoolFactory::instance().GetPool("10.3.22.141", 6379);
	xce::feed::RedisClientPtr client = xce::feed::RedisClientPoolFactory::instance().GetClient("10.3.22.141", 6379);

//   sleep(30);
 //  std::cout << " sleep end." << std::endl;

  stu obj;
  obj.a=1;
  obj.b =2;
  char arr[100];
  *(static_cast< stu * >((void*)arr)) = obj;

  xce::feed::RedisBinArgv arg("list31");
  arg.PushArg(arr, sizeof(obj));
//  arg.PushArg("a11", 3);
//  client->Ping();
  client->Push2List(arg);

   xce::feed::RedisClientPoolFactory::instance().ReleaseClient(client); 


  // pool = xce::feed::RedisClientPoolFactory::instance().GetPool("10.3.22.141", 6379);	
  // client = pool->GetClient();
//	client = xce::feed::RedisClientPoolFactory::instance().GetClient("10.3.22.141", 6379);
//  //sleep(30);
//
//
//	//cl.GetList<cls>("list20",vec, 0,5);
	std::vector<stu> vec; 
	client->GetList<stu>("list31",vec, 0,5);
	for( std::vector<stu>::const_iterator it = vec.begin(); it != vec.end(); it++) {
		//std::cout << " str:" << *it << " len:" << it->size() << std::endl;
		std::cout << " a:" << it->a << " b:" << it->b << " " << sizeof(*it) << std::endl;
	}

//  client->Del("list27");
//
//	std::vector<std::string> vec2; 
//	client->GetList<std::string>("list30",vec2);
//	for( std::vector<std::string>::const_iterator it = vec2.begin(); it != vec2.end(); it++) {
//		std::cout << " str:" << *it << it->size() << std::endl;
//		//std::cout << " a:" << it->a << " b:" << it->b << " " << sizeof(*it) << std::endl;
//	}
		

	

}
*/

