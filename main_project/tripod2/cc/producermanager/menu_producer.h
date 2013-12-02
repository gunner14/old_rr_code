/*
* =====================================================================================
*    Description:  
*
*        Created:  2012年06月06日 11时45分39秒
*         Author:  li.li1@renren-inc.com
*
* =====================================================================================
*/


#ifndef __MENU_PRODUCER_H__
#define __MENU_PRODUCER_H__

#include "app/MenuCache/MenuTripod/MenuTripodShare.h"
#include "boost/lexical_cast.hpp"
#include "tripod2/cc/core/producer.h"
#include "DbCxxPool/src/QueryRunner.h"

namespace xce {
namespace tripod {

class MenuProducer : public Producer {
 public:
  explicit MenuProducer(xce::menu::CacheType cache_type) : cache_type_(cache_type) {}
  virtual ~MenuProducer() {}
  virtual bool Create(const std::string& key, ByteSeq* value, bool use_master);
 private:
  xce::menu::CacheType cache_type_;
};

}} //xce::tirpod

#endif
