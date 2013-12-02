#include "AssociationRuleAdapter.h"

namespace xce {
namespace socialgraph {

friendDataSeq AssociationRuleAdapter::getRecommendByAssociationRule(int userId, int applyId, int limit) {
  friendDataSeq result;
  try {
    result = getProxy(userId)->getRecommendByAssociationRule(userId, applyId, limit);
  } catch (...) {
    //Nothing to do
  }
  return result;
}

RcdInfoSeq AssociationRuleAdapter::getRcdByAssociationRule(int userId, int applyId, int limit) {
  RcdInfoSeq result;
  try {
    result = getProxy(userId)->getRcdByAssociationRule(userId, applyId, limit);
  } catch (...) {
    //Nothing to do
  }
  return result;
}

}
}
