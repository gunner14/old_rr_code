#ifndef FeedSINKI_H_
#define FeedSINKI_H_

#include "RFeed.h"
#include "FeedSink.h"
#include "Singleton.h"
#include <IceUtil/IceUtil.h>
#include <TaskManager.h>
using namespace std;
using namespace MyUtil;

namespace xce {
namespace feed {


class FeedSinkI: public FeedSink, public MyUtil::Singleton<FeedSinkI> {
public:
	virtual void	AddFeed(const FeedItem& item, const FeedConfigPtr& config, const MyUtil::Int2IntMap& id2weight, const Ice::Current& = Ice::Current());
	virtual void	SetRead(int uid, int stype, Ice::Long merge, const Ice::Current& = Ice::Current());
	virtual void	SetReadById(int uid, Ice::Long feedId, const Ice::Current& = Ice::Current());
	virtual void	SetReadAll(int uid, const Ice::Current& = Ice::Current());
	virtual void	ReloadWhitelist(const Ice::Current& = Ice::Current());
	virtual void  GetFeedData(int uid, bool hot, bool stype,const MyUtil::IntSeq & stypes, int begin, int limit,const Ice::Current& = Ice::Current());
  virtual void  MarkFeed(int uid,long fid,long source,int stype,int actor,bool mark, const Ice::Current& = Ice::Current());

	void Init();

private:
	Int2IntMap GetTester(const MyUtil::Int2IntMap& id2weight);
//	set<int> testers_;
};

//-------------------------------------------------------------------------------
class FeedNews50Handler:public MyUtil::Singleton<FeedNews50Handler>{
public:
	void LoadWhitelist();
	Int2IntMap GetTester(const MyUtil::Int2IntMap& id2weight);
	bool IsTester(int uid);
	void AddFeed(const FeedItem& fitem,
	    const FeedConfigPtr& config, const MyUtil::Int2IntMap& id2weight);
	void SetRead(int uid, int stype, Ice::Long merge);
	void SetReadById(int uid, long fid);
	void SetReadAll(int uid);
	void  MarkFeed(int uid,long fid,long source,int stype,int actor,bool mark);

	set<int> testers_;
	IceUtil::Mutex	mutex_;
};

class TargetPrinter{
	static void Handle(const FeedItem& fitem,const MyUtil::Int2IntMap& id2weight);
};
}
}
#endif
