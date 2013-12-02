package com.xiaonei.xce.search;

import java.util.List;
import java.util.Random;
import java.util.Vector;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.generalsuggestion.SearchManagerPrx;
import xce.generalsuggestion.SearchManagerPrxHelper;
import xce.util.channel.Channel;

import com.xiaonei.search.namecheck.NameCheckerPrx;
import com.xiaonei.search.namecheck.NameCheckerPrxHelper;
import com.xiaonei.search.namecheck.queryResult;
import com.xiaonei.search.namecheck.struNCResult;

public class SearchNameCheckAdapter extends ReplicatedClusterAdapter {

	public static final int THREAD_SIZE = 8;

	// private static final String sil_endpoints =
	// "ControllerSearchRelationLogic";
	private static final String sil_endpoints = "ControllerNameCheck";

	private static int _interval = 120;

	private Vector<NameCheckerPrx> managers = new Vector<NameCheckerPrx>();

	private SearchNameCheckAdapter() {

		super(sil_endpoints, _interval, Channel
				.newGadgetsChannel(sil_endpoints));
//		super(sil_endpoints, _interval, Channel
//				.newSearchChannel(sil_endpoints));

	}

	private static SearchNameCheckAdapter _instance = new SearchNameCheckAdapter();

	public static SearchNameCheckAdapter getInstance() {
		return _instance;
	}

	protected NameCheckerPrx getSearchNameCheckManagerOneway(int index) {
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
			NameCheckerPrx prx = NameCheckerPrxHelper.uncheckedCast(prx1.get(0)
					.ice_timeout(300).ice_oneway());
			return prx;
		} else {
			return null;
		}
	}

	protected NameCheckerPrx getSearchNameCheckManagerTwoway(int index) {
		// Ice.ObjectPrx prx0 = getValidProxy(index, 300);
		// @SuppressWarnings("unchecked")
		Random r = new Random();
		index = r.nextInt(getCluster());
		System.out.println("index:" + index + " cluster:" + getCluster());
		// List<Ice.ObjectPrx> prx1 = getAllProxy(index);
		
		Ice.ObjectPrx prx0 = getValidProxy(index, 300);
		if (prx0 != null) {
			NameCheckerPrx prx = NameCheckerPrxHelper.uncheckedCast(prx0
					.ice_timeout(300));
			return prx;
		}

		return null;
		// System.out.println("proxy size: " + prx1.size());
		// int i = 0;
		// for (Ice.ObjectPrx tmp : prx1) {
		// System.out.println("No " + i + " proxy: " + tmp);
		// i++;
		// }
		// if (prx1 != null && prx1.size() > 0) {
		// NameCheckerPrx prx = NameCheckerPrxHelper.uncheckedCast(prx1.get(0)
		// .ice_timeout(300));
		// return prx;
		// } else {
		// return null;
		// }
	}

	/*
	 * public void pushFriendShip(int fromUserId, int toUserId, Operation op) {
	 * try { SearchRelationLogicPrx tmp =
	 * SearchRelationLogicAdapter.getInstance(
	 * ).getSearchRelationLogicManagerOneway(0); if(tmp != null) {
	 * tmp.pushFriendShip(fromUserId, toUserId, op); } else { return; } }
	 * catch(Exception e) { e.printStackTrace(); } }
	 */
	public Boolean Check(String name) {
		try {
			NameCheckerPrx tmp = SearchNameCheckAdapter.getInstance()
					.getSearchNameCheckManagerTwoway(0);
			if (tmp != null) {
				return tmp.Check(name);
			} else {
				return false;
			}
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	}

	public boolean[] CheckBatch(String[] name) {

		try {
			NameCheckerPrx tmp = SearchNameCheckAdapter.getInstance()
					.getSearchNameCheckManagerTwoway(0);
			if (tmp != null) {
				return tmp.CheckBatch(name);
				// return vector;
			} else {
				return null;
			}
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	public struNCResult NECheck(String name) {
		try {
			NameCheckerPrx tmp = SearchNameCheckAdapter.getInstance()
					.getSearchNameCheckManagerTwoway(0);
			if (tmp != null) {
				return tmp.NECheck(name);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public queryResult query(String query) {
		try {
			NameCheckerPrx tmp = SearchNameCheckAdapter.getInstance()
					.getSearchNameCheckManagerTwoway(0);
			if (tmp != null) {
				return tmp.query(query);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	public struNCResult[] NECheckBatch(String[] name) {

		try {
			NameCheckerPrx tmp = SearchNameCheckAdapter.getInstance()
					.getSearchNameCheckManagerTwoway(0);
			if (tmp != null) {
				return tmp.NECheckBatch(name);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	public static void main(String[] args) {
		/*
		 * int fromUserId0 = 25701953, toUserId0 = 58742, fromUserId1 =
		 * 34355462, toUserId1 = 34343523, fromUserId2 = 44444443, toUserId2 =
		 * 9999999; Operation op = Operation.Add;
		 */
		System.setProperty("isTestSearchChannel", "true");
		String[] str = { "北京", "张华", "电脑", "王旭" };

		boolean[] res = SearchNameCheckAdapter.getInstance().CheckBatch(str);
		System.out.println("res:" + res);
		try {
			Thread.sleep(100);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
