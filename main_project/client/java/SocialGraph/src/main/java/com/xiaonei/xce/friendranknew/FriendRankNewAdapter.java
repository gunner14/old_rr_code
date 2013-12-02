package com.xiaonei.xce.friendranknew;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.FriendRankCacheNewPrx;
import xce.socialgraph.FriendRankCacheNewPrxHelper;
import xce.socialgraph.RankDataNew;
import xce.socialgraph.RankDataInt;
import xce.util.channel.Channel;

import com.xiaonei.xce.buddybyidcache.BuddyByIdCacheAdapter;

import com.xiaonei.xce.log.Oce;

public class FriendRankNewAdapter extends ReplicatedClusterAdapter {
	private static FriendRankNewAdapter instance = null;
	private static final String ctrEndpoints = "ControllerFriendRankCacheNew";
	private static int interval = 120;
	private FriendRankNewAdapter() {
		super(ctrEndpoints, interval, Channel.newSocialGraphChannel(ctrEndpoints));
	}
	
	public static FriendRankNewAdapter getInstance() {
		if(null == instance) {
			 synchronized (FriendRankNewAdapter.class) {
				 if (null == instance) {
						try {
							instance = new FriendRankNewAdapter();
						} catch(Exception e) {
							e.printStackTrace();
							instance = null;
						}
					}
			 }
		}
		return instance;
	}

	protected FriendRankCacheNewPrx getFriendRankManager(int userId){
		Ice.ObjectPrx prx0 = getValidProxy(userId, 1000);
		if ( prx0 != null ) {
			FriendRankCacheNewPrx prx = FriendRankCacheNewPrxHelper.uncheckedCast(prx0.ice_timeout(1000).ice_twoway());
			return prx;
		} else {
			return null;
		}
	}
	
	protected FriendRankCacheNewPrx getFriendRankCacheOneWay(int userId) {
		Ice.ObjectPrx prx0 = getValidProxy(userId, 1000);
		if ( prx0 != null ) {
			FriendRankCacheNewPrx prx = FriendRankCacheNewPrxHelper.uncheckedCast(prx0.ice_timeout(1000).ice_oneway());
			return prx;
		} else {
			return null;
		}
	}
	public List<RankDataNew> getFriendRankById(int userId) {
		FriendRankCacheNewPrx prx= null;
		try{
			prx= getFriendRankManager(userId);
			RankDataNew[] ret = prx.GetRank(userId);

			List<RankDataNew> listRank = new ArrayList<RankDataNew>();
			for(RankDataNew one : ret) {
//				RankDataNew obj;
				listRank.add(one);
			}
			return listRank;
		} catch (Ice.ConnectTimeoutException e) {
			Oce.getLogger().error(this.getClass().getName() + ".getFriendRankFromCache [ " + prx + " ] id=" + userId + " Ice.ConnectTimeoutException");
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(this.getClass().getName() + ".getFriendRankFromCache [ " + prx + " ] id=" + userId + " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(this.getClass().getName() +  " getFriendRankFromCache " + e);
			//e.printStackTrace();
		}
		return null;
	}
	
	public List<RankDataInt> getFriendRankWithFactor(int userId, float factor) {
		FriendRankCacheNewPrx prx= null;
		try{
			prx= getFriendRankManager(userId);
			RankDataInt[] ret = prx.GetRankWithFactor(userId, factor);

			List<RankDataInt> listRank = new ArrayList<RankDataInt>();
			for(RankDataInt one : ret) {
//				RankDataNew obj;
				listRank.add(one);
			}
			return listRank;
		} catch (Ice.ConnectTimeoutException e) {
			Oce.getLogger().error(this.getClass().getName() + ".getFriendRankWithFactor [ " + prx + " ] id=" + userId + " Ice.ConnectTimeoutException");
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(this.getClass().getName() + ".getFriendRankWithFactor [ " + prx + " ] id=" + userId + " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(this.getClass().getName() +  " getFriendRankWithFactor " + e);
			//e.printStackTrace();
		}
		return null;
	}
	
	public List<RankDataNew> getFriendRank(int userId) {
		FriendRankCacheNewPrx prx= null;
		try{
			prx= getFriendRankManager(userId);
			RankDataNew[] ret = prx.GetRank(userId);

			List<RankDataNew> listRank = new ArrayList<RankDataNew>();
			for(RankDataNew one : ret) {
//				RankDataNew obj;
				listRank.add(one);
			}
			return listRank;
		} catch (Ice.ConnectTimeoutException e) {
			Oce.getLogger().error(this.getClass().getName() + ".getFriendRankWithFactor [ " + prx + " ] id=" + userId + " Ice.ConnectTimeoutException");
		} catch (Ice.TimeoutException e) {
			Oce.getLogger().error(this.getClass().getName() + ".getFriendRankWithFactor [ " + prx + " ] id=" + userId + " Ice.TimeoutException");
		} catch (Exception e) {
			Oce.getLogger().error(this.getClass().getName() +  " getFriendRankWithFactor " + e);
			//e.printStackTrace();
		}
		return null;
	}
	 public static void main(String[] args) {
		 	int userId = 232491085;  
			FriendRankNewAdapter adapter = new FriendRankNewAdapter();
			float f = 2.3f;
			List<RankDataInt> listRank = adapter.getFriendRankWithFactor(userId,f);
			if(listRank.size() >= 0){
				System.out.println("您关注的好友，按亲密度降序排列：");
				for(RankDataInt r : listRank){
					System.out.print("(" + r.fid + ":" + r.score + ")");
					System.out.println();
				}
			}
			System.out.println();
			
			List<RankDataNew> listRank2 = adapter.getFriendRank(userId);
			if(listRank2.size() >= 0){
				System.out.println("您关注的好友，按亲密度降序排列：");
				System.out.println("size:" + listRank2.size());
				for(RankDataNew r : listRank2){
					System.out.print("(" + r.fid + ":" + r.staticScore + "," + r.activeScore +")");
					System.out.println();
				}
			}
			System.out.println();
			System.exit(0);
		}
}