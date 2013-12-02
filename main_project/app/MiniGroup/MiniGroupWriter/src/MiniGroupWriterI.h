/*
 * =====================================================================================
 *
 *       Filename:  MiniGroupWriterI.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/27/2011 08:05:54 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  min.shang (Jesse Shang), min.shang@renren-inc.com
 *        Company:  renren-inc
 *
 * =====================================================================================
 */
#ifndef __MINIGROUP_WRITERI_H__
#define __MINIGROUP_WRITERI_H__

//#include "../../../../OceCxxAdapter/src/LocalProducerManagerAdapter.h"

#include "LocalProducerManagerAdapter.h"
#include "../../MiniGroupCommon/src/MiniGroupCommon.h"
#include "ServiceI.h"

namespace xce {
namespace mngp {
using namespace com::renren::datasync;

class MiniGroupWriterI : public MiniGroupWriter, public MyUtil::Singleton<MiniGroupWriterI> {
  public:
    MiniGroupWriterI();
    void addJoinedMiniGroups(int user_id, const MyUtil::LongSeq& minigroup_ids, const Ice::Current& = Ice::Current());
    void setJoinedMiniGroups(int user_id, const MyUtil::LongSeq& minigroup_ids, const Ice::Current& = Ice::Current());
    void removeJoinedMiniGroups (int user_id, const MyUtil::LongSeq& minigroup_ids, const Ice::Current& = Ice::Current());
    void reloadUsers(const MyUtil::IntSeq& user_ids, const Ice::Current& = Ice::Current());
    void reloadMiniGroups(const MyUtil::LongSeq& miniGroupIds, const Ice::Current& = Ice::Current());
    void addMiniGroups(const MiniGroupSeq& miniGroups, const Ice::Current& = Ice::Current());
  private:
    void reloadMiniGroupsTripodCache(long miniGroupId);
    void reloadUser2MiniGroupsTripodCache(int userId);
    void reloadUser2MiniGroups(const MyUtil::IntSeq& userIds, int mod);
    void reloadMiniGroups(const MyUtil::LongSeq& userIds, int mod);
    RouterLocalProducerManagerAdapter<int>* user2MiniGroupProducerAdapter_;
    RouterLocalProducerManagerAdapter<long>* miniGroupProducerAdapter_;
};

}
}

#endif
