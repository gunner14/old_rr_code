package com.xiaonei.xce.feed;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.feed.FeedItemFacadePrx;
import xce.feed.FeedItemFacadePrxHelper;
import xce.util.channel.Channel;
import com.xiaonei.xce.log.Oce;

class FeedItemFacadeHaAdapter extends ReplicatedClusterAdapter {

	private static FeedItemFacadeHaAdapter instance;
	
	private static final String ctr_endpoints = "ControllerFeedItemFacade";

	private static int _interval = 1;
	
	
	public static FeedItemFacadeHaAdapter getInstance() {
		if (instance == null) {
			instance = new FeedItemFacadeHaAdapter();
		}
		return instance;
	}

	private FeedItemFacadeHaAdapter() {
		super(ctr_endpoints, _interval, Channel.newFeedControllerChannel("ControllerFeedItemFacade"));
		
	}
	
	private FeedItemFacadePrx getManagerOneway(int userid) {
				
		Ice.ObjectPrx prx0 = getValidProxy(userid, 180);
		if (prx0 != null) {
			FeedItemFacadePrx prx = FeedItemFacadePrxHelper.uncheckedCast(prx0.ice_timeout(180).ice_oneway());
			return prx;
		} else {
			System.out.println("FeedItemFacade ha one way prx is null !");
			return null;
		}
	}

	private FeedItemFacadePrx getManager(int userid) {
	
		Ice.ObjectPrx prx0 = getValidProxy(userid, 180);
		if (prx0 != null) {
			FeedItemFacadePrx prx = FeedItemFacadePrxHelper.uncheckedCast(prx0.ice_timeout(180).ice_twoway());
			return prx;
		} else {
			System.out.println("FeedItemFacade ha prx is null !");
			return null;
		}
	}

	public void changeWikiFeedPrivacy(int userid, long source, int stype, int privacy) {
		getManagerOneway(userid).changeWikiFeedPrivacy(userid, source, stype, privacy);
	}

}
