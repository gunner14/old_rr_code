package com.xiaonei.xce.feed;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.feed.SocialWikiHotByWikiidPrx;
import xce.feed.SocialWikiHotByWikiidPrxHelper;
import xce.util.channel.Channel;
import com.xiaonei.xce.log.Oce;

class SocialWikiHotByWikiidHaAdapter extends ReplicatedClusterAdapter {

	private static SocialWikiHotByWikiidHaAdapter instance;
	
	private static final String ctr_endpoints = "ControllerSocialWikiHotByWikiid";

	private static int _interval = 1;
	
	
	public static SocialWikiHotByWikiidHaAdapter getInstance() {
		if (instance == null) {
			instance = new SocialWikiHotByWikiidHaAdapter();
		}
		return instance;
	}

	private SocialWikiHotByWikiidHaAdapter() {
		super(ctr_endpoints, _interval, Channel.newFeedControllerChannel("ControllerSocialWikiHotByWikiid"));
		
	}
	
	private SocialWikiHotByWikiidPrx getManagerOneway(int wikiid) {
				
		Ice.ObjectPrx prx0 = getValidProxy(wikiid, 180);
		if (prx0 != null) {
			SocialWikiHotByWikiidPrx prx = SocialWikiHotByWikiidPrxHelper.uncheckedCast(prx0.ice_timeout(180).ice_oneway());
			return prx;
		} else {
			System.out.println("SocialWikiHotByWikiid ha one way prx is null !");
			return null;
		}
	}

	private SocialWikiHotByWikiidPrx getManager(int wikiid) {
	
		Ice.ObjectPrx prx0 = getValidProxy(wikiid, 180);
		if (prx0 != null) {
			SocialWikiHotByWikiidPrx prx = SocialWikiHotByWikiidPrxHelper.uncheckedCast(prx0.ice_timeout(180).ice_twoway());
			return prx;
		} else {
			System.out.println("SocialWikiHotByWikiid ha prx is null !");
			return null;
		}
	}

	public long[] get(int wikiid, int begin, int limit) {
		return getManager(wikiid).get(wikiid, begin, limit);
	}

	public long[] getByTypes(int wikiid, int begin, int limit, int... types) {
		return getManager(wikiid).getByTypes(wikiid, begin, limit, types);
	}

	public void remove(int wikiid, long feedid) {
		getManagerOneway(wikiid).remove(wikiid, feedid);
	}

	public void reload(int wikiid) {
		getManagerOneway(wikiid).reload(wikiid);
	}
}

