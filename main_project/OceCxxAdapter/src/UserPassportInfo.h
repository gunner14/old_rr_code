#ifndef XCE_ADAPTER_USERPASSPORTINFO_
#define XCE_ADAPTER_USERPASSPORTINFO_

#include "UserBase.h"
#include "Markable.h"
#include <iostream>
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace userpassport {

using namespace xce::userbase;
using namespace MyUtil;

class UserPassportInfo;
typedef IceUtil::Handle<UserPassportInfo> UserPassportInfoPtr;

class UserPassportInfo : virtual public Ice::Object, public MyUtil::Markable {
public:
  UserPassportInfoPtr parse(const UserPassportDataPtr& data) {
    id_ = data->uID;
    status_ = data->status;
    safestatus_ = data->safeStatus;
    account_ = data->account;
    domain_ = data->domain;
    passwordmd5_ = data->passwordmd5;
    return this;
  }
  std::string to_string() {
    std::ostringstream oss;
    oss << "UserPassportInfo:[";
    oss << "id:" << id_;
    oss << ",status:" << status_;
    oss << ",safestatus:" << safestatus_;
    oss << ",account:" << account_;
    oss << ",domain:" << domain_;
    oss << ",passwordmd5:" << passwordmd5_;
    oss << "]";
    return oss.str();
  }
  int id() const { return id_; }
  int status() const { return status_; }
  int safestatus() const { return safestatus_; }
  std::string account() const { return account_; }
  std::string domain() const { return domain_; }
  std::string passwordmd5() const { return passwordmd5_; }
  void set_id(int id) { id_ = id; mark(xce::userbase::CUPID, boost::lexical_cast<std::string>(id)); }
  void set_status(int status) { status_ = status; mark(xce::userbase::CUPSTATUS, boost::lexical_cast<std::string>(status_)); }
  void set_safestatus(int safestatus) { safestatus_ = safestatus; mark(xce::userbase::CUPSAFESTATUS, boost::lexical_cast<std::string>(safestatus_)); }
  void set_account(const std::string& account) { account_ = account; mark(xce::userbase::CUPACCOUNT, account_); }
  void set_domain(const std::string& domain) { domain_ = domain; mark(xce::userbase::CUPDOMAIN, domain_); }
  void set_passwordmd5(const std::string& passwordmd5) { passwordmd5_ = passwordmd5; mark(xce::userbase::CUPPASSWORDMD5, passwordmd5_); }
private:
  int id_;
  int status_;
  int safestatus_;
  std::string account_;
  std::string domain_;
  std::string passwordmd5_;
};

}}} //namespace xce::adapter::userpassport
#endif
