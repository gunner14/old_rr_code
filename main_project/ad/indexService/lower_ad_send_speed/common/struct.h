#ifndef __XCE_AD_STRUCT__
#define __XCE_AD_STRUCT__

#include <IceUtil/Shared.h>
#include <IceUtil/Handle.h>

namespace xce{
namespace ad{

struct DummyRecord : public IceUtil::Shared{
  DummyRecord() : value_(0.0), update_time_(0){}

  double value_;
  time_t update_time_;
};

typedef IceUtil::Handle<DummyRecord> DummyRecordPtr;

}//end of namespace ad
}//end of namespace xce


#endif
