#include "AdMonitorI.h"
#include "AdStruct.h"


namespace xce {
namespace ad {
using namespace std;

::std::string AdMonitorI::MemberStatus(::Ice::Long id, const ::Ice::Current&) {
    string result;
    return result;
}

::std::string AdMonitorI::CampaignStatus(::Ice::Long id, const ::Ice::Current&) {
    string result;
    return result;
}

::std::string AdMonitorI::GroupStatus(::Ice::Long id, const ::Ice::Current&) {
    string result;
    return result;
}

::std::string AdMonitorI::CompeteRef(::Ice::Long campaign_id, const ::Ice::Current&) {
  string result;
  ostringstream result_oss;
  if (campaign_id) {
    vector<Ice::Long> competes = CompeteRef::instance().Get(campaign_id);
    for(vector<Ice::Long>::iterator it = competes.begin(); it != competes.end(); ++it) {
      result_oss << "campaign_id:\t" << (*it) << endl;
    }
  } else {
    result_oss << "No CompeteRef of this campaign id: " << campaign_id;
  }
  result = result_oss.str();
  MCE_DEBUG("[MONITOR_DEBUG] : Result = " << result);
  return result;
}

::std::string AdMonitorI::RotationCache(::Ice::Long id, const ::Ice::Current&) {
    string result;
    return result;
}

}
}
