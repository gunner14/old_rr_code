#ifndef XCE_ADAPTER_USERBASETRIPODADAPTER_H_
#define XCE_ADAPTER_USERBASETRIPODADAPTER_H_

#include "UserBaseInfo.h"
#include "UserBase.pb.h"
#include "Singleton.h"
#include "tripod2/cc/core/multibiz_tripod_client.h"

namespace xce {
namespace adapter {
namespace userbase {

const static std::string kNameSpace = "xce_user";
const static std::string kBizUserPassport = "user_passport";
const static std::string kBizUserName = "user_name";
const static std::string kBizUserRight = "user_right";
const static std::string kBizUserState = "user_state";
const static std::string kBizUserLoginCount = "user_logincount";
const static std::string kBizUserBorn = "user_born";
const static std::string kBizUserStage = "user_stage";
const static std::string kBizUserDoing = "user_doing";
const static std::string kBizUserUrl = "user_url";

using xce::tripod::MultiBizTripodClient;
using namespace xce::tripoddata::userbase;

class UserBaseTripodAdapter : public MyUtil::Singleton<UserBaseTripodAdapter> {
public:
  UserBaseCommonViewPtr GetCommon(int user_id);
  UserBaseFullViewPtr GetFull(int user_id);
  bool LoadCommon(int user_id);
  bool LoadFull(int user_id);
  virtual ~UserBaseTripodAdapter();

private:
  friend class MyUtil::Singleton<UserBaseTripodAdapter>;
  UserBaseTripodAdapter();
  xce::tripod::StringListType bizs_common_;
  xce::tripod::StringListType bizs_full_;
  boost::shared_ptr<MultiBizTripodClient> multi_biz_client_;
  const static int expiration;
};

class UserBaseUtil {
public:
  static void parse(const PbUserPassport &passport, UserBaseCommonViewPtr &result);
  static void parse(const PbUserName &name, UserBaseCommonViewPtr &result);
  static void parse(const PbUserRight &right, UserBaseCommonViewPtr &result);
  static void parse(const PbUserState &state, UserBaseCommonViewPtr &result);
  static void parse(const PbUserLoginCount &logincount, UserBaseCommonViewPtr &result);
  static void parse(const int logincount, UserBaseCommonViewPtr &result);
  static void parse(const PbUserBorn &born, UserBaseCommonViewPtr &result);
  static void parse(const PbUserStage &staeg, UserBaseCommonViewPtr &result);
  static void parse(const PbUserUrl &url, UserBaseCommonViewPtr &result);

  static void parse(const PbUserPassport &passport, UserBaseFullViewPtr &result);
  static void parse(const PbUserName &name, UserBaseFullViewPtr &result);
  static void parse(const PbUserRight &right, UserBaseFullViewPtr &result);
  static void parse(const PbUserState &state, UserBaseFullViewPtr &result);
  static void parse(const PbUserLoginCount &logincount, UserBaseFullViewPtr &result);
  static void parse(const int logincount, UserBaseFullViewPtr &result);
  static void parse(const PbUserBorn &born, UserBaseFullViewPtr &result);
  static void parse(const PbUserStage &staeg, UserBaseFullViewPtr &result);
  static void parse(const PbUserDoing &doing, UserBaseFullViewPtr &result);
  static void parse(const PbUserUrl &url, UserBaseFullViewPtr &result);
};

}}} //namespace xce::adapter::userbase

#endif
