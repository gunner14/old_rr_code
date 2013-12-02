#ifndef __INDEX_SERVICE_MONITOR_H__
#define __INDEX_SERVICE_MONITOR_H__

#include "IndexService.h"
#include "Singleton.h"
#include <string>

using namespace xce::ad;
namespace xce {
namespace ad {


class IndexMonitorI: public IndexMonitor, public MyUtil::Singleton<IndexMonitorI> {
  public:
    string GetAdPoolInfo(const int type, const Ice::Current&);
    string GetAdZonePrice(Ice::Long groupId, const Ice::Current&);//广告组在广告位上的标价
    string GetZoneAdNum(const Ice::Current&);
    string GetTriggerInfo(const Ice::Current&);
    string GetGameAdNum(const Ice::Current&);
    string GetGameMemNum(const Ice::Current&);
    string GetZoneIndex(Ice::Long zoneId, const Ice::Current&);//广告位上对应的索引
    string GetClosedAd(const Ice::Current&);
};

}
}
#endif
