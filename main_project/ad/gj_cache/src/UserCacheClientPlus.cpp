#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <protocol/TCompactProtocol.h>
#include "UserCacheClientPlus.h"
#include "IceLogger.h"
#include <time.h>

namespace xce {
namespace ad {

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

UserCacheClientPlus::UserCacheClientPlus(){
   port_ = 9632;
   server_list[0] = "s.jie.d.xiaonei.com";
   server_list[1] = "s2.jie.d.xiaonei.com";
 // server_list[0] = "10.2.116.54";
 // server_list[1] = "10.2.116.54";
 //  server_list[0] = "10.3.24.73";
 //   server_list[1] = "10.3.24.73";
 //   server_list[0] = "10.7.18.50";
 //   server_list[1] = "10.7.18.50";
  for(int i =0; i < CONN_SIZE; ++i) {
    int index = i %2;
    string server_name = server_list[index];
    boost::shared_ptr<TSocket> socket(new TSocket(server_name, port_));
    socket->setConnTimeout(300);
    socket->setSendTimeout(300);
    socket->setRecvTimeout(300);
    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TCompactProtocol(transport));
    UserCacheServiceClient *client = new UserCacheServiceClient(protocol);
    transport->open();
    conn_pool.push_back(client);
    //conn_pool.push_back(make_pair(client,true));
  } 
  using namespace MyUtil;
  TaskManager::instance().execute(new FailLoadTask());
}

UserCacheServiceClient* UserCacheClientPlus::GetClient(){
  IceUtil::RWRecMutex::WLock lock(mutex_);
  if(conn_pool.empty()){
    return NULL;
  }else{
    UserCacheServiceClient *client = conn_pool.front();
    conn_pool.pop_front();
    return client;
 }  
}

void UserCacheClientPlus::ReleaseClient(UserCacheServiceClient * client){
    IceUtil::RWRecMutex::WLock lock(mutex_);
    conn_pool.push_back(client);
}

UserCacheClientPlus::~UserCacheClientPlus(){
    for(list<UserCacheServiceClient*>::iterator it = conn_pool.begin(); it != conn_pool.end(); ++it){
        UserCacheServiceClient *conn = *it; 
        boost::shared_ptr< ::apache::thrift::protocol::TProtocol> p = conn->getInputProtocol();
        boost::shared_ptr<TTransport> t = p->getInputTransport();   
        t->close();
        delete conn;

    }
}

void UserCacheClientPlus::GetFollower(vector<int64_t> & follower, const int64_t uid, const int limit) {
    MCE_DEBUG("UserCacheClient::GetFollower() --> uid = " << uid);
    UserCacheServiceClient *client = GetClient();
    try{
        if(client == NULL){
            MCE_WARN("no available client");
            return;
        }else{
            client->getFollower(follower,uid,limit);
            ReleaseClient(client);
        }

    } catch (...) {
        //MCE_WARN("UserCacheClient::GetFollower --> unknown error");
        IceUtil::RWRecMutex::WLock lock(mutex_);
        conn_invalid_pool.push_back(client);
    }
}

void UserCacheClientPlus::GetTopicFollower(vector<int64_t> & follower, const int64_t topic_id, const int limit) {
  MCE_DEBUG("UserCacheClient::GetTopicFollower() --> topic_id = " << topic_id);
  UserCacheServiceClient *client = GetClient();
  try{
    
        if(client == NULL){
            MCE_WARN("no available client");
            return;
        }else{
            client->getTopicFollower(follower,topic_id,limit);
            ReleaseClient(client);
        }

  } catch (...) {
    //MCE_WARN("UserCacheClient::GetFollower --> unknown error");
    IceUtil::RWRecMutex::WLock lock(mutex_);
    conn_invalid_pool.push_back(client);
  }
}

void UserCacheClientPlus::GetTopicFollowerByIndex(vector<int64_t> & follower, const int64_t topic_id, const int64_t index, const int limit) {
    MCE_DEBUG("UserCacheClient::GetTopicFollowerByIndex() --> topic_id = " << topic_id << "; index = " << index );
    UserCacheServiceClient *client = GetClient();
    try{

        if(client == NULL){
            MCE_WARN("no available client");
            return;
        }else{
            client->getTopicFollowerByIndex(follower,topic_id,index,limit);
            ReleaseClient(client);
        }

    } catch (...) {
        //MCE_WARN("UserCacheClient::GetTopicFollowerByIndex --> unknown error");
        IceUtil::RWRecMutex::WLock lock(mutex_);
        conn_invalid_pool.push_back(client);
    }
}

void UserCacheClientPlus::GetFans(vector<int64_t> & fans, const int64_t uid, const int limit) {
  MCE_DEBUG("UserCacheClient::GetFans() --> uid = " << uid);
  UserCacheServiceClient *client = GetClient();
  try{
    
        if(client == NULL){
            MCE_WARN("no available client");
            return;
        }else{
            client->getFans(fans,uid,limit);
            ReleaseClient(client);
        }

  } catch (...) {
    //MCE_WARN("UserCacheClient::GetFans --> unknown error");
    IceUtil::RWRecMutex::WLock lock(mutex_);
   conn_invalid_pool.push_back(client);
  }
}


void UserCacheClientPlus::GetFansByIndex(vector<int64_t> & fans, const int64_t uid, const int64_t index, const int limit) {
  MCE_DEBUG("UserCacheClient::GetFansByIndex() --> uid = " << uid << "; index = " << index );
  UserCacheServiceClient *client = GetClient();
  try{
        if(client == NULL){
            MCE_WARN("no available client");
            return;
        }else{
            client->getFansByIndex(fans,uid,index,limit);
            ReleaseClient(client);
        }

      
    
  } catch (...) {
    //MCE_WARN("UserCacheClient::GetFansByIndex --> unknown error");
    IceUtil::RWRecMutex::WLock lock(mutex_);
    conn_invalid_pool.push_back(client);
  }
}

void UserCacheClientPlus::GetAlbumFans(vector<int64_t> & fans, const int64_t album_id, const int64_t index, const int limit) {
  MCE_DEBUG("UserCacheClient::GetAlbumFans() --> albumId = " << album_id);
  UserCacheServiceClient *client = GetClient();
  try{
        if(client == NULL){
            MCE_WARN("no available client");
            return;
        }else{
            client->getAlbumFans(fans,album_id,index,limit);
            ReleaseClient(client);
        }

  } catch (...) {
    //MCE_WARN("UserCacheClient::GetAlbumFans --> unknown error");
    IceUtil::RWRecMutex::WLock lock(mutex_);
    conn_invalid_pool.push_back(client);
  }
}

UserCacheServiceClient* UserCacheClientPlus::NewClientInternal(const string &name, int port){

    boost::shared_ptr<TSocket> socket(new TSocket(name, port));
    socket->setConnTimeout(300);
    socket->setSendTimeout(300);
    socket->setRecvTimeout(300);

    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TCompactProtocol(transport));
    UserCacheServiceClient *client = new UserCacheServiceClient(protocol);

    try{
        transport->open();
        //conn_pool.push_back(client);
        return client;
    }catch(...){
        MCE_WARN("UserCacheClientPlus connect to " << name << ":" << port << " failed");
        socket->close();
        delete client;
        return NULL;
    }
    
    return NULL;

}


UserCacheServiceClient* UserCacheClientPlus::NewClient(){

    int idx = random() % 2;

    string server_name = server_list[idx];

    UserCacheServiceClient *client = NewClientInternal(server_name, port_);
    if(client != NULL){
        return client;
    }

    server_name = server_list[1-idx];
    client = NewClientInternal(server_name, port_); 
    return client;


}

void UserCacheClientPlus::CheckConnValid() {


    {
        IceUtil::RWRecMutex::WLock lock(mutex_);
        MCE_INFO("UserCacheClientPlus conn_pool size:" << conn_pool.size() << ", conn_invalid_pool size:" 
                    << conn_invalid_pool.size());
        if(conn_invalid_pool.empty()){
            return;
        }
    }

    while(true){

        UserCacheServiceClient *client = NewClient();
        if(client == NULL){
            return;
        }else{
            IceUtil::RWRecMutex::WLock lock(mutex_);
            UserCacheServiceClient *invalid_client = conn_invalid_pool.front();
            boost::shared_ptr< ::apache::thrift::protocol::TProtocol> p = invalid_client->getInputProtocol();
            boost::shared_ptr<TTransport> t = p->getInputTransport();
            t->close();
            conn_invalid_pool.pop_front();
            delete invalid_client;
            conn_pool.push_back(client);
            if(conn_invalid_pool.empty()){
                return;
            }
        }


    }

}

void FailLoadTask::handle() {
  try {
    while(true) {
      UserCacheClientPlus::instance().CheckConnValid();
      usleep(6 * 1000 * 1000);
    }
  } catch (std::exception& e) {
    MCE_WARN("FailLoadTask--> handle ERR  line:" << __LINE__ << " err:" << e.what());
  } catch (...) {
    MCE_WARN("FailLoadTask--> handle ERR  line:" << __LINE__);
  }
}

}
}