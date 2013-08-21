
package com.xiaonei.xce.feed;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import xce.clusterstate.ReplicatedClusterAdapter;
import xce.feed.FeedData;
import xce.feed.FeedDataRes;
import xce.feed.FeedItem;
import xce.feed.FeedNewsManagerPrx;
import xce.feed.FeedNewsManagerPrxHelper;
import xce.util.channel.Channel;
import com.xiaonei.xce.log.Oce;

public class FeedNewsHaAdapter extends ReplicatedClusterAdapter {

	private static FeedNewsHaAdapter instance;
	
	private static final String ctr_endpoints = "ControllerFeedNewsR";

	private static int _interval = 1;
	
	
	public static FeedNewsHaAdapter getInstance() {
		if (instance == null) {
			instance = new FeedNewsHaAdapter();
		}
		return instance;
	}

	private FeedNewsHaAdapter() {
		super(ctr_endpoints, _interval, Channel.newFeedChannel("ControllerFeedNewsR"));
		
	}
	


	private FeedNewsManagerPrx getManagerOneway(int uid) {
				
		Ice.ObjectPrx prx0 = getValidProxy(uid, 180);
		if (prx0 != null) {
			FeedNewsManagerPrx prx = FeedNewsManagerPrxHelper.uncheckedCast(prx0.ice_timeout(260).ice_oneway());
			return prx;
		} else {
			System.out.println("FeedNewsManager ha one way prx is null ! ");
			return null;
		}
	}

	private FeedNewsManagerPrx getManager(int uid) {
	
		Ice.ObjectPrx prx0 = getValidProxy(uid, 180);
		
		if (prx0 != null) {
			FeedNewsManagerPrx prx = FeedNewsManagerPrxHelper.uncheckedCast(prx0.ice_timeout(260).ice_twoway());
			return prx;
		} else {
			System.out.println("FeedNewsManager ha prx is null ! ");
			return null;
		}
	}
	
	
	public List<FeedData> getLiveFeedByStype(int uid, int[] stypes, int begin, int limit){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedDataRes feed_data_res = null;
		FeedNewsManagerPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data_res = prx.GetLiveFeedDataByStype(uid, stypes,  begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getLiveFeedByStype [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		FeedData [] contents = feed_data_res.data;
		for(int i = 0 ; i < contents.length; ++i){
			list.add(contents[i]);
		}
		return list;
	}
	
	public List<FeedData> getHotFeedByStype(int uid, int[] stypes, int begin, int limit){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedDataRes feed_data_res = null;
		FeedNewsManagerPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data_res = prx.GetHotFeedDataByStype(uid, stypes,  begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getHotFeedByStype [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		FeedData [] contents = feed_data_res.data;
		for(int i = 0 ; i < contents.length; ++i){
			list.add(contents[i]);
		}
		return list;
	}
	
	public List<FeedData> getLiveFeedByType(int uid, int[] types, int begin, int limit){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedDataRes feed_data_res = null;
		FeedNewsManagerPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data_res = prx.GetLiveFeedDataByType(uid, types,  begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getLiveFeedByType [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		FeedData [] contents = feed_data_res.data;
		for(int i = 0 ; i < contents.length; ++i){
			list.add(contents[i]);
		}
		return list;
	}
	

	public List<FeedData> getHotFeedByType(int uid, int[] types, int begin, int limit){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedDataRes feed_data_res = null;
		FeedNewsManagerPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data_res = prx.GetHotFeedDataByType(uid, types,  begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getHotFeedByType [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		FeedData [] contents = feed_data_res.data;
		for(int i = 0 ; i < contents.length; ++i){
			list.add(contents[i]);
		}
		return list;
	}
	
	public List<FeedData> getLiveFeed(int uid, int begin, int limit){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedDataRes feed_data_res = null;
		FeedNewsManagerPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data_res = prx.GetLiveFeedData(uid,  begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getLiveFeed [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		FeedData [] contents = feed_data_res.data;
		for(int i = 0 ; i < contents.length; ++i){
			list.add(contents[i]);
		}
		return list;
	}
	

	public List<FeedData> getHotFeed(int uid, int begin, int limit) {
		List<FeedData> list = new ArrayList<FeedData>();
		FeedDataRes feed_data_res = null;
		FeedNewsManagerPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data_res = prx.GetHotFeedData(uid, begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getHotFeed [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		FeedData[] contents = feed_data_res.data;
		for (int i = 0; i < contents.length; ++i) {
			list.add(contents[i]);
		}
		return list;
	}

	
	public List<FeedData> getFeedDataByIds(long[] feedIds){
		List<FeedData> list = new ArrayList<FeedData>();
		if(feedIds.length == 0){
			return list;
		}
		FeedData [] contents  = null;
		FeedNewsManagerPrx prx = null;
		try {
			prx = getManager((int)(feedIds[0]% 10000));
			contents = prx.GetFeedDataByIds(feedIds);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getFeedDataByIds [" + prx + "]";
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		for(int i = 0 ; i < contents.length; ++i){
			list.add(contents[i]);
		}
		return list;
	}
	
	public List<FeedItem>  getOriginalFeedData(int uid) {
		List<FeedItem> list = new ArrayList<FeedItem>();
		
		FeedItem [] items = null;
		FeedNewsManagerPrx prx = null;
		try {
			prx = getManager(uid);
			items = prx.GetOriginalFeedData(uid);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getOriginalFeedData [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		for(int i = 0 ; i < items.length; ++i){
			list.add(items[i]);
		}
		return list;
	}
	
	public int  getUnviewedCount(int uid, int type) {
		int ret = 0;
		FeedNewsManagerPrx prx = null;
		try {
			prx = getManager(uid);
			ret = prx.GetUnviewedCount(uid, type);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getUnviewedCount [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}	
		return  ret;
	
	}
	
	public Map<Integer,Integer>  getCountMapByStype(int uid, int[] stypes, boolean unviewed) {
		Map<Integer,Integer> ret = null;
		FeedNewsManagerPrx prx = null;
		try {
			prx = getManager(uid);
			ret = prx.GetCountMapByStype(uid, stypes,unviewed);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getCountMapByStype [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		return ret;
	}

//	public static void main(String[] args) {
//		int [] stypes = {601,502};
//		Map< Integer, Integer > res = FeedNewsHaAdapter.getInstance().getCountMapByStype(220678672, 
//				stypes, false);
//		for (Map.Entry< Integer, Integer > entry : res.entrySet()) {
//			System.out.println("stype:" + entry.getKey() + ", name="
//					+ entry.getValue());
//		}
//	}
	
//	public static void main(String[] args) {
//		// TODO Auto-generated method stub
//
//		List<FeedData> res = FeedNewsHaAdapter.getInstance().getHotFeed(225092473, 
//   			0, 20);
//		FeedData fd = res.get(0);
//		System.out.println("fd:" + fd.feed + " size:" + res.size());
//	}
			
			
}
