package com.xiaonei.xce.search;

import java.util.List;
import java.util.Random;
import java.util.Vector;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.generalsuggestion.SearchManagerPrx;
import xce.generalsuggestion.SearchManagerPrxHelper;
import xce.util.channel.Channel;

import com.xiaonei.search.keywordextract.KeywordExtracterPrx;
import com.xiaonei.search.keywordextract.KeywordExtracterPrxHelper;
import com.xiaonei.search.keywordextract.struKwResult;

public class SearchKeywordExtractAdapter extends ReplicatedClusterAdapter {

	public static final int THREAD_SIZE = 8;

	// private static final String sil_endpoints =
	// "ControllerSearchRelationLogic";
	private static final String sil_endpoints = "ControllerKeywordExtract";

	private static int _interval = 120;

	private Vector<KeywordExtracterPrx> managers = new Vector<KeywordExtracterPrx>();

	private SearchKeywordExtractAdapter() {

		super(sil_endpoints, _interval, Channel
				.newGadgetsChannel(sil_endpoints));
//		super(sil_endpoints, _interval, Channel
//				.newSearchChannel(sil_endpoints));

	}

	private static SearchKeywordExtractAdapter _instance = new SearchKeywordExtractAdapter();

	public static SearchKeywordExtractAdapter getInstance() {
		return _instance;
	}

	protected KeywordExtracterPrx getSearchKeywordExtractManagerOneway(int index) {
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
			KeywordExtracterPrx prx = KeywordExtracterPrxHelper.uncheckedCast(prx1.get(0)
					.ice_timeout(300).ice_oneway());
			return prx;
		} else {
			return null;
		}
	}

	protected KeywordExtracterPrx getSearchKeywordExtractManagerTwoway(int index) {
		// Ice.ObjectPrx prx0 = getValidProxy(index, 300);
		// @SuppressWarnings("unchecked")
		Random r = new Random();
		index = r.nextInt(getCluster());
		System.out.println("index:" + index + " cluster:" + getCluster());
		// List<Ice.ObjectPrx> prx1 = getAllProxy(index);
		
		Ice.ObjectPrx prx0 = getValidProxy(index, 300);
		if (prx0 != null) {
			KeywordExtracterPrx prx = KeywordExtracterPrxHelper.uncheckedCast(prx0
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
		// KeywordExtracterPrx prx = KeywordExtracterPrxHelper.uncheckedCast(prx1.get(0)
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

	public struKwResult[] getKeywords(String input) {
		try {
			KeywordExtracterPrx tmp = SearchKeywordExtractAdapter.getInstance()
					.getSearchKeywordExtractManagerTwoway(0);
			if (tmp != null) {
				return tmp.getKeywords(input);
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
		String str = "鸟叔的江南style跳的真好";

		struKwResult[] res = SearchKeywordExtractAdapter.getInstance().getKeywords(str);
		System.out.println("res:" + res[0].strKeyword);
		try {
			Thread.sleep(100);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
