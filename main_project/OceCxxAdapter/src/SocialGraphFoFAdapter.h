#ifndef __SOCIALGRAPH_FOF_H_
#define __SOCIALGRAPH_FOF_H_

#include "SocialGraphFoF.h"
#include "Channel.h"
#include "AdapterI.h"
#include "Singleton.h"

namespace xce {
namespace socialgraph {
class SocialGraphFoFAdapter : public MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::SocialGraphFoFPrx>,
   public MyUtil::Singleton<SocialGraphFoFAdapter> {
 public: 
   SocialGraphFoFAdapter():
     MyUtil::ReplicatedClusterAdapterI<xce::socialgraph::SocialGraphFoFPrx>("ControllerSocialGraphFoF",
										  120, 5000, new MyUtil::SocialGraphChannel()) {
   }
  FoFRankSeq getFoFRankSeq(Ice::Int hostId, Ice::Int limit);
  FoFRankSeq getFoFRankSeq(Ice::Int hostId, Ice::Int begin, Ice::Int limit);
  //Int2FoFRankSeqMap getFoFRankSeqs(const MyUtil::IntSeq&, Ice::Int limit);
  //Int2FoFRankSeqMap getFoFRankSeqs(const MyUtil::IntSeq&, Ice::Int begin, Ice::Int limit);
};


};
};

#endif
