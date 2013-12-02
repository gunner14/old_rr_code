#ifndef ENTRY_TYPE_H_
#define ENTRY_TYPE_H_

#include <string>
#include "tripod2/cc/client/namespace_constant.h"
#include "tripod2/cc/client/business_constant.h"
#include "tripod2/cc/client/entry.h"

namespace xce {
namespace tripod {
namespace client {

namespace entry_type {
  const Entry kBizSocialgraphCommon(namespace_id::kSocialgraphCommon,biz_id::kSocialGraphCommon);
  const Entry kBizSocialGraphPhotoRecommend(namespace_id::kSocialgraphCommon,biz_id::kSocialGraphPhotoRecommend);
  const Entry kSocialGraphRcdFriendBr(namespace_id::kSocialgraphCommon,biz_id::kSocialGraphRcdFriendBr);
} 

}}}

#endif
