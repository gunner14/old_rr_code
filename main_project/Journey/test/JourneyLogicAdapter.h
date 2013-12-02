#ifndef JOURNEY_LOGIC_ADAPTER_H_ 
#define JOURNEY_LOGIC_ADAPTER_H_ 

#include "UtilCxx/src/Singleton.h"
#include "util/cpp/TimeCost.h"
#include "tripod2/cc/core/tripod_client.h"
#include "IceExt/src/TempMutex.h"

namespace xce {
namespace tripod {

const std::string kJourneyNameSpaceId = "ljp_namespace";
const std::string kJourneyCountBusinessId = "journey_count";
const std::string kJourneyListBusinessId = "journey_list";
const int kJourneyExpire = 100000000;

class JourneyLogicAdapter : virtual public MyUtil::Singleton<JourneyLogicAdapter> {
  public:
    JourneyLogicAdapter() {
      count_client_ = new TripodClient(kJourneyNameSpaceId, kJourneyCountBusinessId);
      list_client_ = new TripodClient(kJourneyNameSpaceId, kJourneyListBusinessId);

      tempMutexManagerPtr_ = new xce::tempmutex::TempMutexManager(100);
      time_adapter_ = new MyUtil::Clock();
    }
    virtual ~JourneyLogicAdapter() {
      if (time_adapter_ != NULL) {
        delete time_adapter_;
      }
      if (count_client_ != NULL) {
        delete count_client_;
      }
      if (list_client_ != NULL) {
        delete list_client_;
      }
    }
    virtual void invote(long key, int guestId);
    virtual MyUtil::IntSeq getList(long key);
    virtual int getCount(long key);
    virtual void reset(long key);
    virtual void fillRedis(long key);
  private:
    MyUtil::Clock* time_adapter_;
    TripodClient* count_client_;
    TripodClient* list_client_;
    xce::tempmutex::TempMutexManagerPtr tempMutexManagerPtr_;
};

}}

#endif
