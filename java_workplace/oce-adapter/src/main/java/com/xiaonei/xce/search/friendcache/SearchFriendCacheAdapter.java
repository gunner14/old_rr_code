package com.xiaonei.xce.search.friendcache;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Vector;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.generalsuggestion.SearchManagerPrx;
import xce.generalsuggestion.SearchManagerPrxHelper;
import xce.util.channel.Channel;

import com.renren.search.friendcache.SearchFriendCachePrx;
import com.renren.search.friendcache.SearchFriendCachePrxHelper;

//import com.xiaonei.search.namecheck.SearchFriendCachePrx;
//import com.xiaonei.search.namecheck.SearchFriendCachePrxHelper;

public class SearchFriendCacheAdapter extends ReplicatedClusterAdapter {

	public static final int THREAD_SIZE = 8;

	// private static final String sil_endpoints =
	// "ControllerSearchRelationLogic";
	private static final String sil_endpoints = "ControllerSearchFriendCache";

	private static int _interval = 120;

	private Vector<SearchFriendCachePrx> managers = new Vector<SearchFriendCachePrx>();

	private SearchFriendCacheAdapter() {

		super(sil_endpoints, _interval, Channel
				.newSearchControllerChannel(sil_endpoints));

	}

	private static SearchFriendCacheAdapter _instance = new SearchFriendCacheAdapter();

	public static SearchFriendCacheAdapter getInstance() {
		return _instance;
	}

	protected SearchFriendCachePrx getSearchNameCheckManagerOneway(int index) {
		// Ice.ObjectPrx prx0 = getValidProxy(index, 300);
		@SuppressWarnings("unchecked")
		List<Ice.ObjectPrx> prx1 = getAllProxy(index);
		System.out.println("proxy size: " + prx1.size());
		int i = 0;
		for (Ice.ObjectPrx tmp : prx1) {
			System.out.println("No " + i + " proxy: " + tmp);
			i++;
		}
		if (prx1 != null && prx1.size() > 0) {
			SearchFriendCachePrx prx = SearchFriendCachePrxHelper.uncheckedCast(prx1.get(0)
					.ice_timeout(300).ice_oneway());
			return prx;
		} else {
			return null;
		}
	}

	protected SearchFriendCachePrx getSearchNameCheckManagerTwoway(int uid) {
		// Ice.ObjectPrx prx0 = getValidProxy(index, 300);
		// @SuppressWarnings("unchecked")
		int index = uid % this.getCluster();
		// List<Ice.ObjectPrx> prx1 = getAllProxy(index);
		Ice.ObjectPrx prx0 = getValidProxy(index, 300);
		if (prx0 != null) {
			SearchFriendCachePrx prx = SearchFriendCachePrxHelper.uncheckedCast(prx0
					.ice_timeout(500));
			return prx;
		}
		System.out.println("prx0 is null");
		return null;
	}

//	public Map<Integer,Integer> GetSecond(int uid) {
//		try {
//			SearchFriendCachePrx tmp = getSearchNameCheckManagerTwoway(uid);
//			if (tmp != null) {
//				return tmp.GetSecond(uid);
//			} else {
//				return new HashMap<Integer,Integer>();
//			}
//		} catch (Exception e) {
//			e.printStackTrace();
//			return new HashMap<Integer,Integer>();
//		}
//	}
	
	public int[] GetSecondFriends(int uid) {
		try {
			int idx = uid / 10 % 10;//按照十位散
			SearchFriendCachePrx tmp = getSearchNameCheckManagerTwoway(idx);
			if (tmp != null) {
				return tmp.GetSecondFriends(uid);
			} else {
				return new int[0];
			}
		} catch (Exception e) {
			e.printStackTrace();
			return new int[0];
		}
	}

	public static void main(String[] args) {
//		Map<Integer,Integer> res= SearchFriendCacheAdapter.getInstance().GetSecond(220678672);
//		System.out.println("res:" + res);
//		try {
//			Thread.sleep(100);
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
	}
}
