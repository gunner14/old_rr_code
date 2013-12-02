#ifndef __SCORE_CACHE_ADAPTER_H__
#define __SCORE_CACHE_ADAPTER_H__

#include <boost/lexical_cast.hpp>
#include <IceUtil/RWRecMutex.h>
#include <ScoreCache.h>
#include <AdapterI.h>
#include <Channel.h>
#include <Singleton.h>
#include <TaskManager.h>

namespace xce {
namespace scorecache {
namespace adapter {

const int MAX_LEVEL = 45;

struct ClientScoreDataN : public xce::scorecache::ScoreDataN {
	ClientScoreDataN( xce::scorecache::ScoreDataN& sdn ):
		id(sdn.id),
		historyLoginDays(sdn.historyLoginDays),
		continueLoginDays(sdn.continueLoginDays),
		activeDays(sdn.activeDays),
		lastLoginTime(sdn.lastLoginTime),
		loginType(sdn.loginType),
		score(sdn.score),
		level(sdn.level),
		nextLevelScore(sdn.nextLevelScore),
		awardCount(sdn.awardCount),
		vip(sdn.vip),
		sign(sdn.sign) {
			
			if(level<=0) return;
			if(level>=MAX_LEVEL)level = MAX_LEVEL;
			int doLevel = level;
			std::ostringstream active;
			active << (continueLoginDays>=7?"levelHot-":"level-");
			for (int i = 0; i < doLevel / 15; i++){
				icons.push_back(active.str()+"4");
			}
			int next = doLevel % 15;
			if (next > 0) {
				string nextStr = (next >= 10) ? "3" : (next >= 5) ? "2" : "1";
				icons.push_back(active.str()+nextStr);
			}

	};
	int id;
	///
	int historyLoginDays;
	int continueLoginDays;
	int activeDays;
	long lastLoginTime;
	int loginType;
	///
	int score;
	int level;
	int nextLevelScore;
	///
	int awardCount;
	///
	/// vip<0: disable
	/// vip>0: vip_level(enable)
	/// vip==0: 0==1
	int vip;
	///
	long sign;
	std::vector<std::string> icons;
};

class ScoreCacheNAdapter : public MyUtil::ReplicatedClusterAdapterI<ScoreCacheNManagerPrx>, public MyUtil::Singleton<ScoreCacheNAdapter> {
public:
  //构造ScoreCacheNAdapter的唯一实例
	ScoreCacheNAdapter() :
		MyUtil::ReplicatedClusterAdapterI <ScoreCacheNManagerPrx>("ControllerScoreCacheN", 120, 300) {
	}

  //获取一个用户的积分信息，以ScoreDataN的方式返回
	xce::scorecache::ScoreDataN getScoreDataN(int userId);
  //批量获取一批用户的积分信息，以一个map返回，其中存放的是userId->ScoreDataN的映射信息
  //返回结果的size可能小于userIds.size，因为cache中可能没有一些用户的积分信息
	xce::scorecache::ScoreDataNMap getScoreDataNMap(const MyUtil::IntSeq& userIds);

  //获取一个用户的积分信息，如果cache中没有此用户的积分信息，那么去数据库进行加载，然后返回
	xce::scorecache::ScoreDataN getScoreDataNNotNull(int userId);

  //批量获取用户的积分信息，返回一个map, 其中存放的是userId->ScoreDataN的映射信息
  //如果cache中没有某些用户的信息，那么去数据库进行加载，然后返回
	xce::scorecache::ScoreDataNMap getScoreDataNMapNotNull(const MyUtil::IntSeq& userIds);

  //获取一个用户的积分信息，以ClientScoreDataN的方式返回
	ClientScoreDataN getClientScoreDataN(int userId);

  //获取一个用户在所有cache中的积分信息，以一个vector<ScoreDataN>返回
	std::vector<xce::scorecache::ScoreDataN> getScoreDataNAllPrx(int userId);

  //批量获取一些用户在所有cache中的积分信息，以一个vector<ScoreDataMap>的形式返回
	std::vector<xce::scorecache::ScoreDataNMap> getScoreDataNMapAllPrx(const MyUtil::IntSeq& userIds);

  //设置用户的登录信息， historyLoginDays表示历史登录天数， continueLoginDays是连续登录天数，
  //lastLoginTime是最后一次登录时间, loginType是最后一次登录类型， succ用来记录成功与否
	Ice::ObjectSeq setLoginDays( int userId, int historyLoginDays, int continueLoginDays, long lastLoginTime, int loginType, bool& succ );

  //更新用户最后的登录类型
	Ice::ObjectSeq updateLoginType( int userId, int loginType, bool& succ );

  //设置用户积分
	Ice::ObjectSeq setScore(int userId, int score, bool& succ);

  //增加用户积分
	Ice::ObjectSeq incScore(int userId, int inc, bool& succ);

  //减少用户积分
	Ice::ObjectSeq decScore(int userId, int dec, bool& succ);

  //设置用户奖励值
	Ice::ObjectSeq setAwardCount(int userId, int awardCount, bool& succ);

  //增加用户奖励值
	Ice::ObjectSeq incAwardCount(int userId, int inc, bool& succ);

  //减少用户奖励值
	Ice::ObjectSeq decAwardCount(int userId, int dec, bool& succ);

  //设置用户某一位的状态值为1
	Ice::ObjectSeq setOn(int userId, int pos, bool& succ);

  //设置用户某一位的状态值为0
	Ice::ObjectSeq setOff(int userId, int pos, bool& succ);

  //更新用户vip 状态信息
	Ice::ObjectSeq updateVipState(int userId, int state, bool& succ);

  //更新用户vip等级信息
	Ice::ObjectSeq updateVipLevel(int userId, int level, bool& succ);

	int getPrxCount();

	/** 
	 * ScoreLoader invoke this. 
	 **/
   //从数据库重新加载用户的积分信息
        void load(Ice::Int,const Ice::ObjectPtr&);

	void setData(const MyUtil::ObjectResultPtr&);

  //设置mask
	void setMask(long mask);
};

class ScoreLoaderNAdapter : public MyUtil::ReplicatedClusterAdapterI<ScoreLoaderNPrx>, public MyUtil::Singleton<ScoreLoaderNAdapter> {
public:
        //初始化唯一的ScoreLoaderAdapter
        ScoreLoaderNAdapter() :
                MyUtil::ReplicatedClusterAdapterI <ScoreLoaderNPrx>("ControllerScoreLoaderN", 120, 300) {
        }

  //设置用户的登录天数信息
	void setLoginDays(int userId, int historyLoginDays, int continueLoginDays, long lastLoginTime, int loginType );

  //更新用户最后的登录类型信息
	void updateLoginType( int userId, int loginType );

  //设置用户的登录天数信息，并且返回用户的积分信息
	xce::scorecache::ScoreDataN setLoginDaysWithRes(int userId, int historyLoginDays, int continueLoginDays, long lastLoginTime, int loginType );

  //更新用户的登录类型信息，并且返回用乎的积分信息
	xce::scorecache::ScoreDataN updateLoginTypeWithRes( int userId, int loginType );

  //设置用户积分
	void setScore(int userId, int score );

  //增加用户积分
	void incScore(int userId, int inc );

  //减少用户积分
	void decScore(int userId, int dec );

  //设置用户奖励值
	void setAwardCount(int userId, int awardCount );

  //增加用户奖励值
	void incAwardCount(int userId, int inc );

  //减少用户奖励值
	void decAwardCount(int userId, int dec );

  //设置用户pos位的状态为1
	void setOn(int userId, int pos);

  //设置用户pos位的状态为0
	void setOff(int userId, int pos);

  //更新用户vip状态信息
	void updateVipState(int userId, int state);

  //更新用户vip等级信息
	void updateVipLevel(int userId, int level);

	/** 
	 * ScoreLoaderN invoke this. 
	 **/
        //从数据库重新 加载用户的积分信息
        void reload(Ice::Int);

        //从数据库重新加载一批用户的积分信息
        void reloadBatch(const MyUtil::IntSeq&);

  //设置mask
	void setMask(long mask);
};

};
};
};

#endif
