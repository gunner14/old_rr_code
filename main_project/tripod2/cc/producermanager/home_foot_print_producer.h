/*
* =====================================================================================
*    Description:  
*
*        Created:  2012年06月06日 11时45分39秒
*         Author:  li.li1@renren-inc.com
*
* =====================================================================================
*/


#ifndef __HOME_FOOTPRINT_PRODUCER_H__
#define __HOME_FOOTPRINT_PRODUCER_H__

#include "app/FootprintUnite/share/FootprintShare.h"
#include "boost/lexical_cast.hpp"
#include "tripod2/cc/core/producer.h"
#include "DbCxxPool/src/QueryRunner.h"

namespace xce {
namespace tripod {

class HomeFootprintProducer : public Producer {
 public:
  explicit HomeFootprintProducer(int footprintSize);
  virtual ~HomeFootprintProducer() {}
  virtual bool Create(const std::string& key, ByteSeq* value, bool use_master);
 private:
  string GetTablePrefix();
  int footprintSize_;
};

}} //xce::tirpod

#endif
