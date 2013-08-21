package com.xiaonei.xce.feed;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.feed.SocialWikiNewsByWikiidPrx;
import xce.feed.SocialWikiNewsByWikiidPrxHelper;
import xce.util.channel.Channel;
import com.xiaonei.xce.log.Oce;

class SocialWikiNewsByWikiidHaAdapter extends ReplicatedClusterAdapter {

	private static SocialWikiNewsByWikiidHaAdapter instance;
	
	private static final String ctr_endpoints = "ControllerSocialWikiNewsByWikiid";

	private static int _interval = 1;
	
	
	public static SocialWikiNewsByWikiidHaAdapter getInstance() {
		if (instance == null) {
			instance = new SocialWikiNewsByWikiidHaAdapter();
		}
		return instance;
	}

	private SocialWikiNewsByWikiidHaAdapter() {
		super(ctr_endpoints, _interval, Channel.newFeedControllerChannel("ControllerSocialWikiNewsByWikiid"));
		
	}
	
	private SocialWikiNewsByWikiidPrx getManagerOneway(int wikiid) {
				
		Ice.ObjectPrx prx0 = getValidProxy(wikiid, 180);
		if (prx0 != null) {
			SocialWikiNewsByWikiidPrx prx = SocialWikiNewsByWikiidPrxHelper.uncheckedCast(prx0.ice_timeout(180).ice_oneway());
			return prx;
		} else {
			System.out.println("SocialWikiNewsByWikiid ha one way prx is null !");
			return null;
		}
	}

	private SocialWikiNewsByWikiidPrx getManager(int wikiid) {
	
		Ice.ObjectPrx prx0 = getValidProxy(wikiid, 180);
		if (prx0 != null) {
			SocialWikiNewsByWikiidPrx prx = SocialWikiNewsByWikiidPrxHelper.uncheckedCast(prx0.ice_timeout(180).ice_twoway());
			return prx;
		} else {
			System.out.println("SocialWikiNewsByWikiid ha prx is null !");
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
    SocialWikiNewsByWikiidPrx prx = getManagerOneway(wikiid);
		prx.remove(wikiid, feedid);
	}

	public void reload(int wikiid) {
    SocialWikiNewsByWikiidPrx prx = getManagerOneway(wikiid);
    prx.reload(wikiid);
	}

}

