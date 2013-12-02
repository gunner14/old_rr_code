#ifndef _XOA2_CLIENT_POOL_H_
#define _XOA2_CLIENT_POOL_H_

#include <iostream>

#include <boost/thread.hpp>

#include "xoa2/client/xoa_client_factory.h"
#include "xoa2/client/service.h"
#include "UtilCxx/src/LogWrapper.h"

namespace xoa2 {
namespace client {
  
class ShardingStrategy {
 public:
  virtual int Shard(int factor) const = 0;
  virtual ~ShardingStrategy() {} 
};

class ModSharding : public ShardingStrategy {
 public:
  virtual int Shard(int factor) const {
    return factor % 10;
  }
  virtual ~ModSharding() {} 
};

// 可以是任意client类型, 经ClientWrap封装后放在pool中
template <typename ClientType>
class ClientPool {
 private:
  ClientFactory<ClientType> * factory_;

  // 各个地址，对应的client组
  typedef boost::shared_ptr<ClientWrap<ClientType> > ClientWrapPtr;

  typedef std::map<std::string, std::list<ClientWrapPtr> > PoolType;
  PoolType idle_pool_;
  // boost::mutex idle_mutex_;
  boost::shared_mutex idle_mutex_;

  typedef std::map<ClientType *, ClientWrapPtr> ActiveMap;
  ActiveMap active_pool_;
  // boost::mutex active_mutex_;
  boost::shared_mutex active_mutex_;

  ShardingStrategy * sharding_strategy_;

  //用来标识是否访问本地服务 
  ServicePtr service_;

 public:
  ClientPool(const char * service_id, const int timeout = 200, ShardingStrategy * sharding = NULL)
    : factory_(NULL)
    , sharding_strategy_(sharding)
    , service_(new Service(service_id)) {
      factory_ = new XoaClientFactory<ClientType>(timeout);
      if (!service_->Load()) {
        MCE_ERROR("service " << service_->id() << " load error.");
      }
  }

  virtual ~ClientPool() {
  }

  //获得所对应的客户端
  ClientType * Alloc(int shard = 0) {
    if (sharding_strategy_) {
      // shard = sharding_strategy_->Shard(shard);
    }

    std::string server_addr = service_->LocateService(shard);
    if (server_addr.empty()) {
      MCE_ERROR("server_addr is empty\n");
      return NULL;
    }

    ClientWrapPtr wrap;
    {
      // boost::mutex::scoped_lock(idle_mutex_);
      boost::unique_lock<boost::shared_mutex> wlock(idle_mutex_);
      std::list<ClientWrapPtr> & resources = idle_pool_[server_addr];

      while (!resources.empty()) {
        wrap = resources.front();
        resources.pop_front();
        MCE_DEBUG("Client pool not empty : " << wrap->client());
        break;
      }
    }

    if (!wrap) {
      MCE_DEBUG("try to created new Xoa2Client.");
      wrap.reset(factory_->Create(server_addr.c_str()));
    } else {
      MCE_DEBUG("Xoa2Client reused : " << wrap->client());
    }

    if (!wrap || !wrap->client()) {
      MCE_ERROR("wrap is null or wrap client is null\n");
      return NULL;
    }
    
    {
      // boost::mutex::scoped_lock(active_mutex_);
      boost::unique_lock<boost::shared_mutex> wlock(active_mutex_);

      typename ActiveMap::iterator it = active_pool_.find(wrap->client());
      if (it != active_pool_.end()) {
        // TODO: 致命错误！
        MCE_DEBUG("Xoa2Client collision : " << wrap->client());
      }

      pair<typename ActiveMap::iterator,bool> i_res = active_pool_.insert(std::make_pair(wrap->client(), wrap));
      if(!i_res.second) {
        // MCE_DEBUG("Xoa2Client insert error : " << wrap->client());
        return NULL;
      }
    }
    MCE_DEBUG("Xoa2Client allocated : " << wrap->client());
    return wrap->client();
  }

  // 释放客户端. Alloc()之后，务必调用Release().
  // success 表示RPC调用是否成功
  void Release(ClientType * client, bool success) {
    if (client == NULL) {
      return;
    }
    ClientWrapPtr wrap;

    {
      // boost::mutex::scoped_lock(active_mutex_);
      boost::unique_lock<boost::shared_mutex> wlock(active_mutex_);

      typename ActiveMap::iterator it = active_pool_.find(client);
      if (it == active_pool_.end()) {
        // TODO: 致命错误！
        MCE_WARN("Xoa2Client not found : " << client);
        return;
      }
      
      wrap = it->second;
      active_pool_.erase(it);
    }

    if (success) {
      MCE_DEBUG("Xoa2Client recycled : " << client);
      // boost::mutex::scoped_lock(idle_mutex_);
      boost::unique_lock<boost::shared_mutex> wlock(idle_mutex_);

      typename PoolType::iterator it = idle_pool_.find(wrap->endpoint_key());
      if (it != idle_pool_.end()) {
        std::list<ClientWrapPtr> & resources = it->second;
        
        if (resources.size() < 200) {
          resources.push_back(wrap);
        }
      }
    } else {
      MCE_DEBUG("Xoa2Client droped : " << client);
    }

    // service_->UpdateServiceState(wrap->endpoint_key());
  }
};

}
}

#endif // _XOA2_CLIENT_POOL_H_

