#ifndef JOURNEYLOGIC_H_
#define JOURNEYLOGIC_H_
#include "ServiceI.h"
#include "OceSlice/slice/Journey.h"
#include "util/cpp/TimeCost.h"
#include "tripod2/cc/core/tripod_client.h"
#include "IceExt/src/TempMutex.h"

#include "UtilCxx/src/TaskManager.h"
#include <IceUtil/Mutex.h>
#include <IceUtil/Monitor.h>

#include "Journey/JourneyLogicTripodAdapter.h"

namespace xce {
namespace journey {

const int kJourneyExpire = 100000000;

const int TASK_LEVEL_CREATE = 1;

class JourneyLogicI : virtual public JourneyLogic, virtual public MyUtil::Singleton<JourneyLogicI> {
  public:
    JourneyLogicI() {
      
      tempMutexManagerPtr_ = new xce::tempmutex::TempMutexManager;
      time_adapter_ = new MyUtil::Clock();
    }
    virtual ~JourneyLogicI() {
      if (time_adapter_ != NULL) {
        delete time_adapter_;
      }
    }
    virtual void invote(::Ice::Long key, 
                ::Ice::Int guestId, 
                ::Ice::Int limit,
                const Ice::Current& = Ice::Current());
    virtual MyUtil::IntSeq getList(::Ice::Long key,
                ::Ice::Int limit,
                const Ice::Current& = Ice::Current());
    virtual int getCount(::Ice::Long key,
                const Ice::Current& = Ice::Current());
    virtual void reset(::Ice::Long key,
                const Ice::Current& = Ice::Current());

  private:
    MyUtil::Clock* time_adapter_;
    xce::tempmutex::TempMutexManagerPtr tempMutexManagerPtr_;


}; 

class ListOpTask : public MyUtil::Task{
  public:
    ListOpTask(long key, int guestId, int limit, bool& flag, MyUtil::Clock* time_adapter,
        IceUtil::Monitor<IceUtil::Mutex>& monitor, int level = TASK_LEVEL_CREATE);
    virtual void handle();

  private:
    long key_;
    int guest_id_;
    int limit_;
    bool& action_flag_;
    MyUtil::Clock* time_adapter_;
    IceUtil::Monitor<IceUtil::Mutex>& list_op_monitor_;
};

}} //xce::journey

#endif
