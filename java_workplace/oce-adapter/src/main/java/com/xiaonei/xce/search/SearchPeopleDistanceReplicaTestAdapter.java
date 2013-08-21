package com.xiaonei.xce.search;

import java.util.List;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.util.channel.Channel;

//import com.xiaonei.search.namecheck.PeopleDistancePrx;
//import com.xiaonei.search.namecheck.PeopleDistancePrxHelper;

//public class SearchPeopleDistanceReplicaAdapter {
//
//}
public class SearchPeopleDistanceReplicaTestAdapter {


	public static void main(String[] args) {
		System.setProperty("isTestSearchChannel", "true");
		int uid = 220678672;
		int targets[] = {220678672,238489851};
		Map<Integer,Float> res = SearchPeopleDistanceReplicaAdapter.getInstance().Get(uid,targets);
		
		System.out.println("abc size:" + res.size());
		for(Map.Entry<Integer, Float> entry:res.entrySet()){
			System.out.println(entry.getKey()+":"+entry.getValue());
		}
		try {
			Thread.sleep(100);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}