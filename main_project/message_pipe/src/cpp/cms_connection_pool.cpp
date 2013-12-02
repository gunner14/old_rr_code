#include <cms_connection_pool.h>
#include <decaf/util/concurrent/Concurrent.h>

namespace xce {
namespace messagepipe {

using namespace decaf::util::concurrent;
using namespace activemq::core;

CmsConnectionPool::CmsConnectionPool(const std::string& uri, int max_connection) : uri_(uri){ 
    MCE_DEBUG("CmsConnectionPool::CmsConnectionPool uri "<< uri);
    max_connection_ = max_connection;
    //uri是如下形式，定义了是否异步发送还有超时时间，一定要设置超时时间，否则可能出现broker挂掉，还依然等待消息的情况
    //new ActiveMQConnectionFactory( "tcp://localhost:61616?connection.alwaysSyncSend=true&connection.sendTimeout=1000" );
}
CmsConnectionPool::~CmsConnectionPool(){
    for( std::list<CmsTemplate*>::iterator iter = avaliable_.begin(); iter != avaliable_.end(); ++iter ) {
        delete *iter;
    }
    
    for( std::list<ActiveMQConnectionFactory*>::iterator iter = factories_.begin(); iter != factories_.end(); ++iter ) {
        delete *iter;
    }
    factories_.clear();
}
CmsTemplate* CmsConnectionPool::takeConnection(){

    {
        IceUtil::Mutex::Lock lock(mutex_);
        CmsTemplate* cms_template= NULL;
        if( avaliable_.empty() && max_connection_ >0 ) {
            ActiveMQConnectionFactory* factory = new ActiveMQConnectionFactory(uri_);
            cms_template = new CmsTemplate( factory );

            //may set to client_acknowledge
            cms_template->setSessionAcknowledgeMode(cms::Session::CLIENT_ACKNOWLEDGE);
            //cms_template->setReceiveTimeout(1000);
            cms_template->setReceiveTimeout(-1);

            //如果设置为true每次调用都会fsync?总之非常慢，而且没有必要
            //broker 需要把fsync改为false
            cms_template->setDeliveryPersistent(true);
            //如果不设置为true上面那条的设置也会无效
            cms_template->setExplicitQosEnabled(true);
            --max_connection_;
            factories_.push_back(factory);
        } else if(max_connection_ > 0) {
            cms_template = avaliable_.front();
            avaliable_.pop_front();
            --max_connection_;
        }
        return cms_template;
    }
    return NULL;
}

void CmsConnectionPool::returnConnection( CmsTemplate* session ) {

//目前没有处理连接不可用的情况，放入连接池中，下次使用的时候CmpTemplate对象会自动进行重连
    {
        IceUtil::Mutex::Lock lock(mutex_);
        avaliable_.push_back( session );
        ++max_connection_;
    }
}

}
}
