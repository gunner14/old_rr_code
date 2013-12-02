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
  for(int i =0; i < CONN_SIZE; ++i) {
    int index = i %2;
    string server_name = server_list[index];
    boost::shared_ptr<TSocket> socket(new TSocket(server_name, port_));
    socket->setConnTimeout(200);
    socket->setSendTimeout(200);
    socket->setRecvTimeout(200);
    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TCompactProtocol(transport));
    UserCacheServiceClient client(protocol);
    transport->open();
    conn_pool.push_back(make_pair(client,true));
  } 
  using namespace MyUtil;
  TaskManager::instance().execute(new FailLoadTask());
}

UserCacheClientPlus::~UserCacheClientPlus(){
  for(int i =0; i < CONN_SIZE; ++i) {
//    cout << "UserCacheClient::destroy =i " << i << endl;
      UserCacheServiceClient conn = conn_pool[i].first; 
      boost::shared_ptr< ::apache::thrift::protocol::TProtocol> p = conn.getInputProtocol();
      boost::shared_ptr<TTransport> t = p->getInputTransport();   
      t->close();
  } 
}
void UserCacheClientPlus::GetFollower(vector<int64_t> & follower, const int64_t uid, const int limit) {
  MCE_DEBUG("UserCacheClient::GetFollower() --> uid = " << uid);
  int random = 0;
  try{
    srand(time(NULL));
    random = rand() % CONN_SIZE;
    IceUtil::RWRecMutex::RLock lock(mutex_);
    pair<UserCacheServiceClient,bool> conn = conn_pool[random]; 
    if (!conn.second) {
      random = (random + 1)%CONN_SIZE;
      conn = conn_pool[random]; 
    } 
    
//    cout << "UserCacheClient::GetFollower() --> uid = " << uid <<",rand:" << random << endl;
    if (conn.second) {
      conn.first.getFollower(follower,uid,limit);
    }
    
  } catch (...) {
    MCE_WARN("UserCacheClient::GetFollower --> unknown error, random:"<<random);
//    cout << "UserCacheClient::GetFollower --> unknown error, random:"<<random;
    IceUtil::RWRecMutex::WLock lock(mutex_);
    conn_pool[random].second = false;
  }
}
void UserCacheClientPlus::GetFans(vector<int64_t> & fans, const int64_t uid, const int limit) {
  MCE_DEBUG("UserCacheClient::GetFans() --> uid = " << uid);
  int random = 0;
  try{
    srand(time(NULL));
    random = rand() % CONN_SIZE;
    IceUtil::RWRecMutex::RLock lock(mutex_);
    pair<UserCacheServiceClient,bool> conn = conn_pool[random]; 
    if (!conn.second) {
      random = (random + 1)%CONN_SIZE;
      conn = conn_pool[random]; 
    } 
    
    if (conn.second) {
      conn.first.getFans(fans,uid,limit);
    }
    
  } catch (...) {
    MCE_WARN("UserCacheClient::GetFans --> unknown error, random:"<<random);
    IceUtil::RWRecMutex::WLock lock(mutex_);
    conn_pool[random].second = false;
  }
}

void UserCacheClientPlus::CheckConnValid() {
    IceUtil::RWRecMutex::WLock lock(mutex_);
    for(int i = 0; i < CONN_SIZE; ++i) {
      pair<UserCacheServiceClient,bool> conn = conn_pool[i]; 
      try{
      if(!conn.second){
//关闭链接
        boost::shared_ptr< ::apache::thrift::protocol::TProtocol> p = conn.first.getInputProtocol();
        boost::shared_ptr<TTransport> t = p->getInputTransport();   
        t->close();
//创建新的链接
        int index = i % 2;
        string server_name = server_list[index];
        boost::shared_ptr<TSocket> socket(new TSocket(server_name, port_));
        socket->setConnTimeout(200);
        socket->setSendTimeout(200);
        socket->setRecvTimeout(200);
        boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
        boost::shared_ptr<TProtocol> protocol(new TCompactProtocol(transport));
        UserCacheServiceClient client(protocol);
        transport->open();
        conn_pool[i] = make_pair(client,true);
      }
     }catch(...) {
       MCE_WARN("UserCacheClient::CheckConnValid --> create new conn  error, random:"<<i);
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
