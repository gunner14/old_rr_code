#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TCompactProtocol.h>
#include "IceLogger.h"
#include "AntiFraudClient.h"

namespace xce {
namespace ad {

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace com::renren::ad::engine;
/**
* 本地调用方法,通过反作弊client调用远端服务判断点击情况
* return 0:真实点击，非0:作弊点击
*
**/
int AntiFraudClient::check(const ClickAction& clickAction) {
  MCE_DEBUG("AntiFraudClient::check() --> enter");
  int res = 0;
  try{
   // boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
    boost::shared_ptr<TSocket> socket(new TSocket(host_, port_));
    socket->setConnTimeout(50);
    socket->setSendTimeout(50);
    socket->setRecvTimeout(100);
    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TCompactProtocol(transport));
   //FIXME 看是否可以转成连接client池
    AntiFraudServiceClient client(protocol);
    transport->open();
    res = client.check(clickAction);
    transport->close();
  } catch (std::exception e) {
    res = 0;
    MCE_WARN("AntiFraudClient::check() --> error: " << e.what());
  } catch (...) {
    res = 0;
    MCE_WARN("AntiFraudClient::check() --> error: unknown error!" );
  }
  
  return res;
  MCE_DEBUG("AntiFraudClient::check() --> out");
}

}
}
