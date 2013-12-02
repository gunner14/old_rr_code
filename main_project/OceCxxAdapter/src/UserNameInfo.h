#ifndef XCE_ADAPTER_USERNAMEINFO_
#define XCE_ADAPTER_USERNAMEINFO_

#include "UserBase.h"
#include "Markable.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace username {

using namespace xce::userbase;
using namespace MyUtil;

class UserNameInfo;
typedef IceUtil::Handle<UserNameInfo> UserNameInfoPtr;

class UserNameInfo : virtual public Ice::Object, public MyUtil::Markable {
public:
  UserNameInfoPtr parse(const UserNameDataPtr& data) {
    id_ = data->uID;
    name_ = data->name;
    nickname_ = data->nickname;
    return this;
  }
  std::string to_string() {
    std::ostringstream oss;
    oss << "UserNameInfo:[";
    oss << "id:" << id_;
    oss << ",name:" << name_;
    oss << ",nickname:" << nickname_;
    oss << "]";
    return oss.str();
  }
  std::string name() const {return name_;}
  std::string nickname() const {return nickname_;}
  int id() const {return id_;}
  void set_id(int id) {id_ = id;mark(xce::userbase::CUNID, boost::lexical_cast<std::string>(id));}
  void set_name(const std::string& name) {name_ = name;mark(xce::userbase::CUNNAME, name);}
  void set_nickname(const std::string& nickname) {nickname_ = nickname;mark(xce::userbase::CUNNICKNAME, nickname);}
private:
  int id_;
  std::string name_;
  std::string nickname_;
};

}}} //namespace xce::adapter::username
#endif
