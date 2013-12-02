/******************************************************************************
Function:  Implementaion of processing weights for friends recommendation
Developer: xiongjunwu
Email:     junwu.xiong@renren-inc.com
Date:      2012-02-29
*******************************************************************************/

#ifndef __SOCIALGRAPH_FOF_I_H_
#define __SOCIALGRAPH_FOF_I_H_

#include "IceExt/src/ServiceI.h"
#include "DbCxxPool/src/QueryRunner.h"
//#include "item.pb.h"
#include "UtilCxx/src/TaskManager.h"
#include "BuddyByApplicantAdapter.h"

#include "SocialGraphFoF.h"
#include "socialgraph/socialgraphutil/cctypedef.h"
#include "socialgraph/socialgraphfof/src/fofrank.h"

namespace xce {
namespace socialgraph {
class SocialGraphFoFI : virtual public SocialGraphFoF, virtual public MyUtil::Singleton<SocialGraphFoFI> {
 public:
  virtual FoFRankSeq getFoFRankSeq(Ice::Int hostId, Ice::Int limit, const Ice::Current&);
  virtual FoFRankSeq getFoFRankSeqN(Ice::Int hostId, Ice::Int begin, Ice::Int limit, const Ice::Current&);
  //virtual Int2FoFRankSeqMap getFoFRankSeqs(const MyUtil::IntSeq&, Ice::Int limit, const Ice::Current&);
  //virtual Int2FoFRankSeqMap getFoFRankSeqsN(const MyUtil::IntSeq&, Ice::Int begin, Ice::Int limit,const Ice::Current&);
 // virtual void setFoFRank(Ice::Int hostId, const FoFRank&, const Ice::Current&);
 // virtual void setFoFRanks(Ice::Int hostId, const FoFRankSeq&, const Ice::Current&);
 // virtual void AddFoFRank(Ice::Int hostId, const FoFRank&, const Ice::Current&);
 // virtual void DelFoFRank(Ice::Int hostId, const FoFRank&, const Ice::Current&);

 private:
  friend class MyUtil::Singleton<SocialGraphFoFI>;
  SocialGraphFoFI() {}

};

};
};

#endif
