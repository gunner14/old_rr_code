#ifndef __FEEDDISPATCHNEWI_H__
#define __FEEDDISPATCHNEWI_H__

#include "RFeed.h"
#include "QueryRunner.h"
#include "Singleton.h"
#include "ObjectCache.h"
#include "DbHelper.h"
#include <set>
//#include <boost/multi_index/member.hpp>
//#include <boost/multi_index/ordered_index.hpp>
//#include <boost/multi_index/sequenced_index.hpp>
//#include <boost/multi_index/composite_key.hpp>
//#include <boost/multi_index/hashed_index.hpp>
//#include <boost/multi_index/identity.hpp>




namespace xce {
namespace feed {


using namespace MyUtil;
//using boost::multi_index_container;
//using namespace boost::multi_index;

const std::string FEED_DISPATCHNEW = "FDNW";

class FeedDispatchNewI: public FeedDispatchNew, public MyUtil::Singleton<FeedDispatchNewI> {
public:
  FeedDispatchNewI() {
    int arr[TYPE_FILTER_SIZE]={213,217,218, 701, 702, 708, 709, 2008, 2013, 3731, 3730, 3733};
    _typeFilter.insert(arr, arr+FeedDispatchNewI::TYPE_FILTER_SIZE);
  }

	virtual void Dispatch(const FeedSeedPtr& seed, const Ice::Current& =
			Ice::Current());
  //----------------------原FeedAssistantN接口
  //分发新鲜事
	virtual void DispatchWithReply(const FeedSeedPtr& seed, const FeedReplyPtr& reply, const Ice::Current& = Ice::Current());
	virtual void DispatchEDM(const FeedSeedPtr& seed, const Ice::Current& = Ice::Current());
	virtual void DispatchAgain(Ice::Long source, int stype, int actor, int toid, const Ice::Current& = Ice::Current());
	virtual void DispatchNewsAgain(Ice::Long feedid, const string& expr, const Ice::Current& = Ice::Current());
  //添加回复
	virtual void AddFeedReplyById(int stype, Ice::Long feedid, const FeedReplyPtr& reply, const Ice::Current& = Ice::Current()); 
  virtual void AddFeedReplyWithConfig(Ice::Long source, Ice::Int stype,
      Ice::Int actor, const FeedReplyPtr& reply, const map<string,string> & reply_config, const Ice::Current& current = Ice::Current());
	virtual void AddFeedReply(Ice::Long source, Ice::Int stype, Ice::Int actor,
							  const FeedReplyPtr& reply,
							  const Ice::Current& = Ice::Current());
  //删除新鲜事,回复
  	virtual void RemoveFeedByTime(Ice::Long source, Ice::Int stype, Ice::Int actor, Ice::Int time, const Ice::Current& =
			Ice::Current());
	virtual void RemoveFeedById(Ice::Long feedId, const Ice::Current& =
			Ice::Current());
	virtual void RemoveFeedBySource(Ice::Long source, Ice::Int stype,
			const Ice::Current& = Ice::Current());
	virtual void RemoveFeedBySourceHS(Ice::Long source, Ice::Int stype,
			const Ice::Current& = Ice::Current());
	virtual void RemoveFeedByParent(Ice::Long parent, Ice::Int ptype,
			const Ice::Current& = Ice::Current());
	virtual void RemoveFeed(Ice::Long source, Ice::Int stype, Ice::Int actor,
			const Ice::Current& = Ice::Current());
	virtual void RemoveFeedReply(Ice::Long source, Ice::Int stype, Ice::Int actor,
								 Ice::Long removeReplyId, const FeedReplyPtr& reply,
								 const Ice::Current& = Ice::Current());
	virtual void RemoveFeedReplyById(int stype, Ice::Long feedid, Ice::Long removeReplyId, const FeedReplyPtr& reply, const Ice::Current& = Ice::Current());
  //-------------- 重载表达式
  virtual void Reload(LoadType type, int id, const Ice::Current& =
      Ice::Current());
  virtual void Reload2(LoadType type, long id, const Ice::Current& =
      Ice::Current());
  virtual void ReloadBatch(LoadType type, const vector<int> & ids,
      const Ice::Current& = Ice::Current());
  //-------------新鲜事导入接口
  /* 待确定无其他业务调用此接口,去掉此注释,否则还原接口
  virtual void dispatchReply(const ReplyDataPtr&, const FeedConfigPtr&,
      const Ice::Current& = Ice::Current());
  */
  virtual void AddBuddyNotify(int host, int guest, const Ice::Current& =
      Ice::Current());
  virtual void BuddyApplyNotify(int host, int guest, const Ice::Current& =
      Ice::Current());
  virtual void JoinPageNotify(int page, int fans, const Ice::Current& =
      Ice::Current());
  virtual void GuideBuddyReplyNotify(int newUser, int buddy,
      const Ice::Current& = Ice::Current());
	virtual void AddMiniSiteFeed(int siteid, int userid, const Ice::Current& = Ice::Current());
	virtual void AddMiniSiteFeedToGuides(long source,int type,int siteid, const MyUtil::IntSeq & guides, const Ice::Current& = Ice::Current());
  //--------------其他接口
  virtual void CacheSync(Ice::Long source ,int stype, int actor,const Ice::Current & = Ice::Current());
  virtual Ice::Long GetFeedId(Ice::Long source,int stype,int actor,const Ice::Current & = Ice::Current());
  virtual void ReplaceXML(Ice::Long source,int stype,int actor,const string & xml,const Ice::Current & = Ice::Current());
  virtual void DirectedSend(Ice::Long source,int stype,int actor,const vector<int> & targets,const Ice::Current & = Ice::Current()); 

  bool IsNormalFeedType(int stype);
	void PrintThreadPoolInfoByLevel();	
  bool RefleshMiniGroupTime(const map<string,string> & config,const FeedReplyPtr & reply,const vector<long> & fids,int actor);
  FeedIndexSeq GetFeedIndex(Ice::Long source,int stype,int actor = -1) ;
  FeedIndexPtr GetFeedIndexById(int feed);

  //-----------------------
    bool NotNormalFeedType(int stype) {
      return (_typeFilter.count(stype));
    }
private:
  //向指定user导入新鲜事
  void AppendNewsFeed(int from, int to, int limit, char * funcname = NULL);
  void AppendNewsFeed(int from, int to, int type, int limit, char * funcname = NULL);
 	//查找到所有符合条件的新鲜事（source,stype,actor），更新feedcontent，返回id序列中的第一个id，如果没有符合条件的feed，返回-1
	Ice::Long _replaceFeedReply(Ice::Long source, Ice::Int stype, Ice::Int actor,
						 const FeedReplyPtr& reply);
  bool IsNewFeedidForDb(long feedid) {
         return (feedid / DB_CONTENT_ZONE) >= DB_ZONE_SPLIT || feedid < 0;
   }
	static void SetEdmTime(const FeedSeedPtr & seed){
		int type = seed->type & 0xFFFF;
		if(type == 8015 || type == 8016 ){
			seed->time = -2;
		}else{
			seed->time = -480;
		}
		MCE_INFO("FeedDispatchNewI::SetEdmTime. fid:" << seed->feed << " actor:" << seed->actor
				<< " type:" << type << " source:" << seed->source  << " time:" << seed->time);
	}
private:
	IceUtil::Mutex _mutex;
	static const int EDM_MUTEX_COUNT = 100;
	IceUtil::Mutex _EDMMutexs[EDM_MUTEX_COUNT];
  set<int> _typeFilter;
public:
	static const int TYPE_FILTER_SIZE = 12;
};
class StatTimer: public Timer{
public:
	StatTimer():Timer(5000){};
	virtual void handle();
};


	
}
;
}
;

#endif /* FEEDASSISTANTI_H_ */
