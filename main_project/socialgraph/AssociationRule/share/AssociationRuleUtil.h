/*************************************************************************
    > File Name: AssociationRuleUtil.h
    > Author: zunwen.you
    > Mail: zunwen.you@renren-inc.com 
    > Created Time: 2012年09月25日 星期二 11时36分42秒
 ************************************************************************/
#ifndef __ASSOCIATION_RULE_UTIL_H__
#define __ASSOCIATION_RULE_UTIL_H__
#include <AssociationRule.h>

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "ObjectCacheI.h"
#include "Util.h"
#include "QueryRunner.h"
#include <FriendFinder.h>
#include <FriendFinderInfoCache.h>
#include <FriendFinderByIP.h>
#include <IPRecord.h>

namespace xce {
namespace socialgraph {

  class AssociationRuleUtil {
    public:
      static friendDataSeq getFoFRecommend(int userId, int limit); 
      static RcdInfoSeq getFoFRcd(int userId, int limit); 
      static MyUtil::IntSeq getFriendSeq(int userId);
      static MyUtil::Int2IntSeqMap getFriendSeqBatch(const MyUtil::IntSeq& ids); 
	    static MyUtil::IntSeq getApplySeq(int userId);
	    static MyUtil::IntSeq getApplySeqByCache(int userId);
	    static MyUtil::IntSeq getBlockSeq(int userId);
	    static MyUtil::IntSeq getBlockSeqByCache(int userId);
	    static MyUtil::IntSeq getFriendRank(int userId);
	    static std::map<int,bool> getPhotoStateBatch(const MyUtil::IntSeq& ids);
    	static std::map<int,bool> getStatusBatch(const MyUtil::IntSeq& ids);
    	static std::map<int,bool> getStateStarBatch(const MyUtil::IntSeq& ids);
	    static std::map<int,bool> getOnlineStateBatch(const MyUtil::IntSeq& ids);
  };

//***************************************************************************************************************

class ApplyResultHandler : public com::xiaonei::xce::ResultHandler {
public:
	ApplyResultHandler(MyUtil::IntSeq& result): _result(result) {}
	~ApplyResultHandler();

protected:
	virtual bool handle(mysqlpp::Row& row) const;
	
private:
	MyUtil::IntSeq& _result;
};

//***************************************************************************************************************

class BlockResultHandler : public com::xiaonei::xce::ResultHandler {
public:
        BlockResultHandler(MyUtil::IntSeq& results, const std::string& toField);
        virtual ~BlockResultHandler();
protected:
        virtual bool handle(mysqlpp::Row& row) const;
private:
        MyUtil::IntSeq& _results;
        std::string _toField;
};

//***************************************************************************************************************

}
}

#endif
