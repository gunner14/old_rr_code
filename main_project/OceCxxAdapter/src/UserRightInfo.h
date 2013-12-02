#ifndef XCE_ADAPTER_USERRIGHTINFO_
#define XCE_ADAPTER_USERRIGHTINFO_

#include "UserBase.h"
#include "Markable.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace userright {

using namespace xce::userbase;
using namespace MyUtil;

class UserRightInfo;
typedef IceUtil::Handle<UserRightInfo> UserRightInfoPtr;

class UserRightInfo : virtual public Ice::Object, public MyUtil::Markable {
public:
	UserRightInfoPtr parse(const UserRightDataPtr& ptr) { id_ = ptr->uID; auth_ = ptr->auth; rights_ = ptr->rights; return this; }
  std::string to_string() {
    std::ostringstream oss;
    oss << "UserStageInfo:[";
    oss << "id:" << id_;
    oss << ",auth:" << auth_;
    oss << ",rights:" << rights_;
    oss << "]";
    return oss.str();
  }
  int id() const { return id_; }
	int auth() const { return auth_; }
	int rights() const { return rights_; }
	void set_id(int id) { id_ = id; mark(xce::userbase::CURID, boost::lexical_cast<std::string>(id)); }
	void set_auth(int auth) { auth_ = auth; mark(xce::userbase::CURAUTH, boost::lexical_cast<std::string>(auth)); }
	void set_rights(int rights) { rights_ = rights; mark(xce::userbase::CURCOMMONRIGHTS, boost::lexical_cast<std::string>(rights)); }
private:
	int id_;
	int auth_;
	int rights_;
};

}}} //namespace xce::adapter::userright
#endif
