#ifndef XCE_ADAPTER_USERDOINGINFO_
#define XCE_ADAPTER_USERDOINGINFO_

#include "UserBase.h"
#include "Markable.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace userdoing {

using namespace xce::userbase;
using namespace MyUtil;

class UserDoingInfo;
typedef IceUtil::Handle<UserDoingInfo> UserDoingInfoPtr;

class UserDoingInfo : virtual public Ice::Object, public MyUtil::Markable {
public:
	UserDoingInfoPtr parse(const UserDoingDataPtr& data) { id_ = data->id; doing_ = data->doing; doingTime_ = data->doingTime; return this; }
  std::string to_string() {
    std::ostringstream oss;
    oss << "UserDoingInfo:[";
    oss << "id:" << id_;
    oss << ",doing:" << doing_;
    oss << ",doingTime:" << doingTime_;
    oss << "]";
    return oss.str();
  }
	int id() const { return id_; }
  std::string doing() const { return doing_; }
	int dointTime() const { return doingTime_; }
	void set_id(int id) { id_ = id; mark(xce::userbase::CUDID, boost::lexical_cast<std::string>(id)); }
	void set_doing(const std::string& doing) { doing_ = doing; mark(xce::userbase::CUDDOING, doing); }
	void set_dointTime(int doingTime) { doingTime_ = doingTime; mark(xce::userbase::CUDDOINGTIME, boost::lexical_cast<std::string>(doingTime)); }

private:
	int id_;
	int doingTime_;
  std::string doing_;
};

}}} //namespace xce::adapter::userdoing
#endif
