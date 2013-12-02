#ifndef XCE_ADAPTER_USERBASICTRIPODADAPTER_H_
#define XCE_ADAPTER_USERBASICTRIPODADAPTER_H_

#include "UserBase.pb.h"
#include "UserBase.h"
#include "Singleton.h"
#include "tripod2/cc/core/multibiz_tripod_client.h"

namespace xce {
namespace adapter {
namespace userbasic {

const static std::string kNameSpace = "xce_user";
const static std::string kBizUserPassport = "user_passport";
const static std::string kBizUserName = "user_name";
const static std::string kBizUserState = "user_state";
const static std::string kBizUserBorn = "user_born";
const static std::string kBizUserStage = "user_stage";
const static std::string kBizUserDoing = "user_doing";
const static std::string kBizUserUrl = "user_url";

using xce::tripod::MultiBizTripodClient;
using namespace xce::tripoddata::userbase;
using xce::userbase::UserBasicDataPtr;
using xce::userbase::UserBasicData;

class UserBasicTripodAdapter : public MyUtil::Singleton<UserBasicTripodAdapter> {
public:
  UserBasicDataPtr GetBasic(int user_id);
  virtual ~UserBasicTripodAdapter();

private:
  friend class MyUtil::Singleton<UserBasicTripodAdapter>;
  UserBasicTripodAdapter();
  xce::tripod::StringListType bizs_;
  boost::shared_ptr<MultiBizTripodClient> multi_biz_client_;
  const static int expiration;

};

class UserBasicUtil {
public:
  static void parse(const PbUserPassport &passport, UserBasicDataPtr &result);
  static void parse(const PbUserName &name, UserBasicDataPtr &result);
  static void parse(const PbUserState &state, UserBasicDataPtr &result);
  static void parse(const PbUserBorn &born, UserBasicDataPtr &result);
  static void parse(const PbUserStage &staeg, UserBasicDataPtr &result);
  static void parse(const PbUserDoing &doing, UserBasicDataPtr &result);
  static void parse(const PbUserUrl &url, UserBasicDataPtr &result);
};

}}} //namespace xce::adapter::userbase

#endif
