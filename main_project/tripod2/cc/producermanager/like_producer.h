/*
* =====================================================================================
*    Description:  
*
*        Created:  2012年06月06日 11时45分39秒
*         Author:  li.li1@renren-inc.com
*
* =====================================================================================
*/


#ifndef __LIKE_PRODUCER_H__
#define __LIKE_PRODUCER_H__

#include "app/ILikeIt/share/ILikeItUtil.h"
#include "boost/lexical_cast.hpp"
#include "tripod2/cc/core/producer.h"
#include "DbCxxPool/src/QueryRunner.h"

namespace xce {
namespace tripod {

class LikeProducer : public Producer {
 public:
  LikeProducer() {}
  virtual ~LikeProducer() {}
  virtual bool Create(const std::string& key, ByteSeq* value, bool use_master);
};

}} //xce::tirpod

#endif
