package com.xiaonei.xce.search.car;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Vector;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.generalsuggestion.SearchManagerPrx;
import xce.generalsuggestion.SearchManagerPrxHelper;
import xce.util.channel.Channel;

import com.xiaonei.search2.server.IndexManagerPrx;
import com.xiaonei.search2.server.IndexManagerPrxHelper;
import com.xiaonei.search2.model.IndexCondition;
import com.xiaonei.search2.model.IndexConditionType;
import com.xiaonei.search2.model.IndexResult;
import com.xiaonei.search2.model.IndexResultItem;

//import com.xiaonei.search.namecheck.SearchCarIndexManagerPrx;
//import com.xiaonei.search.namecheck.SearchCarIndexManagerPrxHelper;

public class SearchCarIndexManagerAdapter extends ReplicatedClusterAdapter {

	public static final int THREAD_SIZE = 8;

	// private static final String sil_endpoints =
	// "ControllerSearchRelationLogic";
	private static final String sil_endpoints = "ControllerCarIndexManager";

	private static int _interval = 5;

	private Vector<IndexManagerPrx> managers = new Vector<IndexManagerPrx>();

	private SearchCarIndexManagerAdapter() {

		super(sil_endpoints, _interval, Channel
				.newSearchControllerChannel(sil_endpoints));

	}

	private static SearchCarIndexManagerAdapter _instance = new SearchCarIndexManagerAdapter();

	public static SearchCarIndexManagerAdapter getInstance() {
		return _instance;
	}

	protected IndexManagerPrx getCarIndexManagerTwoway(int uid) {
		// Ice.ObjectPrx prx0 = getValidProxy(index, 300);
		// @SuppressWarnings("unchecked")
		int index = uid % this.getCluster();
		// List<Ice.ObjectPrx> prx1 = getAllProxy(index);
		
		List<?> list = this.getAllProxy(index);
		System.out.println("cluster:" + this.getCluster() +  " len:"+ list.size());
		Ice.ObjectPrx prx0 = this.getProxy(index);
		if (prx0 != null) {
			IndexManagerPrx prx = IndexManagerPrxHelper.uncheckedCast(prx0
					.ice_timeout(500));
			return prx;
		}
		System.out.println("prx0 is null");
		return null;
	}

	public IndexResult searchIndexWithRestriction(int opUserId,
			IndexCondition condition, int begin, int limit, String caller) {
		try {
			IndexManagerPrx tmp = getCarIndexManagerTwoway(opUserId);
			if (tmp != null) {
				return tmp.searchIndexWithRestriction(condition, begin, limit,
						caller);
			} else {
				return new IndexResult();
			}
		} catch (Exception e) {
			e.printStackTrace();
			return new IndexResult();
		}
	}

	public static void main(String[] args) {
//		System.out.println("res:" + SearchCarIndexManagerAdapter.getInstance().getCarIndexManagerTwoway(1));
		IndexCondition condition = new IndexCondition();
		condition.opUserCondition = new HashMap<String,String>();
		condition.queryCondition = new HashMap<String,String>();
		condition.type = IndexConditionType.OpenSearch;
		condition.queryCondition.put("query","奥迪");
		condition.queryCondition.put("GENERAL|user_basic.id","220678672");
		IndexResult res= SearchCarIndexManagerAdapter.getInstance().searchIndexWithRestriction(0, condition, 0, 2, "hoho");
		System.out.println("res:" + res.contents.length);
		for(IndexResultItem i: res.contents){
			System.out.println("type:" + i.type);
			System.out.println("item:" + i.content);
		}
		
		try {
			Thread.sleep(100);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
