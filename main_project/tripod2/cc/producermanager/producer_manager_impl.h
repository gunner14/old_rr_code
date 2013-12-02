#ifndef PRODUCER_MANAGER_IMPL_H_
#define PRODUCER_MANAGER_IMPL_H_


#include "Ice/Ice.h"


#include "OceSlice/slice/tripod2.h"

namespace xce {
namespace tripod {

class ProducerManagerImpl : public ProducerManager {
 public:
  ProducerManagerImpl();
  virtual ~ProducerManagerImpl();

  virtual ByteSeq produce(const std::string& key, 
                              const std::string& ns, 
                              const std::string& biz, 
                              bool use_master, 
                              const Ice::Current& = ::Ice::Current());

  virtual std::string ping(const ::Ice::Current& = ::Ice::Current());
};


}} // xce::tripod


#endif


