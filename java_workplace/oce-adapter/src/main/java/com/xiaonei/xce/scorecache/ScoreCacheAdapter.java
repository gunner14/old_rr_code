package com.xiaonei.xce.scorecache;

import com.xiaonei.xce.log.Oce;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import xce.scorecache.ScoreDataN;
import xce.scorecache.ScoreCacheNManagerPrx;
import xce.scorecache.ScoreCacheNManagerPrxHelper;
import xce.scorecache.ScoreLoaderNPrx;
import xce.scorecache.ScoreLoaderNPrxHelper;

import xce.util.tools.MathUtil;
import Ice.ConnectTimeoutException;
import Ice.TimeoutException;

import com.xiaonei.xce.log.TimeCost;
import com.xiaonei.xce.log.XceStat;
import com.xiaonei.xce.scorecache.ScoreCache.ScoreSignType;

import xce.clusterstate.ReplicatedClusterAdapter;

public final class ScoreCacheAdapter<T extends ScoreCache> extends
		ReplicatedClusterAdapter {
	private static final String ctr_endpoints = "ControllerScoreCacheN";
	private ScoreCacheFactory<T> _factory;
	private static int _interval = 120;

	private static ScoreCacheAdapter<DefaultScoreCache> _instance = new ScoreCacheAdapter<DefaultScoreCache>(
			new DefaultScoreCacheFactory());

	/***
	 * 初始化模板类型为为DefaultScoreCache的ScoreCacheAdapter
	 * @return 模板对象为DefaultScoreCache的ScoreCacheAdapter的唯一实例
	 */
	public static ScoreCacheAdapter<DefaultScoreCache> getInstance() {
		return _instance;
	}

	/***
	 * 初始化一个模板对象为T的ScoreCacheAdapter
	 * @param 模板对象为T的ScoreCacheAdapter的一个实例
	 */
	public ScoreCacheAdapter(ScoreCacheFactory<T> factory) {
		super(ctr_endpoints, _interval);
		_factory = factory;
	}

	/***
	 * 获取用户积分的相关信息
	 * @param userId 需要获取积分信息的用户id
	 * @return 一个模板对象，其中保存用户积分的相关信息
	 */
	public T getScoreData(int userId) {
		TimeCost cost = TimeCost.begin("getScoreData");
		T o = _factory.createScoreCache();
		ScoreCacheNManagerPrx prx = null;
		try {
			prx = getScoreCacheManagerTwoway(userId);
			if (prx != null) {
				ScoreDataN data = prx.getScoreDataN(userId);
				o.setBase(data);
			} else {
				return null;
			}
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getScoreData [" + prx + "] userId="
							+ userId + " Ice.ConnectTimeoutException");
			o = null;
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getScoreData [" + prx + "] userId="
							+ userId + " Ice.TimeoutException");
			o = null;
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getScoreData [" + prx + "] userId="
							+ userId, e);
			o = null;
		} finally {
			cost.endFinally();
		}
		return o;
	}

	/***
	 * 批量获取用户的积分信息，返回的Map的size可能小于userIds.size(), 因为有的用户的积分信息在服务中可能没有，该方法不回去数据库
	 * 进行加载
	 * @param userIds， 需要获取积分信息的用户ids
	 * @return 一个Map，其中保存了用户id到类型为T的积分信息的映射
	 */
	public Map<Integer, T> getScoreDataMap(List<Integer> userIds) {
		TimeCost cost = TimeCost.begin("GetScoreCacheMap");

		Map<Integer, T> allResults = new HashMap<Integer, T>();
		if (userIds.isEmpty()) {
			XceStat.getLogger().warn(
					this.getClass().getName() + ".getScoreCacheMap "
							+ " idSize=0");
			return allResults;
		}
		Map<Integer, List<Integer>> splitTargets = new HashMap<Integer, List<Integer>>();
		int cluster = getCluster();
		for (int i = 0; i < cluster; ++i) {
			splitTargets.put(i, new ArrayList<Integer>());
		}
		for (int i : userIds) {
			splitTargets.get(i % cluster).add(i);
		}
		for (int i = 0; i < cluster; ++i) {
			List<Integer> subUserIds = splitTargets.get(i);
			if (subUserIds.isEmpty()) {
				continue;
			}
			ScoreCacheNManagerPrx prx = null;
			try {
				Map<Integer, ScoreDataN> subTargets = null;
				prx = getScoreCacheManagerTwoway(i);
				if (prx != null) {
					subTargets = prx.getScoreDataNMap(MathUtil
							.list2array(subUserIds));
				}
				if (subTargets != null) {
					for (Entry<Integer, ScoreDataN> obj : subTargets.entrySet()) {
						T o = _factory.createScoreCache();
						o.setBase(obj.getValue());
						allResults.put(o.getId(), o);
					}
				}
			} catch (ConnectTimeoutException e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".getScoreCacheMap [" + prx + "]"
								+ " idSize=" + subUserIds.size()
								+ " Ice.ConnectTimeoutException");
			} catch (TimeoutException e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".getScoreCacheMap [" + prx + "]"
								+ " idSize=" + subUserIds.size()
								+ " Ice.TimeoutException");
			} catch (java.lang.Throwable e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".getScoreCacheMap [" + prx + "]"
								+ " idSize=" + subUserIds.size(), e);
			} finally {
				cost.endFinally();
			}
		}
		return allResults;
	}

	/***
	 * 获取用户的积分信息，如果系统中没有该用户的积分信息，那么去相应的数据库进行加载
	 * @param userId， 需要获取积分信息的用户id
	 * @return 一个类型为T的用户的积分信息
	 */
	public T getScoreDataNotNull(int userId) {
		TimeCost cost = TimeCost.begin("getScoreDataNotNull");
		T o = _factory.createScoreCache();
		ScoreCacheNManagerPrx prx = null;
		try {
			prx = getScoreCacheManagerTwoway(userId);
			if (prx != null) {
				ScoreDataN data = prx.getScoreDataNNotNull(userId);
				o.setBase(data);
			} else {
				return null;
			}
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getScoreDataNotNull [" + prx + "] userId="
							+ userId + " Ice.ConnectTimeoutException");
			o = null;
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getScoreDataNotNull [" + prx + "] userId="
							+ userId + " Ice.TimeoutException");
			o = null;
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".getScoreDataNotNull [" + prx + "] userId="
							+ userId, e);
			o = null;
		} finally {
			cost.endFinally();
		}
		return o;
	}

	/***
	 * 批量获取用户的积分信息，如果用户信息在当前Cache中没有， 那么系统会去数据库进行加载
	 * @param userIds， 需要获取积分信息的用户ids
	 * @return 一个Map, 其中保存从用户id到类型为T的积分信息的映射关系
	 */
	public Map<Integer, T> getScoreDataMapNotNull(List<Integer> userIds) {
		TimeCost cost = TimeCost.begin("GetScoreCacheMapNotNull");

		Map<Integer, T> allResults = new HashMap<Integer, T>();
		if (userIds.isEmpty()) {
			XceStat.getLogger().warn(
					this.getClass().getName() + ".getScoreCacheMapNotNull "
							+ " idSize=0");
			return allResults;
		}
		Map<Integer, List<Integer>> splitTargets = new HashMap<Integer, List<Integer>>();
		int cluster = getCluster();
		for (int i = 0; i < cluster; ++i) {
			splitTargets.put(i, new ArrayList<Integer>());
		}
		for (int i : userIds) {
			splitTargets.get(i % cluster).add(i);
		}
		for (int i = 0; i < cluster; ++i) {
			List<Integer> subUserIds = splitTargets.get(i);
			if (subUserIds.isEmpty()) {
				continue;
			}
			ScoreCacheNManagerPrx prx = null;
			try {
				Map<Integer, ScoreDataN> subTargets = null;
				prx = getScoreCacheManagerTwoway(i);
				if (prx != null) {
					subTargets = prx.getScoreDataNMapNotNull(MathUtil
							.list2array(subUserIds));
				}
				if (subTargets != null) {
					for (Entry<Integer, ScoreDataN> obj : subTargets.entrySet()) {
						T o = _factory.createScoreCache();
						o.setBase(obj.getValue());
						allResults.put(o.getId(), o);
					}
				}
			} catch (ConnectTimeoutException e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".getScoreCacheMapNotNull [" + prx + "]"
								+ " idSize=" + subUserIds.size()
								+ " Ice.ConnectTimeoutException");
			} catch (TimeoutException e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".getScoreCacheMapNotNull [" + prx + "]"
								+ " idSize=" + subUserIds.size()
								+ " Ice.TimeoutException");
			} catch (java.lang.Throwable e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".getScoreCacheMapNotNull [" + prx + "]"
								+ " idSize=" + subUserIds.size(), e);
			} finally {
				cost.endFinally();
			}
		}
		return allResults;
	}

	/***
	 * 批量获取用户的积分信息，以List的形式返回， 返回List的size可能小于userIds.size, 因为有的用户积分信息在当前的
	 * cache中可能没有
	 * @param userIds， 需要获取积分信息的用户ids
	 * @return 一个List，其中保存类型为T的用户积分信息
	 */
	public List<T> getScoreCacheList(List<Integer> userIds) {
		/* re-order the result from map */
		TimeCost cost = TimeCost
				.begin(XceStat.getLogger(), "GetScoreCacheList");
		Map<Integer, T> allResults = getScoreDataMap(userIds);
		List<T> result = new ArrayList<T>();
		for (int userId : userIds) {
			T obj = allResults.get(userId);
			if (obj == null)
				continue;
			result.add(obj);
		}
		cost.endFinally();
		return result;
	}

	/***
	 * 设置用户的登录信息
	 * @param userId， 需要设置登录信息的用户id
	 * @param historyLoginDays, 历史登录天数，用户一共登录了多少次
	 * @param continueLoginDays, 当前连续登录天数
	 * @param lastLoginTime， 最后一次登录时间
	 * @param loginType， 最后一次登录类型
	 */
	public void setLoginDays(int userId, int historyLoginDays,
			int continueLoginDays, long lastLoginTime, int loginType) {
		ScoreLoaderAdapter.getInstance().setLoginDays(userId, historyLoginDays,
				continueLoginDays, lastLoginTime, loginType);
	}

	/***
	 * 更新用户最后的登录类型
	 * @param userId， 需要更新登录类型的用户id
	 * @param loginType, 登录类型
	 */
	public void updateLoginType(int userId, int loginType) {
		ScoreLoaderAdapter.getInstance().updateLoginType(userId, loginType);
	}

	/***
	 * 设置用户的登录信息，并返回积分信息
	 * @param userId， 要操作的用户id
	 * @param historyLoginDays, 历史登录天数
	 * @param continueLoginDays， 连续登录次数
	 * @param lastLoginTime，最后登录时间
	 * @param loginType， 最后登录类型
	 * @return， 用户的积分信息
	 */
	public DefaultScoreCache setLoginDaysWithRes(int userId, int historyLoginDays,
			int continueLoginDays, long lastLoginTime, int loginType) {
		return ScoreLoaderAdapter.getInstance().setLoginDaysWithRes(userId, historyLoginDays,
				continueLoginDays, lastLoginTime, loginType);
	}

	/***
	 * 更新用户的最后登录类型，并返回积分信息
	 * @param userId，需要更新信息的用户id
	 * @param loginType, 登录类型
	 * @return 该用户的积分信息
	 */
	public DefaultScoreCache updateLoginTypeWithRes(int userId, int loginType) {
		return ScoreLoaderAdapter.getInstance().updateLoginTypeWithRes(userId, loginType);
	}

	/***
	 * 增加用户积分
	 * @param userId， 需要增加积分的用户id
	 * @param inc,  积分增加值
	 */
	public void incScore(int userId, int inc) {
		ScoreLoaderAdapter.getInstance().incScore(userId, inc);
	}

	/***
	 * 设置用户积分
	 * @param userId， 需要设置积分的用户id
	 * @param score, 新的积分
	 */
	public void setScore(int userId, int score) {
		ScoreLoaderAdapter.getInstance().setScore(userId, score);
	}

	/***
	 * 减少用户积分
	 * @param userId， 需要减少积分的用户id
	 * @param dec, 减少的积分值
	 */
	public void decScore(int userId, int dec) {
		ScoreLoaderAdapter.getInstance().decScore(userId, dec);
	}

	/***
	 * 增加用户的奖励值
	 * @param userId，需要增加奖励的用户id
	 * @param inc, 奖励的增加值
	 */
	public void incAwardCount(int userId, int inc) {
		ScoreLoaderAdapter.getInstance().incAwardCount(userId, inc);
	}

	/***
	 * 设置用户的奖励值
	 * @param userId，需要设置奖励的用户id
	 * @param awardCount, 要设置的奖励值
	 */
	public void setAwardCount(int userId, int awardCount) {
		ScoreLoaderAdapter.getInstance().setAwardCount(userId, awardCount);
	}

	/***
	 * 减少用户的奖励值
	 * @param userId， 需要减少奖励值的用户id
	 * @param dec, 减少的奖励值
	 */
	public void decAwardCount(int userId, int dec) {
		ScoreLoaderAdapter.getInstance().decAwardCount(userId, dec);
	}

	/***
	 * 将用户的type的类型设置为1
	 * @param userId， 需要设置ScoreSignType的用户id
	 * @param type， 要设置的ScoreSignType
	 */
	public void setOn(int userId, ScoreSignType type) {
		ScoreLoaderAdapter.getInstance().setOn(userId, type);
	}

	/***
	 * 将用户的ScoreSignType type设置为0
	 * @param userId， 需要设置ScoreSignType的用户id
	 * @param type, 要设置的ScoreSignType
	 */
	public void setOff(int userId, ScoreSignType type) {
		ScoreLoaderAdapter.getInstance().setOff(userId, type);
	}

	/***
	 * 设置用户为最新升级与否
	 * @param userId， 要设置最新升级的用户id
	 * @param on, 最新升级与否
	 */
	public void setNewlyUpgrade(int userId, boolean on) {
		ScoreLoaderAdapter.getInstance().setNewlyUpgrade(userId, on);
	}

	/***
	 * 设置用户为双倍积分
	 * @param userId， 要设置双倍积分的用户id
	 * @param on, 是否设置为双倍积分
	 */
	public void setDoubleScore(int userId, boolean on) {
		ScoreLoaderAdapter.getInstance().setDoubleScore(userId, on);
	}

	/***
	 * 设置用户的掩码
	 * @param mask，要设置的掩码值
	 */
	public void setMask(long mask) {
		ScoreLoaderAdapter.getInstance().setMask(mask);
	}

	protected ScoreCacheNManagerPrx getScoreCacheManagerTwoway(int userId) {
		Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
		if (prx0 != null) {
			ScoreCacheNManagerPrx prx = ScoreCacheNManagerPrxHelper
					.uncheckedCast(prx0.ice_timeout(300).ice_twoway());
			return prx;
		} else {
			return null;
		}
	}

	public static void main(String[] args) {
		try {
			ScoreCacheAdapter<DefaultScoreCache> adapter = new ScoreCacheAdapter<DefaultScoreCache>(
					new DefaultScoreCacheFactory());
			int[] ids = {702613154,263873238,702621036,263875730,702918267,264259809,702613267,263875152};
			List<Integer> userIds = new ArrayList<Integer>();
			for( int i: ids){
				userIds.add(i);
//				adapter.setOn(i, ScoreSignType.TYPE_NO_FOOTPRINT);
			}
			Map<Integer, DefaultScoreCache> a = adapter.getScoreDataMap(userIds);
			for( Entry<Integer,DefaultScoreCache> one : a.entrySet() ){
				System.out.println(one.toString());
			}
//			int userId = 24496255;
//			 DefaultScoreCache ret = adapter.getScoreData(userId);
//			 System.out.println(ret.toString());
//			 
//			 adapter.incAwardCount(userId, 2);
//			 adapter.incScore(userId, 2);
//			 adapter.decAwardCount(userId, 1);
//			 adapter.decScore(userId, 1);
//
//			 adapter.setOn(userId, ScoreSignType.TYPE_FOOTPRINT_REMOVABLE);
//			 adapter.setOn(userId, ScoreSignType.TYPE_COLOURFUL_TALKBOX);
//			 Thread.sleep(500);
//			 adapter.setOff(userId, ScoreSignType.TYPE_COLOURFUL_TALKBOX);
//
//			 Thread.sleep(1000);
//			 ret = adapter.getScoreData(userId);
//			 System.out.println(ret.toString());
			 
//			List<Integer> ids = new ArrayList<Integer>();
//			ids.add(userId);
//			Map<Integer, DefaultScoreCache> map = adapter.getScoreDataMap(ids);
//			for (Entry<Integer, DefaultScoreCache> entry : map.entrySet()) {
//				System.out.println(entry.toString());
//			}
		} catch (Exception e) {
			Oce.getLogger().error(e);
		}
		System.exit(0);
	}
}

class ScoreLoaderAdapter<T extends ScoreCache> extends ReplicatedClusterAdapter {
	private static final String ctr_endpoints = "ControllerScoreLoaderN";
	private ScoreCacheFactory<T> _factory;
	private static int _interval = 120;

	private ScoreLoaderAdapter(ScoreCacheFactory<T> factory) {
		super(ctr_endpoints, _interval);
		_factory = factory;
	}

	private static ScoreLoaderAdapter<DefaultScoreCache> _instance = new ScoreLoaderAdapter<DefaultScoreCache>(
			new DefaultScoreCacheFactory());

	/***
	 * 返回模板类为DefaultScoreCache的ScoreLoaderAdapter
	 * @return 模板类型为DefaultScoreCache的ScoreLoaderAdapter的唯一实例
	 */
	public static ScoreLoaderAdapter<DefaultScoreCache> getInstance() {
		return _instance;
	}

	/***
	 * 设置用户登录信息
	 * @param userId， 要设置登录信息的用户id
	 * @param historyLoginDays, 历史登录天数
	 * @param continueLoginDays， 连续登录天数
	 * @param lastLoginTime， 最后登录时间
	 * @param loginType， 最后登录类型
	 */
	public void setLoginDays(int userId, int historyLoginDays,
			int continueLoginDays, long lastLoginTime, int loginType) {
		try {
			getScoreLoaderOneway(userId).setLoginDays(userId, historyLoginDays,
					continueLoginDays, lastLoginTime, loginType);

		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setLoginDays " + " userId="
							+ userId + " historyLoginDays=" + historyLoginDays
							+ " continueLoginDays=" + continueLoginDays
							+ " lastLoginTime=" + lastLoginTime + " loginType="
							+ loginType + " Ice.ConnectTimeoutException");
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setLoginDays " + " userId="
							+ userId + " historyLoginDays=" + historyLoginDays
							+ " continueLoginDays=" + continueLoginDays
							+ " lastLoginTime=" + lastLoginTime + " loginType="
							+ loginType + " Ice.TimeoutException");
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setLoginDays " + " userId="
							+ userId + " historyLoginDays=" + historyLoginDays
							+ " continueLoginDays=" + continueLoginDays
							+ " lastLoginTime=" + lastLoginTime + " loginType="
							+ loginType, e);
		}
	}

	/***
	 * 更新用户最后的登录类型
	 * @param userId， 要更新登录类型的用户id
	 * @param loginType, 新的登录类型
	 */
	public void updateLoginType(int userId, int loginType) {
		try {
			getScoreLoaderOneway(userId).updateLoginType(userId, loginType);
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".updateLoginType "
							+ " userId=" + userId + " loginType=" + loginType
							+ " Ice.ConnectTimeoutException");
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".updateLoginType "
							+ " userId=" + userId + " loginType=" + loginType
							+ " Ice.TimeoutException");
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".updateLoginType "
							+ " userId=" + userId + " loginType=" + loginType,
					e);
		}
	}

	/***
	 * 设置用户的登录信息，并返回用户的积分信息
	 * @param userId， 要操作的用户id
	 * @param historyLoginDays， 历史登录天数
	 * @param continueLoginDays， 连续登录天数
	 * @param lastLoginTime， 最后登录时间
	 * @param loginType， 最后登录类型
	 * @return 该用户的积分信息
	 */
	public T setLoginDaysWithRes(int userId, int historyLoginDays,
			int continueLoginDays, long lastLoginTime, int loginType) {
		T o = _factory.createScoreCache();
		try {
			ScoreDataN data = getScoreLoaderTwoway(userId).setLoginDaysWithRes(userId, historyLoginDays,
					continueLoginDays, lastLoginTime, loginType);
			o.setBase(data);
			return o;
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setLoginDaysWithRes " + " userId="
							+ userId + " historyLoginDays=" + historyLoginDays
							+ " continueLoginDays=" + continueLoginDays
							+ " lastLoginTime=" + lastLoginTime + " loginType="
							+ loginType + " Ice.ConnectTimeoutException");
			throw e;
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setLoginDaysWithRes " + " userId="
							+ userId + " historyLoginDays=" + historyLoginDays
							+ " continueLoginDays=" + continueLoginDays
							+ " lastLoginTime=" + lastLoginTime + " loginType="
							+ loginType + " Ice.TimeoutException");
			throw e;
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setLoginDaysWithRes " + " userId="
							+ userId + " historyLoginDays=" + historyLoginDays
							+ " continueLoginDays=" + continueLoginDays
							+ " lastLoginTime=" + lastLoginTime + " loginType="
							+ loginType, e);
			throw new RuntimeException(e);
		}
	}

	/***
	 * 更新用户的最后登录类型，并返回用户的积分信息
	 * @param userId，要操作的用户id
	 * @param loginType, 新的登录类型
	 * @return 该用户的积分信息
	 */
	public T updateLoginTypeWithRes(int userId, int loginType) {
		T o = _factory.createScoreCache();
		try {
			ScoreDataN data = getScoreLoaderTwoway(userId).updateLoginTypeWithRes(userId, loginType);
			o.setBase(data);
			return o;
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".updateLoginTypeWithRes "
							+ " userId=" + userId + " loginType=" + loginType
							+ " Ice.ConnectTimeoutException");
			throw e;
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".updateLoginTypeWithRes "
							+ " userId=" + userId + " loginType=" + loginType
							+ " Ice.TimeoutException");
			throw e;
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".updateLoginTypeWithRes "
							+ " userId=" + userId + " loginType=" + loginType,
					e);
			throw new RuntimeException(e);
		}
	}

	/***
	 * 增加一个用户的积分
	 * @param userId， 要增加积分的用户
	 * @param inc， 积分增加值
	 */
	public void incScore(int userId, int inc) {
		try {
			getScoreLoaderOneway(userId).incScore(userId, inc);
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".incScore " + " userId="
							+ userId + " inc=" + inc
							+ " Ice.ConnectTimeoutException");
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".incScore " + " userId="
							+ userId + " inc=" + inc + " Ice.TimeoutException");
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".incScore " + " userId="
							+ userId + " inc=" + inc, e);
		}
	}

	/***
	 * 设置用户的积分值
	 * @param userId， 要设置积分值的用户id
	 * @param score， 新的积分值
	 */
	public void setScore(int userId, int score) {
		try {
			getScoreLoaderOneway(userId).setScore(userId, score);
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setScore " + " userId="
							+ userId + " score=" + score
							+ " Ice.ConnectTimeoutException");
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setScore " + " userId="
							+ userId + " score=" + score + " Ice.TimeoutException");
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setScore " + " userId="
							+ userId + " score=" + score, e);
		}
	}

	/***
	 * 减少用户的积分值
	 * @param userId，要减少积分值的用户id
	 * @param dec, 减少的积分值
	 */
	public void decScore(int userId, int dec) {
		try {
			getScoreLoaderOneway(userId).decScore(userId, dec);
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".decScore " + " userId="
							+ userId + " dec=" + dec
							+ " Ice.ConnectTimeoutException");
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".decScore " + " userId="
							+ userId + " dec=" + dec + " Ice.TimeoutException");
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".decScore " + " userId="
							+ userId + " dec=" + dec, e);
		}
	}

	/***
	 * 设置用户的奖励值
	 * @param userId，要设置奖励值的用户
	 * @param awardCount， 新的奖励值
	 */
	public void setAwardCount(int userId, int awardCount) {
		try {
			getScoreLoaderOneway(userId).setAwardCount(userId, awardCount);
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setAwardCount " + " userId="
							+ userId + " awardCount=" + awardCount
							+ " Ice.ConnectTimeoutException");
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setAwardCount " + " userId="
							+ userId + " awardCount=" + awardCount + " Ice.TimeoutException");
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setAwardCount " + " userId="
							+ userId + " awardCount=" + awardCount, e);
		}
	}

	/***
	 * 增加用户的奖励值
	 * @param userId，要增加奖励值的用户
	 * @param inc，增加的奖励值
	 */
	public void incAwardCount(int userId, int inc) {
		try {
			getScoreLoaderOneway(userId).incAwardCount(userId, inc);
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".incAwardCount " + " userId="
							+ userId + " inc=" + inc
							+ " Ice.ConnectTimeoutException");
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".incAwardCount " + " userId="
							+ userId + " inc=" + inc + " Ice.TimeoutException");
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".incAwardCount " + " userId="
							+ userId + " inc=" + inc, e);
		}
	}

	/***
	 * 减少用户的奖励值
	 * @param userId，要减少奖励值的用户
	 * @param dec，减少的奖励值
	 */
	public void decAwardCount(int userId, int dec) {
		try {
			getScoreLoaderOneway(userId).decAwardCount(userId, dec);
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".decAwardCount " + " userId="
							+ userId + " dec=" + dec
							+ " Ice.ConnectTimeoutException");
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".decAwardCount " + " userId="
							+ userId + " dec=" + dec + " Ice.TimeoutException");
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".decAwardCount " + " userId="
							+ userId + " dec=" + dec, e);
		}
	}

	/***
	 * 设置用户ScoreSignType类型为1
	 * @param userId, 要设置ScoreSignType的用户id
	 * @param type, ScoreSignType的类型
	 */
	public void setOn(int userId, ScoreSignType type) {
		try {
			getScoreLoaderOneway(userId).setOn(userId, type.type);
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setOn " + " userId=" + userId
							+ " type=" + type.name()
							+ " Ice.ConnectTimeoutException");
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setOn " + " userId=" + userId
							+ " type=" + type.name() + " Ice.TimeoutException");
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setOn " + " userId=" + userId
							+ " type=" + type.name(), e);
		}
	}

	/***
	 * 设置用户ScoreSignType为0
	 * @param userId， 要设置ScoreSignType的用户id
	 * @param type, 要设置的ScoreSignType
	 */
	public void setOff(int userId, ScoreSignType type) {
		try {
			getScoreLoaderOneway(userId).setOff(userId, type.type);
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setOff " + " userId="
							+ userId + " type=" + type.name()
							+ " Ice.ConnectTimeoutException");
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setOff " + " userId="
							+ userId + " type=" + type.name()
							+ " Ice.TimeoutException");
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setOff " + " userId="
							+ userId + " type=" + type.name(), e);
		}
	}

	/***
	 * 设置用户为新升级用户
	 * @param userId， 要设置新升级的用户id
	 * @param on, 是否是新升级的用户
	 */
	public void setNewlyUpgrade(int userId,boolean on) {
		if( on ){
			try {
				getScoreLoaderOneway(userId).setOn(userId, 0);
			} catch (ConnectTimeoutException e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".setOn " + " userId=" + userId
						+ " type=" + 0
						+ " Ice.ConnectTimeoutException");
			} catch (TimeoutException e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".setOn " + " userId=" + userId
						+ " type=" + 0 + " Ice.TimeoutException");
			} catch (java.lang.Throwable e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".setOn " + " userId=" + userId
						+ " type=" + 0, e);
			}
		} else{
			try {
				getScoreLoaderOneway(userId).setOff(userId, 0);
			} catch (ConnectTimeoutException e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".setOff " + " userId=" + userId
						+ " type=" + 0
						+ " Ice.ConnectTimeoutException");
			} catch (TimeoutException e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".setOff " + " userId=" + userId
						+ " type=" + 0 + " Ice.TimeoutException");
			} catch (java.lang.Throwable e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".setOff " + " userId=" + userId
						+ " type=" + 0, e);
			}
		}
	}

	/***
	 * 设置用户积分双倍增长与否
	 * @param userId， 要设置双倍增长与否的用户id
	 * @param on, 是否设置为双倍增长
	 */
	public void setDoubleScore(int userId,boolean on) {
		if( on ){
			try {
				getScoreLoaderOneway(userId).setOn(userId, 62);
			} catch (ConnectTimeoutException e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".setOn " + " userId=" + userId
						+ " type=" + 62
						+ " Ice.ConnectTimeoutException");
			} catch (TimeoutException e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".setOn " + " userId=" + userId
						+ " type=" + 62 + " Ice.TimeoutException");
			} catch (java.lang.Throwable e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".setOn " + " userId=" + userId
						+ " type=" + 62, e);
			}
		} else{
			try {
				getScoreLoaderOneway(userId).setOff(userId, 62);
			} catch (ConnectTimeoutException e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".setOff " + " userId=" + userId
						+ " type=" + 62
						+ " Ice.ConnectTimeoutException");
			} catch (TimeoutException e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".setOff " + " userId=" + userId
						+ " type=" + 62 + " Ice.TimeoutException");
			} catch (java.lang.Throwable e) {
				Oce.getLogger().error(
						this.getClass().getName() + ".setOff " + " userId=" + userId
						+ " type=" + 62, e);
			}
		}
	}

	/***
	 * 设置用户掩码
	 * @param mask， 新的掩码
	 */
	public void setMask(long mask) {
		try {
			getScoreLoaderOneway(0).setMask(mask);
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setMask " + " mask=" + mask
							+ " Ice.ConnectTimeoutException");
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setMask " + " mask=" + mask
							+ " Ice.TimeoutException");
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".setMask " + " mask=" + mask,
					e);
		}
	}

	/***
	 * 从数据库重新加载用户的积分信息
	 * @param userId， 需要重新加载积分信息的用户id
	 */
	public void reload(int userId) {
		try {
			getScoreLoaderOneway(userId).reload(userId);
		} catch (ConnectTimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".reload " + " userId="
							+ userId + " Ice.ConnectTimeoutException");
		} catch (TimeoutException e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".reload " + " userId="
							+ userId + " Ice.TimeoutException");
		} catch (java.lang.Throwable e) {
			Oce.getLogger().error(
					this.getClass().getName() + ".reload " + " userId="
							+ userId, e);
		}
	}

	protected ScoreLoaderNPrx getScoreLoaderOneway(int userId) {
		Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
		if (prx0 != null) {
			ScoreLoaderNPrx prx = ScoreLoaderNPrxHelper.uncheckedCast(prx0
					.ice_oneway());
			return prx;
		} else {
			return null;
		}
	}
	protected ScoreLoaderNPrx getScoreLoaderTwoway(int userId) {
		Ice.ObjectPrx prx0 = getValidProxy(userId, 3000);
		if (prx0 != null) {
			ScoreLoaderNPrx prx = ScoreLoaderNPrxHelper
					.uncheckedCast(prx0.ice_timeout(3000).ice_twoway());
			return prx;
		} else {
			return null;
		}
	}
}
