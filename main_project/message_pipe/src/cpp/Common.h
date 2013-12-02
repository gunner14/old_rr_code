#ifndef _MP_COMMON_H__
#define _MP_COMMON_H__


#include <zookeeper/zookeeper.h>

#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/interprocess/detail/atomic.hpp>

#include <activemq/library/ActiveMQCPP.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/System.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQSession.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/core/ActiveMQProducer.h>
#include <activemq/core/ActiveMQConsumer.h>
#include <activemq/util/Config.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/Message.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <cms/ConnectionFactory.h>

#include <IceUtil/IceUtil.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Mutex.h>

#include <memory>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <iostream>

#include <LogWrapper.h>

namespace xce{
namespace messagepipe{

	class Node;
	typedef boost::shared_ptr<Node> NodePtr;

    class NodeSet;
    typedef boost::shared_ptr<NodeSet> NodeSetPtr;

	typedef boost::shared_ptr<cms::BytesMessage> BytesMessagePtr;

	class MessageData;
	typedef boost::shared_ptr<MessageData> MessageDataPtr;

    class CmsConnectionPool;
    typedef boost::shared_ptr<CmsConnectionPool> CmsConnectionPoolPtr;

	typedef std::vector<MessageDataPtr> MessageDataSeq;

}
}
#endif
