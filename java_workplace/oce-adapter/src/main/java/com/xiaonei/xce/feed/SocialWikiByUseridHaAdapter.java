package com.xiaonei.xce.feed;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.feed.SocialWikiByUseridPrx;
import xce.feed.SocialWikiByUseridPrxHelper;
import xce.util.channel.Channel;
import com.xiaonei.xce.log.Oce;

class SocialWikiByUseridHaAdapter extends ReplicatedClusterAdapter {

	private static SocialWikiByUseridHaAdapter instance;
	
	private static final String ctr_endpoints = "ControllerSocialWikiByUserid";

	private static int _interval = 1;
	
	
	public static SocialWikiByUseridHaAdapter getInstance() {
		if (instance == null) {
			instance = new SocialWikiByUseridHaAdapter();
		}
		return instance;
	}

	private SocialWikiByUseridHaAdapter() {
		super(ctr_endpoints, _interval, Channel.newFeedControllerChannel("ControllerSocialWikiByUserid"));
		
	}
	
	private SocialWikiByUseridPrx getManagerOneway(int wikiid) {
				
		Ice.ObjectPrx prx0 = getValidProxy(wikiid, 180);
		if (prx0 != null) {
			SocialWikiByUseridPrx prx = SocialWikiByUseridPrxHelper.uncheckedCast(prx0.ice_timeout(180).ice_oneway());
			return prx;
		} else {
			System.out.println("SocialWikiByUserid ha one way prx is null !");
			return null;
		}
	}

	private SocialWikiByUseridPrx getManager(int wikiid) {
	
		Ice.ObjectPrx prx0 = getValidProxy(wikiid, 180);
		if (prx0 != null) {
			SocialWikiByUseridPrx prx = SocialWikiByUseridPrxHelper.uncheckedCast(prx0.ice_timeout(180).ice_twoway());
			return prx;
		} else {
			System.out.println("SocialWikiByUserid ha prx is null !");
			return null;
		}
	}

	public long[] gets(int[] userids, int wikiid, int begin, int limit) {
		return getManager(wikiid).gets(userids, wikiid, begin, limit);
	}

	public long[] getsWithType(int[] userids, int wikiid, int[] types, int begin, int limit) {
		return getManager(wikiid).getsWithType(userids, wikiid, types, begin, limit);
	}

}

