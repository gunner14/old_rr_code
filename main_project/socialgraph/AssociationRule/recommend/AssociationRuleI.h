#ifndef __ASSOCIATION_RULE_I_H__
#define __ASSOCIATION_RULE_I_H__

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include "AssociationRule.h"
#include "IceExt/src/ServiceI.h"

namespace xce {
namespace socialgraph {

class AssociationRuleI: virtual public AssociationRule,
		virtual public MyUtil::Singleton<AssociationRuleI> {
 public:
	  virtual friendDataSeq getRecommendByAssociationRule(int userId, int applyId, 
																								int limit, const Ice::Current& current);
	  virtual RcdInfoSeq getRcdByAssociationRule(int userId, int applyId, 
																								int limit, const Ice::Current& current);
  
};

}
}

#endif
