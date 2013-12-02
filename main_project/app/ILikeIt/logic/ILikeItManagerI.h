#ifndef __I_LIKE_IT_MANAGER_H__
#define __I_LIKE_IT_MANAGER_H__

#include <ILikeIt.h>
#include "IceExt/src/ServiceI.h"
#include "app/ILikeIt/share/ILikeItUtil.h"
#include "UtilCxx/src/TaskManager.h"
#include "DbCxxPool/src/QueryRunner.h"
#include "LocalProducerManagerAdapter.h"

namespace xce {
namespace ilikeit {

const int TASK_LEVEL_ADD = 1;
const int TASK_LEVEL_REMOVE = 2;
const int LOGPATH_ID_ADD = 0;
const int LOGPATH_ID_REMOVE = 1;

const int EXESQL_ADD = 0;
const int EXESQL_DELETE = 1;
const int EXESQL_OK = 0;
const int EXESQL_ERR = 1;

const int SQL_INTERVAL = 30;

// ===================== ILikeItManagerI主类 ==============================
class ILikeItManagerI : virtual public ILikeItManager,
			public MyUtil::Singleton<ILikeItManagerI>  {
public:
	void initialize();

	virtual void addLike(const LikeshipPtr& ship, const Ice::Current& crt=Ice::Current());	

	virtual LikeInfoPtr addLikeAndGet(const LikeshipPtr& ship, const Ice::Current& crt=Ice::Current());	

	virtual LikeInfo2Ptr addLikeAndGet2(const LikeshipPtr& ship, const Ice::Current& crt=Ice::Current());	

	virtual void addLikeBatch(const LikeshipSeq& shipSeq, const Ice::Current& crt=Ice::Current());

	virtual void removeLike(const LikeshipPtr& ship, const Ice::Current& ctr=Ice::Current());
	
	virtual LikeInfoPtr removeLikeAndGet(const LikeshipPtr& ship, const Ice::Current& ctr=Ice::Current());

	virtual LikeInfo2Ptr removeLikeAndGet2(const LikeshipPtr& ship, const Ice::Current& ctr=Ice::Current());

	virtual void removeLikeBatch(const LikeshipSeq& shipSeq, const Ice::Current& crt=Ice::Current());

private:
  //EXESQL_OK表示正常返回，EXESQL_ERR表示出错
  int exeSql(const LikeshipPtr& ship, int opCode);
	friend class MyUtil::Singleton<ILikeItManagerI>;
	ILikeItManagerI();
	

  void addDataSyncToLoad(long gid);

  boost::shared_ptr<com::renren::datasync::RouterLocalProducerManagerAdapter<long> > like_local_producer_manager_adapter_;

};

};
};


#endif
