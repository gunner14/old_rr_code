
package com.xiaonei.xce.feed;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.feed.FeedAssistantPrx;
import xce.feed.FeedAssistantPrxHelper;
import xce.util.channel.Channel;
import com.xiaonei.xce.log.Oce;

public class FeedAssistantNHaAdapter extends ReplicatedClusterAdapter {

	private static FeedAssistantNHaAdapter instance;
	
	private static final String ctr_endpoints = "ControllerFeedAssistantNR";

	private static int _interval = 1;
	
	
	public static FeedAssistantNHaAdapter getInstance() {
		if (instance == null) {
			instance = new FeedAssistantNHaAdapter();
		}
		return instance;
	}

	private FeedAssistantNHaAdapter() {
		super(ctr_endpoints, _interval, Channel.newFeedControllerChannel("ControllerFeedAssistantNR"));
		
	}
	
	private FeedAssistantPrx getManagerOneway(int source) {
				
		Ice.ObjectPrx prx0 = getValidProxy(source, 180);
		if (prx0 != null) {
			FeedAssistantPrx prx = FeedAssistantPrxHelper.uncheckedCast(prx0.ice_timeout(180).ice_oneway());
			return prx;
		} else {
			System.out.println("FeedAssistantN ha one way prx is null !");
			return null;
		}
	}

	private FeedAssistantPrx getManager(int source) {
	
		Ice.ObjectPrx prx0 = getValidProxy(source, 180);
		if (prx0 != null) {
			FeedAssistantPrx prx = FeedAssistantPrxHelper.uncheckedCast(prx0.ice_timeout(180).ice_twoway());
			return prx;
		} else {
			System.out.println("FeedView ha prx is null !");
			return null;
		}
	}
        public long getFeedId(long source, int stype, int actor){
                int mod = (int)(source % 10);
                return getManager(mod).getFeedId(source, stype, actor);
        }
}
