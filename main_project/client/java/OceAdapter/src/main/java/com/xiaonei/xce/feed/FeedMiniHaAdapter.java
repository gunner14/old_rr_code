package com.xiaonei.xce.feed;

import java.util.ArrayList;
import java.util.List;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.feed.FeedData;
import xce.feed.FeedItemManagerPrx;
import xce.feed.FeedItemManagerPrxHelper;
import xce.util.channel.Channel;
import com.xiaonei.xce.log.Oce;

class FeedMiniHaAdapter extends  ReplicatedClusterAdapter{

private static FeedMiniHaAdapter instance;
	
	private static final String ctr_endpoints = "ControllerFeedMiniR";

	private static int _interval = 1;
	
	
	public static FeedMiniHaAdapter getInstance() {
		if (instance == null) {
			instance = new FeedMiniHaAdapter();
		}
		return instance;
	}

	private FeedMiniHaAdapter() {
		super(ctr_endpoints, _interval, Channel.newFeedChannel("ControllerFeedMiniR"));
		
	}
	


	private FeedItemManagerPrx getManagerOneway(int uid) {
				
		Ice.ObjectPrx prx0 = getValidProxy(uid, 180);
		if (prx0 != null) {
			FeedItemManagerPrx prx = FeedItemManagerPrxHelper.uncheckedCast(prx0.ice_timeout(260).ice_oneway());
			return prx;
		} else {
			System.out.println("FeedMini ha one way prx is null !");
			return null;
		}
	}

	private FeedItemManagerPrx getManager(int uid) {
	
		Ice.ObjectPrx prx0 = getValidProxy(uid, 180);
		//System.out.println("prx:" + prx0);
		if (prx0 != null) {
			FeedItemManagerPrx prx = FeedItemManagerPrxHelper.uncheckedCast(prx0.ice_timeout(260).ice_twoway());
			return prx;
		} else {
			System.out.println("FeedMini ha prx is null !");
			
			return null;
		}
	}
	
	
	public List<FeedData> getMiniByStype(int uid, int[] stypes, int begin, int limit){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedData [] contents = null;
		FeedItemManagerPrx prx = null;
		try {
			prx = getManager(uid);
			contents = prx.getFeedDataByStype(uid, stypes, begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getMiniByStype [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		for(int i = 0 ; i < contents.length; ++i){
			list.add(contents[i]);
		}
		return list;
	}
	public List<FeedData> getMiniByType(int uid, int[] types, int begin, int limit){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedData [] contents = null;
		FeedItemManagerPrx prx = null;
		try {
			prx = getManager(uid);
			contents = prx.getFeedDataByType(uid, types, begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getMiniByType [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		for(int i = 0 ; i < contents.length; ++i){
			list.add(contents[i]);
		}
		return list;
	}
	public List<FeedData> getMini(int uid, int begin, int limit){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedData [] contents = null;
		FeedItemManagerPrx prx = null;
		try {
			prx = getManager(uid);
			contents = prx.getFeedData(uid,  begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getMini [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		for(int i = 0 ; i < contents.length; ++i){
			list.add(contents[i]);
		}
		return list;
	}
	
	public boolean hasFeed(int uid){
		boolean ret = false;
		FeedItemManagerPrx prx = null;
		try {
			prx = getManager(uid);
			ret = prx.hasFeed(uid);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".hasFeed [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		return ret;
	}
	
//	public static void main(String[] args) {
//		// TODO Auto-generated method stub
//
//		List<FeedData> res = FeedMiniHaAdapter.getInstance().getMini(225092473, 
//   			0, 20);
//		FeedData fd = res.get(0);
//		System.out.println("fd:" + fd.feed + " size:" + res.size());
//	}
	
//	public List<FeedData> getMiniByStype(int uid, int[] stypes, int begin, int limit){
//		
//	}
//	public List<FeedData> getMiniByType(int uid, int[] types, int begin, int limit){
//		
//	}
//	public List<FeedData> getMini(int uid, int begin, int limit){
//		
//	}
}
