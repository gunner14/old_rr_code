/*
* =====================================================================================
*    Description:  
*
*        Created:  2012年06月06日 11时45分39秒
*         Author:  li.li1@renren-inc.com
*
* =====================================================================================
*/


#ifndef __USER_VIEWCOUNT_PRODUCER_H__
#define __USER_VIEWCOUNT_PRODUCER_H__

#include "app/FootprintUnite/share/FootprintShare.h"
#include "boost/lexical_cast.hpp"
#include "tripod2/cc/core/producer.h"

namespace xce {
namespace tripod {

class UserViewCountProducer : public Producer {
 public:
  UserViewCountProducer() {}
  virtual ~UserViewCountProducer() {}
  virtual bool Create(const std::string& key, ByteSeq* value, bool use_master);
};

}} //xce::tirpod

#endif
