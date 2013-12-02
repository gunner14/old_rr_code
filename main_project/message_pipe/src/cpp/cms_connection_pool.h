#ifndef  __MP_CMS_CONNECTION_POOL_H
#define __MP_CMS_CONNECTION_POOL_H

#include "Common.h"
#include <activemq/cmsutil/CmsTemplate.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/ConcurrentMap.h>
#include <decaf/util/concurrent/Mutex.h>

namespace xce {
namespace messagepipe {

using namespace activemq::cmsutil;
using namespace activemq::core;

//CmsTemplate可以帮助我们更好的管理ActiveMQ连接的资源,包括Connection,Session,Producer,Consumer
//使用连接池可以帮我们避免在同步发送的时候，并发对资源加锁导致的串行
//两外一个CmsTemplate对应一个broker的一个topic或者queue
	
class CmsConnectionPool {

public:
	CmsConnectionPool();
	CmsConnectionPool(const std::string& uri, int max_connection=1000);
	~CmsConnectionPool();
	CmsTemplate* takeConnection();
	void returnConnection(CmsTemplate* cmsTemplate);
private:
	int max_connection_;

	std::list<CmsTemplate*> avaliable_;
	std::list<ActiveMQConnectionFactory*> factories_;
  std::string uri_;
  IceUtil::Mutex mutex_;
	
};

}
}
#endif
