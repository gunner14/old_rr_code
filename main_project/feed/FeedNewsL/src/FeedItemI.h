#ifndef FEEDITEM_H_
#define FEEDITEM_H_

#include <map>
#include <vector>
#include <set>
#include "RFeed.h"
#include <IceUtil/IceUtil.h>
#include "TaskManager.h"
#include "XceLoggerAdapter.h"
#include "Evictor.h"
#include "UserInfo.h"

using namespace std;
using namespace MyUtil;
using namespace xce::feed;
using namespace boost::multi_index;
using namespace com::xiaonei::service;

namespace xce{
namespace feed{

typedef vector<Ice::Long> Long1D;
typedef vector<Long1D> Long2D;
typedef vector<Long2D> Long3D;

typedef FeedItem* FeedItemPtr;

typedef vector<FeedItemPtr> FeedItem1D;
typedef vector<vector<FeedItemPtr> > FeedItem2D;
typedef vector<vector<vector<FeedItemPtr> > > FeedItem3D;

typedef vector<FeedItemPtr> ItemPtr1D;
typedef vector<vector<FeedItemPtr> > ItemPtr2D;
typedef vector<vector<vector<FeedItemPtr> > > ItemPtr3D;

class GetRequest;

class FeedItemManagerI : public FeedNewsManager, public Singleton<FeedItemManagerI>{
public:
	FeedItemManagerI();
	~FeedItemManagerI();

	FeedDataResPtr	GetLiveFeedData(int uid, int begin, int limit, const Ice::Current& = Ice::Current());
	FeedDataResPtr	GetLiveFeedDataByStype(int uid, const IntSeq& stypes, int begin, int limit, const Ice::Current& = Ice::Current());
	FeedDataResPtr	GetLiveFeedDataByType(int uid, const IntSeq& types, int begin, int limit, const Ice::Current& = Ice::Current());
	FeedDataResPtr	GetHotFeedData(int uid, int begin, int limit, const Ice::Current& = Ice::Current());
	FeedDataResPtr	GetHotFeedDataByStype(int uid, const IntSeq& stypes, int begin, int limit, const Ice::Current& = Ice::Current());
	FeedDataResPtr	GetHotFeedDataByType(int uid, const IntSeq& types, int begin, int limit, const Ice::Current& = Ice::Current());

	FeedDataResPtr	GetFriendOriginal(int uid, const IntSeq & friends, int begin, int limit, const Ice::Current& = Ice::Current()); //好友原创 by duo.zhang 2012.8.15
	FeedDataResPtr	GetFeedDataByGroupName(int uid, const string & groupname, int begin, int limit, const Ice::Current& = Ice::Current()); //好友分组 by duo.zhang 2012.8.16
	FeedDataResPtr	GetFeedDataByGroupId(int uid, int groupid, int begin, int limit, const Ice::Current& = Ice::Current()); //好友分组 by duo.zhang 2012.8.16

	int	GetUnviewedCount(int uid, int type, const Ice::Current& = Ice::Current());
	map<int,int>	GetCountMapByStype(int uid, const vector<int> & type, bool unviewed, const Ice::Current& = Ice::Current());
	void	AddFeed(const FeedItem& item, const FeedConfigPtr& config, const MyUtil::Int2IntMap& id2weight, const Ice::Current& = Ice::Current());
	void	load(int uid, const Ice::Current& = Ice::Current());

	void	SetRead(int uid, int stype, Ice::Long merge, const Ice::Current& = Ice::Current());
	void	SetReadById(int uid, Ice::Long feedId, const Ice::Current& = Ice::Current());
	void	SetReadAll(int uid, const Ice::Current& = Ice::Current());

	FeedItemSeqSeq GetFeedItemSeqSeqByUsers(const IntSeq& uids, const Ice::Current& = Ice::Current());
	FeedItemSeq   GetOriginalFeedData(int uid, const Ice::Current& = Ice::Current());
	FeedDataSeq GetFeedDataByIds(const MyUtil::LongSeq& fids, const Ice::Current& = Ice::Current());
	FeedDataSeq GetFeedDataByIdsWithUid(int uid,const MyUtil::LongSeq& fids, Ice::Long first,int stype, const Ice::Current& = Ice::Current());
	bool HasFeed(int uid, const Ice::Current& = Ice::Current()){ return true; }
  void EraseUser(int uid, const Ice::Current& = Ice::Current());
  virtual void MarkFeed(int uid,long fid,long source,int stype,int actor,bool mark, const Ice::Current& = Ice::Current()){}
  virtual void AddFeedReply(const FeedIndexPtr & idx,int replycnt, const Ice::Current& = Ice::Current()){
	  if(!idx){
		  MCE_WARN("FeedItemManagerI::AddFeedReply. idx null");
		  return;
	  }
	  MCE_INFO("FeedItemManagerI::AddFeedReply. fid:" << idx->feed << " src:" << idx->source << " stype:" << idx->stype << " actor:" << idx->actor << " replycnt:" << replycnt);
  }

  virtual void LoadExperiment(int totalsize, const Ice::Current& = Ice::Current());
  virtual void AddWhiteList(int uid, const Ice::Current& = Ice::Current());
  virtual void DelWhiteList(int uid, const Ice::Current& = Ice::Current());
  virtual MyUtil::IntSeq GetWhiteList(const Ice::Current& = Ice::Current());
  virtual void ClearWhiteList(const Ice::Current& = Ice::Current());

protected:
  bool CheckArgs(int uid, const IntSeq& types,
    bool stype, int begin, int limit, bool hot);
  UserInfoPtr LocateUserInfo(int uid, GetRequest & req);
  UserInfoPtr FindUserInfo(int uid);
  FeedDataResPtr GetFeedDataImpl(int uid, const IntSeq& types, bool stype, int begin, int limit, bool hot);
  FeedDataResPtr GetFeedDataRemote(int uid, const IntSeq& types, bool stype, int begin, int limit, bool hot);

  //当begin+limit>=250时,从好友处拉比feedid(memcache中)小的feeds
  //by jin.zhang(张劲), 2012.03.01
  LongSeq GetFeedidsFromFriends(int uid, int begin, int limit, const IntSeq& types);
  FeedDataResPtr GetFeedDataFromFriends(int uid, int begin, int limit, const IntSeq& types);

  string GetIp(const Ice::Current& cur){
  	if(!cur.con)
  		return "";
  	string ip = cur.con->toString();
  	size_t found = ip.find('\n');
  	if (found == string::npos) {
  		return ip;
  	}
  	string res ( ip.begin() + found + 1,ip.end());
  	return res;
  }

  pair<float,float> GetFeedContentDict(GetRequest & req,FeedContentDict & fdict);
  void CheckLoad(GetRequest & req, const vector<Ice::Long>& fids, FeedContentDict & dict);

	typedef MyUtil::TimedEvictor<Ice::Int, UserInfoPtr> UserInfoCache;
private:
	IceUtil::RWRecMutex white_list_mutex_;
	set<int> white_list_;
	bool InWhiteList(int uid);
};

class GetLogger: public Singleton<GetLogger>{
public:
	void Init();
	void SendLog(int uid,const FeedDataSeq & data);
private:
	XceLoggerPtr logger_;
	vector<string> path_;
};

class StatTimer : public Timer, public Singleton<StatTimer>{
public:
	StatTimer() : Timer(60*1000, 150){	}
	void handle();
};

/*
class CheckEnterHotFeedTimer : public Timer, public Singleton<CheckEnterHotFeedTimer>{
public:
  CheckEnterHotFeedTimer() : Timer(30*1000, 2), ready(false){}
  void handle();
  void SetReady(){
    ready = true;
  }
private:
  bool ready;
};*/
}
}
#endif