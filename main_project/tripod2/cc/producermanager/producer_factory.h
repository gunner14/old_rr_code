#ifndef PRODUCER_FACTORY_H_
#define PRODUCER_FACTORY_H_

#include <map>

#include "tripod2/cc/core/producer.h"

namespace xce {
namespace tripod {


struct ProducerFactory {
  static ProducerFactory& GetInstance();
  typedef std::map<std::string, Producer*> BusinessProducerMapType;
  BusinessProducerMapType biz_producers_;

  void Add(const std::string& biz, Producer* producer);
  Producer* Get(const std::string& biz);
};


}} // xce::tripod

#endif

