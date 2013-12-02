#ifndef XCE_TRIPOD_ACCOUNT_CONNECT_PRODUCER_H_
#define XCE_TRIPOD_ACCOUNT_CONNECT_PRODUCER_H_

#include "boost/lexical_cast.hpp"
#include "tripod2/cc/core/producer.h"
#include "QueryRunner.h"

namespace xce {
namespace tripod {

class AccountConnectProducer : public Producer {
 public:
  AccountConnectProducer() {}
  virtual ~AccountConnectProducer() {}
  virtual bool Create(const std::string& key, ByteSeq* value, bool use_master);
};

class AccountConnectHandler : public com::xiaonei::xce::ResultHandler {
public:
  AccountConnectHandler (int &id);
  virtual bool handle(mysqlpp::Row& row) const;
private:
  int &id_;
};


}} //xce::tirpod

#endif
