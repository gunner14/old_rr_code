#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <protocol/TCompactProtocol.h>
#include "UserCacheClient.h"
#include "IceLogger.h"

namespace xce {
namespace ad {

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

void UserCacheClient::GetFollower(vector<int64_t> & follower, const int64_t uid, const int limit) {
  MCE_DEBUG("UserCacheClient::GetFollower() --> uid = " << uid);
  try{
    boost::shared_ptr<TSocket> socket(new TSocket(server_name_, port_));
    socket->setConnTimeout(200);
    socket->setSendTimeout(200);
    socket->setRecvTimeout(200);
    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TCompactProtocol(transport));
    UserCacheServiceClient client(protocol);
    transport->open();
    client.getFollower(follower,uid,limit);
    transport->close();
  } catch (...) {
    MCE_DEBUG("UserCacheClient::GetFollower --> unknown error");
  }
}

}
}
