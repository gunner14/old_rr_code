#include "FriendRankCacheI.h"
#include "FriendRankLoaderNewAdapter.h"
#include "DbCxxPool/src/ConnectionPoolManager.h"
#include "ReplicaCluster/CacheLoader/src/CacheLoader.h"
#include "ReplicaCluster/ServerState/src/client/cxx/ServerStateSubscriber.h"


using namespace MyUtil;
using namespace xce::serverstate;
using namespace xce::socialgraph;
using namespace com::xiaonei::xce;
// MyUtil::initialize {{{
void MyUtil::initialize()
{
	MCE_INFO("FriendRankCacheI MyUtil::initialize().");
	ServiceI& service = ServiceI::instance();
        service.getAdapter()->add(&FriendRankCacheI::instance(),
                        service.createIdentity("FriendRankCacheNew", ""));
        service.registerObjectCache(FRIEND_RANK_DATA, "FRIENDRANKNEW", new FriendRankFactoryI, false);

        ConnectionPoolManager::instance().initialize();

        int mod = service.getCommunicator()->getProperties()->getPropertyAsInt("FriendRankCacheNew.Mod");
        int sInterval = service.getCommunicator()->getProperties()->getPropertyAsIntWithDefault("FriendRankCacheNew.Interval", 5);

  //向controller注册
        ServerStateSubscriber::instance().initialize("ControllerFriendRankCacheNew", &FriendRankCacheI::instance(),
                                         mod, sInterval, new MyUtil::SocialGraphChannel());
        ServerStateSubscriber::instance().setStatus(1);

//        FriendRankUpdateI::instance().Bootstrap();
        // 设置线程类消费线程的最大值和最小值，可根据任务执行时间长度来调整
	TaskManager::instance().config(TASK_LEVEL_FRIEND_RANK_CACHE_FILL, ThreadPoolConfig(1,1));
        TaskManager::instance().config(TASK_LEVEL_FRIEND_RANK_CACHE_LOAD, ThreadPoolConfig(1,1));
}// }}}

// FriendRankCacheI::Bootstrap {{{
// 进行FriendRankCacheI的初始化工作
void FriendRankCacheI::Bootstrap(const string& endpoint, int mod)
{
	MCE_INFO( "[FriendRankCacheI::Bootstrap] endpoint=" << endpoint << " mod=" << mod );
	_endpoint = endpoint;
	_mod = mod;
}// }}}

// FriendRankFactoryI::create(Ice::Int id) {{{
Ice::ObjectPtr FriendRankFactoryI::create(Ice::Int userId)
{
	MCE_INFO("FriendRankFactoryI::create single -> id: "<<userId);
	// 直接从数据库取数据
	FriendRankDataNewPtr result = new FriendRankDataNew;
	com::xiaonei::xce::Statement sql;
	sql	<< "SELECT user_id,rank FROM friend_rank_new WHERE user_id=" << userId;
	com::xiaonei::xce::QueryRunner("friend_rank", com::xiaonei::xce::CDbRServer).query(
			sql, FriendRankSingleResultHandler(result));
	
	// 策略：这里即使userId没有数据也要向Cache加载一个空对象，这样下次就会更快
	// 调用Loader加载该Id (因为服务可能有多份要更新Cache)
	TaskManager::instance().execute(new LoadTask(userId));
	return result;
}// }}}

// FriendRankSingleResultHandler::handle {{{
bool FriendRankSingleResultHandler::handle(mysqlpp::Row& row) const 
{
	// if using binary
	const int num = row["rank"].length() / sizeof(FriendRankDataNew);
	_result->data.reserve(num);
	const char* p = row["rank"].c_str();
	RankDataNew elem;
	for (int i=0; i<num; i++)
	{
		elem.fid = *(int*)p;  p += sizeof(int);
		elem.staticScore = *(float*)p;  p += sizeof(float);
		elem.activeScore = *(float*)p;  p += sizeof(float);
		//MCE_INFO(userid<<weight);
		_result->data.push_back(elem);
	}
	return true;
}// }}}

RankSeqIntNew FriendRankCacheI::GetRankWithFactor(int userId, float factor, const Ice::Current & cur)
{
	MCE_INFO("FriendRankCacheI::GetRankWithFactor( " << userId << " )"<<"factor:"<<factor);
	FriendRankDataNewPtr rank =
                ServiceI::instance().findObject<FriendRankDataNewPtr>(FRIEND_RANK_DATA, userId);
	int num = rank->data.size();
	vector<RankDataNew> vec;
	RankSeqIntNew res;
	float maxScore = 0;
    	float minScore = 10000;
	float stemp = 0;
	vector<RankScoreFloat> vecFloat;	
	int userid = 0;
	int weight = 0;
	float sFactor = 1.0;
	if(factor < -1.0) sFactor = 0.0;
	if(num == 0) return RankSeqIntNew();
	else{
		for(vector<RankDataNew>::iterator iter = rank->data.begin(); iter!=rank->data.end(); iter++)
		{
			stemp = sFactor * iter->staticScore + factor * iter->activeScore;
			RankScoreFloat tmp;
			tmp.fid = iter->fid;
			tmp.weight = stemp;
			vecFloat.push_back(tmp);
			if (maxScore < stemp)
                        	maxScore = stemp;
                	if (minScore > stemp)
                        	minScore = stemp;
		}
		sort(vecFloat.begin(), vecFloat.end(), RankScoreComp_ByScore());
		for (vector<RankScoreFloat>::const_iterator iter1 = vecFloat.begin();
                        iter1 != vecFloat.end(); ++iter1)
        	{
                	userid = iter1->fid;
              		weight = 10 + (int)(90*(0.001+iter1->weight-minScore)/(0.001+maxScore-minScore));
			RankDataInt tmp;
			tmp.fid = userid;
			tmp.score = weight;
			res.push_back(tmp);
        	}
		return res;
	}
	
}
// FriendRankCacheI::GetRank {{{
RankSeqNew FriendRankCacheI::GetRank(int userId, const Ice::Current & cur)
{
	MCE_INFO("FriendRankCacheI::GetRank( " << userId << " )");
	// 方案1：若Cache中不存在，会自动调用FriendRankFactoryI::create
	//FriendRankDataPtr rank = 
	//	ServiceI::instance().locateObject<FriendRankDataPtr>(FRIEND_RANK_DATA, userId);

	// 方案2：若Cache中不存在，就直接返回空对象。前提DB与Cache保持一致
	FriendRankDataNewPtr rank =
		ServiceI::instance().findObject<FriendRankDataNewPtr>(FRIEND_RANK_DATA, userId);
	if (!rank)
	{
		return RankSeqNew();
	}
	else
		return rank->data;
}// }}}
// FriendRankCacheI::GetRevRank {{{
map<int,int> FriendRankCacheI::GetRevRank(int hostId, const MyUtil::IntSeq& guestIds,
		const Ice::Current& cur)
{

	MCE_INFO("FriendRankCacheI::GetRevRank("<<hostId<<",guestIds.size="<<guestIds.size()<<")");
	map<int,int> revrank;
	return revrank;
}// }}}

// FriendRankCacheI::SetRank {{{
void FriendRankCacheI::SetRank(int userId, const FriendRankDataNewPtr& obj, 
		const Ice::Current & cur)
{
	MCE_INFO("FriendRankCacheI::SetRank( " <<userId << " )");
	MCE_INFO("data size:"<<obj->data.size());
	ServiceI::instance().addObject(obj, FRIEND_RANK_DATA, userId);
}// }}}
// FriendRankCacheI::SetRanks {{{
void FriendRankCacheI::SetRanks(const ::MyUtil::ObjectResultPtr& rankData, 
		const Ice::Current & cur)
{
	MCE_INFO( "[FriendRankCacheI::setRanks] size=" << rankData->data.size());
	setData(rankData, cur);
}// }}}

// FriendRankCacheI::setData {{{
// Cache-loader内部机制调用
void FriendRankCacheI::setData(const ::MyUtil::ObjectResultPtr& rankData,
		const Ice::Current& cur)
{
	MCE_INFO( "[FriendRankCacheI::setData] size=" << rankData->data.size());
	TaskManager::instance().execute(new FillTask(rankData));
}// }}}
// Load {{{
void FriendRankCacheI::Load(const RankSeqMapNew& datas, 
		const Ice::Current& cur)
{
	MCE_INFO( "[FriendRankCacheI::Load] size=" << datas.size());
	MyUtil::ObjectResultPtr allrank = new MyUtil::ObjectResult;
	RankDataNew elem;
	int hostId = 0;

	for (RankSeqMapNew::const_iterator iter = datas.begin();
			iter != datas.end(); ++iter)
	{
		hostId = iter->first;
		const vector<RankDataNew>& rank = iter->second;
		int num = rank.size();

		FriendRankDataNewPtr myrank = new FriendRankDataNew;
		myrank->data.reserve(num);
		for (int i=0; i<num; ++i)
		{
			elem.fid = rank[i].fid;
			elem.staticScore = rank[i].staticScore;
			elem.activeScore = rank[i].activeScore;
			myrank->data.push_back(elem);
		}
		allrank->data.insert(make_pair(hostId,myrank));
	}
	setData(allrank, cur);
}// }}}

// FriendRankCacheI::isValid {{{
bool FriendRankCacheI::isValid(const Ice::Current&)
{
	MCE_INFO("[FriendRankCacheI::isValid] " << _isValid );
	IceUtil::RWRecMutex::RLock lock(_validMutex);
	return _isValid;
}// }}}

// FriendRankCacheI::setValid {{{
void FriendRankCacheI::setValid(bool valid,const Ice::Current&)
{
	MCE_INFO( "[FriendRankCacheI::setValid] " << valid );
	IceUtil::RWRecMutex::WLock lock(_validMutex);
	_isValid = valid;
	if( valid ){
		ServerStateSubscriber::instance().setStatus(1);
	} else{
		ServerStateSubscriber::instance().setStatus(0);
	}
}// }}}


// FillTask::handle {{{
void FillTask::handle()
{
	ServiceI::instance().getObjectCache()->addObjects(FRIEND_RANK_DATA, _rankData->data);
	MCE_INFO("FillTask done. Task size / ObjectCache size : "
			<< _rankData->data.size() << " / "
			<< ServiceI::instance().getObjectCache()->locateCategory(FRIEND_RANK_DATA)->size());
}// }}}

// LoadTask::handle {{{
void LoadTask::handle()
{
	FriendRankLoaderAdapter::instance().reload(_userId);
}// }}}


// ServiceSchemaI::configure {{{
// 小白那边JSP监控使用
void ServiceSchemaI::configure(const Ice::PropertiesPtr& props)
{
	TaskManager::instance().config(props, 
			"Fill", TASK_LEVEL_FRIEND_RANK_CACHE_FILL, ThreadPoolConfig(1,1));
	TaskManager::instance().config(props, 
			"Load", TASK_LEVEL_FRIEND_RANK_CACHE_LOAD, ThreadPoolConfig(1,1));
}// }}}

