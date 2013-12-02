#ifndef XCE_ADAPTER_USERSTAGEINFO_
#define XCE_ADAPTER_USERSTAGEINFO_

#include "UserBase.h"
#include "Markable.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace userstage {

using namespace xce::userbase;
using namespace MyUtil;

class UserStageInfo;
typedef IceUtil::Handle<UserStageInfo> UserStageInfoPtr;

class UserStageInfo : virtual public Ice::Object, public MyUtil::Markable {
public:
  UserStageInfoPtr parse(const UserStageDataPtr& data) {
    id_ = data->id;
    univ_ = data->univ;
    stage_ = data->stage;
    return this;
  }
  std::string to_string() {
    std::ostringstream oss;
    oss << "UserStageInfo:[";
    oss << "id:" << id_;
    oss << ",univ:" << univ_;
    oss << ",stage:" << stage_;
    oss << "]";
    return oss.str();
  }
  int id() const { return id_; }
  int univ() const { return univ_; }
  int stage() const { return stage_; }
  void set_univ(int univ) { univ_ = univ; mark(xce::userbase::CUSGUNIV, boost::lexical_cast<std::string>(univ));}
  void set_id(int id) { id_ = id; mark(xce::userbase::CUSGID, boost::lexical_cast<std::string>(id));}
  void set_stage(int stage) { stage_ = stage; mark(xce::userbase::CUSGSTAGE, boost::lexical_cast<std::string>(stage));}
private:
  int id_;
  int univ_;
  int stage_;
};

}}} //namespace xce::adapter::userstage
#endif
