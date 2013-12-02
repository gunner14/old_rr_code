#ifndef _SORT_BY_CLUSTER_ADAPTER_H_
#define _SORT_BY_CLUSTER_ADAPTER_H_

#include "Channel.h"
#include "Singleton.h"
#include "AdapterI.h"
#include "AssociationRule.h"

namespace xce {
namespace socialgraph {

class AssociationRuleAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::AssociationRulePrx>, 
		public MyUtil::Singleton<AssociationRuleAdapter> {
 public:
  AssociationRuleAdapter() :
		MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::AssociationRulePrx>("ControllerAssociationRule", 120, 1000, 
																																						new MyUtil::SocialGraphChannel()) { }
  
      friendDataSeq getRecommendByAssociationRule(int userId, int applyId, int limit);
      RcdInfoSeq getRcdByAssociationRule(int userId, int applyId, int limit);
};


}}

#endif

