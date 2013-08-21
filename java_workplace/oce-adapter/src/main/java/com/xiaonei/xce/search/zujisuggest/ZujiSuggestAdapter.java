package com.xiaonei.xce.search.zujisuggest;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Vector;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.generalsuggestion.SearchManagerPrx;
import xce.generalsuggestion.SearchManagerPrxHelper;
import xce.util.channel.Channel;

import com.xiaonei.search2.model.IndexCondition;
import com.xiaonei.search2.model.IndexConditionType;
import com.xiaonei.search2.model.IndexResult;
import com.xiaonei.search2.model.IndexResultItem;
import com.renren.search.generalquerysuggestion.*;

//import com.xiaonei.search.namecheck.SearchCarGeneralQSPrx;
//import com.xiaonei.search.namecheck.SearchCarGeneralQSPrxHelper;

public class ZujiSuggestAdapter extends ReplicatedClusterAdapter {

	public static final int THREAD_SIZE = 8;

	// private static final String sil_endpoints =
	// "ControllerSearchRelationLogic";
	private static final String sil_endpoints = "ControllerGeneralQS";

	private static int _interval = 5;

	private Vector<GeneralQSPrx> managers = new Vector<GeneralQSPrx>();

	private ZujiSuggestAdapter() {

		super(sil_endpoints, _interval, Channel
				.newSearchControllerChannel(sil_endpoints));

	}

	private static ZujiSuggestAdapter _instance = new ZujiSuggestAdapter();

	public static ZujiSuggestAdapter getInstance() {
		return _instance;
	}

	protected GeneralQSPrx getPrxTwoway(int uid) {
		// Ice.ObjectPrx prx0 = getValidProxy(index, 300);
		// @SuppressWarnings("unchecked")
		int index = uid % this.getCluster();
		// List<Ice.ObjectPrx> prx1 = getAllProxy(index);
		
		List<?> list = this.getAllProxy(index);
		Ice.ObjectPrx prx0 = this.getProxy(index);
		if (prx0 != null) {
			GeneralQSPrx prx = GeneralQSPrxHelper.uncheckedCast(prx0
					.ice_timeout(500));
			return prx;
		}
		return null;
	}

	public List<Integer> search(int id, String query, int len, int limit, Boolean isPre) {
		List<Integer> res = new ArrayList<Integer>();
		try {
			GeneralQSPrx tmp = getPrxTwoway(id);
			if (tmp != null) {
				//int[] resids = tmp.search( id,  query,  len,  limit,  isPre) ;
				//for(int resid : resids){
				//	res.add(resid);
				//}
				int[] resids  = null;
			} else {
				System.out.println("prx is null");
				return res;
			}
		} catch (Exception e) {
			e.printStackTrace();
			return res;
		}
		return res;
	}

	public static void main(String[] args) {
		List<Integer> res = ZujiSuggestAdapter.getInstance().search(375960868, "bj" ,10, 10,false);
//		List<Integer> res = ZujiSuggestAdapter.getInstance().search(12, "高", 10, 3, false);
		System.out.println("res:" + res.toString());
	}
}
