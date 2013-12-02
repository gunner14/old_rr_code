#ifndef EXAMPLE_PRODUCER_H_
#define EXAMPLE_PRODUCER_H_

#include "tripod2/cc/core/producer.h"

namespace xce {
namespace tripod {

class ExampleProducer : public Producer {
 public:
  virtual bool Create(const std::string& key, ByteSeq* value, bool use_master);
};

}} // xce::tripod
#endif

