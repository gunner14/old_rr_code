package com.xiaonei.xce.search;

import java.util.List;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.util.channel.Channel;

//import com.xiaonei.search.namecheck.PeopleDistancePrx;
//import com.xiaonei.search.namecheck.PeopleDistancePrxHelper;

import com.xiaonei.search.distance.PeopleDistancePrx;
import com.xiaonei.search.distance.PeopleDistancePrxHelper;
import Ice.ObjectPrx;
//public class SearchPeopleDistanceReplicaAdapter {
//
//}
public class SearchPeopleDistanceReplicaAdapter extends
		ReplicatedClusterAdapter {

	public static final int THREAD_SIZE = 8;

	// private static final String sil_endpoints =
	// "ControllerSearchRelationLogic";
	private static final String sil_endpoints = "ControllerPeopleDistance";

	private static int _interval = 120;

	private Vector<PeopleDistancePrx> managers = new Vector<PeopleDistancePrx>();

	private SearchPeopleDistanceReplicaAdapter() {

		super(sil_endpoints, _interval, Channel.newSearchChannel(sil_endpoints));

	}

	private static SearchPeopleDistanceReplicaAdapter _instance = new SearchPeopleDistanceReplicaAdapter();

	public static SearchPeopleDistanceReplicaAdapter getInstance() {
		return _instance;
	}

	protected PeopleDistancePrx getSearchPeopleDistanceManagerOneway(int index) {
		// Ice.ObjectPrx prx0 = getValidProxy(index, 300);

		@SuppressWarnings("unchecked")
		List<Ice.ObjectPrx> prx1 = getAllProxy(index);
		System.out.println("@@ proxy size: " + prx1.size());
		int i = 0;
		for (Ice.ObjectPrx tmp : prx1) {
			System.out.println("No " + i + " proxy: " + tmp);
			i++;
		}
		if (prx1 != null && prx1.size() > 0) {
			PeopleDistancePrx prx = PeopleDistancePrxHelper.uncheckedCast(prx1
					.get(0).ice_timeout(300).ice_oneway());
			return prx;
		} else {
			return null;
		}
	}

	protected PeopleDistancePrx getSearchPeopleDistanceManagerTwoway(int index) {
		@SuppressWarnings("unchecked")
		ObjectPrx prx2 = getProxy(index);
		if(prx2 == null){
			System.out.println("@@@@ getSearchPeopleDistanceManagerOneway. index:" + index + " is null");
			return null;
		} else {
			System.out.println("@@@@ getSearchPeopleDistanceManagerOneway. index:" + index + " not null");
			PeopleDistancePrx prx = PeopleDistancePrxHelper.uncheckedCast(prx2.ice_timeout(300));
			return prx;
		}
//		List<Ice.ObjectPrx> prx1 = getAllProxy(index);
//		System.out.println("proxy size: " + prx1.size());
//		int i = 0;
//		for (Ice.ObjectPrx tmp : prx1) {
//			System.out.println("No " + i + " proxy: " + tmp);
//			i++;
//		}
//		if (prx1 != null && prx1.size() > 0) {
//			PeopleDistancePrx prx = PeopleDistancePrxHelper.uncheckedCast(prx1
//					.get(0).ice_timeout(300));
//			return prx;
//		} else {
//			return null;
//		}
	}

	public Map<Integer, Float> Get(int uid, int[] targets) {
		Map<Integer, Float> res = new HashMap<Integer, Float>();
		try {
			res = getSearchPeopleDistanceManagerTwoway(uid).Get(uid, targets);
		} catch (Exception e) {
			e.printStackTrace();
			return res;
		}
		return res;
	}

	public static void main(String[] args) {
		/*
		 * int fromUserId0 = 25701953, toUserId0 = 58742, fromUserId1 =
		 * 34355462, toUserId1 = 34343523, fromUserId2 = 44444443, toUserId2 =
		 * 9999999; Operation op = Operation.Add;
		 */
		System.setProperty("isTestSearchChannel", "true");
		// String name = "魏铁";
		String[] name = { "北京", "张华", "电脑", "王旭" };
//		boolean[] res = SearchPeopleDistanceReplicaAdapter.getInstance()
//				.Get();
//		System.out.println("res:" + res);
		try {
			Thread.sleep(100);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}