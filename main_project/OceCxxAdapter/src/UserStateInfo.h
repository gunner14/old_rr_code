#ifndef XCE_ADAPTER_USERSTATEINFO_
#define XCE_ADAPTER_USERSTATEINFO_

#include "UserBase.h"
#include "Markable.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace xce {
namespace adapter {
namespace userstate {

using namespace xce::userbase;
using namespace MyUtil;

class UserStateInfo;
typedef IceUtil::Handle<UserStateInfo> UserStateInfoPtr;

class UserStateInfo : virtual public Ice::Object, public MyUtil::Markable {
public:
  UserStateInfo() : id_(0), state_(0), star_(0), level_(0) {}
  std::string to_string() {
    std::ostringstream oss;
    oss << "UserStageInfo:[";
    oss << "id:" << id_;
    oss << ",state:" << state_;
    oss << ",level:" << level_;
    oss << ",star:" << star_;
    oss << "]";
    return oss.str();
  }
  void parse(const UserStateDataNPtr& data) {
    id_ = data->id;
    state_ = data->state;
    star_ = data->star;
    level_ = data->level;
  }
  //为UserStateReaderAdapter编译通过而存在，实际没有用到
  //UserStateReaderAdapter弃用后此方法可废弃
  void parse(const UserStateDataPtr& data) {
    id_ = data->id;
    state_ = data->state;
    star_ = data->star;
    level_ = data->level;
  }
  long state() const {return state_;}
  int star() const {return star_;}
  int level() const {return level_;}
  //请千万不要使用这个接口，如果对应的位没有开放，请知会中间层:renren.xce@renren-inc.com
  void set_state(long state) {state_ = state;mark("STATE", boost::lexical_cast<std::string>(state));}
  void set_level(int level) {level_ = level;mark("LEVEL", boost::lexical_cast<std::string>(level));}
  void set_star(int star) {star_ = star;mark("STAR", boost::lexical_cast<std::string>(star));}
  int id() const {return id_;}
  void set_id(int id) {id_ = id; mark("ID", boost::lexical_cast<std::string>(id));}
  bool isSelected() const { return 0 != (state_ & (1L << CStateSELECTED));}
  bool isVipMember() const { return 0 != (state_ & (1L << CStateVIPMEMBER));}
  bool isMSNIMME() const { return 0 != (state_ & (1L << CStateMSNIMME));}
  bool isMobileBind() const { return 0 != (state_ & (1L << CStateMOBILEBIND));}
  bool isEverVipMember() const { return 0 != (state_ & (1L << CStateEVERVIPMEMBER));}
  bool isGuideDone() const { return 0 != (state_ & (1L << CStateGUIDEDONE));}
  bool isSocialGraphGood() const { return 0 != (state_ & (1L << CStateSOCIALGRAPHGOOD));}
  bool isKeepUse() const { return 0 != (state_ & (1L << CStateKEEPUSE));}
  bool isCharity() const { return 0 != (state_ & (1L << CStateCHARITY));}
  bool isCharityInvited() const { return 0 != (state_ & (1L << CStateCHARITYINVITED));}
  bool isDoubleAccount() const { return 0 != (state_ & (1L << CStateDOUBLEACCOUNT));}
  bool isHideAds() const { return 0 != (state_ & (1L << CStateHIDEADS));}
  bool isVest() const { return 0 != (state_ & (1L << CStateVEST));}
  bool isDisneyVIP() const {return 0 != (state_ & (1L << CStateDisneyVIP));}
  bool isGameManager() const { return 0 != (state_ & (1L << CStateGAMEMANAGER));}
  bool isSamePassword() const { return 0 != (state_ & (1L << CStateSAMEPASSWORD));}
  bool isCouplesSpace() const { return 0 != (state_ & (1L << CStateCOUPLESSPACE));}
  bool isPageManager() const { return 0 != (state_ & (1L << CStatePAGEMANAGER));}
  bool isApplySpammer() const { return 0 != (state_ & (1L << CStateAPPLYSPAMMER));}
  bool isInWhiteList() const { return 0 != (state_ & (1L << CStateINWHITELIST));}
private:
  int id_;
  long state_;
  int star_;
  int level_;
};


}}} //namespace xce::adapter::userstate
#endif
