
package com.xiaonei.xce.feed;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.feed.FeedData;
import xce.feed.FeedDataByte;
import xce.feed.FeedDataRes;
import xce.feed.FeedDataByteRes;
import xce.feed.FeedViewPrx;
import xce.feed.FeedViewPrxHelper;
import xce.util.channel.Channel;
import com.xiaonei.xce.log.Oce;

public class FeedViewHaAdapter extends ReplicatedClusterAdapter {

	private static FeedViewHaAdapter instance;
	
	private static final String ctr_endpoints = "ControllerFeedViewR";

	private static int _interval = 1;
	
	
	public static FeedViewHaAdapter getInstance() {
		if (instance == null) {
			instance = new FeedViewHaAdapter();
		}
		return instance;
	}

	private FeedViewHaAdapter() {
		super(ctr_endpoints, _interval, Channel.newFeedChannel("ControllerFeedViewR"));
		
	}
	


	private FeedViewPrx getManagerOneway(int uid) {
				
		Ice.ObjectPrx prx0 = getValidProxy(uid, 180);
		if (prx0 != null) {
			FeedViewPrx prx = FeedViewPrxHelper.uncheckedCast(prx0.ice_timeout(180).ice_oneway());
			return prx;
		} else {
			System.out.println("FeedView ha one way prx is null !");
			return null;
		}
	}

	private FeedViewPrx getManager(int uid) {
	
		Ice.ObjectPrx prx0 = getValidProxy(uid, 180);
		//System.out.println("FeedView prx:" + prx0);
		if (prx0 != null) {
			FeedViewPrx prx = FeedViewPrxHelper.uncheckedCast(prx0.ice_timeout(180).ice_twoway());
			return prx;
		} else {
			System.out.println("FeedView ha prx is null !");
			return null;
		}
	}

	// uid, name 始终为访问者的

	// 由调用者传入name，可以减少一次远程调用

	public List<String> getNews(int vistor, String vistorName, int view,
			int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(vistor);
			html = prx.Get(vistor, vistorName, view, begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getNews [" + prx + "] userid=" + vistor;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getNewsByType(int vistor, String vistorName, int view,
			int[] types, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(vistor);
			html = prx.GetByType(vistor, vistorName, view, types, begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getNewsByType [" + prx + "] userid=" + vistor;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getNewsByStype(int vistor, String vistorName, int view,
			int[] stypes, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(vistor);
			html = prx.GetByStype(vistor, vistorName, view, stypes, begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getNewsByStype [" + prx + "] userid=" + vistor;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;

	}

	public List<String> getFeedByIds(int vistor, String vistorName,
			long[] feedIds) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(vistor);
			html = prx.GetByIds(vistor, vistorName, feedIds);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getFeedByIds [" + prx + "] userid=" + vistor;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getFeedByIds2(int vistor, String vistorName, int view,
			long[] feedIds) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(vistor);
			html = prx.GetByIds2(vistor, vistorName, view, feedIds);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getFeedByIds2 [" + prx + "] userid=" + vistor;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}
	
	public List<String> getFeedByIdsAndStype(int vistor, String vistorName,
			long[] feedIds, int stype, long firstID) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(vistor);
			html = prx.GetByIdsAndStype(vistor, vistorName, feedIds, stype, firstID);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getFeedByIdsAndStype [" + prx + "] userid=" + vistor;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getFeedByIdsAndStype2(int vistor, String vistorName, int view,
			long[] feedIds, int stype, long firstID) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(vistor);
			html = prx.GetByIdsAndStype2(vistor, vistorName, view, feedIds, stype, firstID);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getFeedByIdsAndStype2 [" + prx + "] userid=" + vistor;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	// owner 为被访问者 id

	public List<String> getMini(int owner, int vistor, String vistorName,
			int view, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetMini(owner, vistor, vistorName, view, begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getMini [" + prx + "] userid=" + owner;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getMiniByType(int owner, int vistor, String vistorName,
			int view, int[] types, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetMiniByType(owner, vistor, vistorName, view, types, begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getMiniByType [" + prx + "] userid=" + owner;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getMiniByStype(int owner, int vistor,
			String vistorName, int view, int[] stypes, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetMiniByStype(owner, vistor, vistorName, view, stypes, begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getMiniByStype [" + prx + "] userid=" + owner;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getFavorite(int owner, String ownerName, int view,
			int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetFavorite(owner, ownerName, view, begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getFavorite [" + prx + "] userid=" + owner;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getGuideFeed(int owner, String ownerName, int view,
			int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetGuideFeed(owner, ownerName, view, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getGuideFeed [" + prx + "] userid=" + owner;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getFriendOriginal(int owner, String ownerName, int view,
			int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetFriendOriginal(owner, ownerName, view, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getFriendOriginal [" + prx + "] userid=" + owner;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getHot(int owner, String ownerName, int view,
			int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetHot(owner, ownerName, view, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getHot [" + prx + "] userid=" + owner;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getHotByType(int owner, String ownerName, int view,
			int[] types, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetHotByType(owner, ownerName, view, types, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getHotByType [" + prx + "] userid=" + owner;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}
	
	public List<String> getHotByStype(int owner, String ownerName, int view,
			int[] stypes, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetHotByStype(owner, ownerName, view, stypes, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getHotByStype [" + prx + "] userid=" + owner;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getLive(int owner, String ownerName, int view,
			int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetLive(owner, ownerName, view, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getLive [" + prx + "] userid=" + owner;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getLiveByStype(int owner, String ownerName, int view,
			int[] stypes, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetLiveByStype(owner, ownerName, view, stypes, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getLiveByStype [" + prx + "] userid=" + owner;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}

	public List<String> getLiveByType(int owner, String ownerName, int view,
			int[] types, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetLiveByType(owner, ownerName, view, types, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getLiveByType [" + prx + "] userid=" + owner;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}
 
	public List<String> getMiniGroupFeedData(int gid, String name, int render,
			int memberid, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(gid);
			html = prx.GetMiniGroupFeedData(gid, name, render, memberid, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getMiniGroupFeedData [" + prx + "] groupid=" + gid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}
	
	public List<String> getMiniGroupFeedDataByType(int gid, String name, int render,
			int memberid, int[] types, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(gid);
			html = prx.GetMiniGroupFeedDataByType(gid, name, render, memberid, types, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getMiniGroupFeedDataByType [" + prx + "] groupid=" + gid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}
	
	public List<String> getMiniGroupFeedDatabyStype(int gid, String name, int render,
			int memberid, int[] types, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(gid);
			html = prx.GetMiniGroupFeedDataByStype(gid, name, render, memberid, types, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getMiniGroupFeedDatabyStype [" + prx + "] groupid=" + gid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}
	
	public List<String> getMiniGroupFeedData2(int gid, String name, int render,
			int memberid, int begin, int limit, boolean isAdmin) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(gid);
			html = prx.GetMiniGroupFeedData2(gid, name, render, memberid, begin, limit, isAdmin);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getMiniGroupFeedData2 [" + prx + "] groupid=" + gid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}
	
	public List<String> getMiniGroupFeedData2CS(int gid, String name, int render,
			int memberid, int begin, int limit, boolean isAdmin, int view) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(gid);
			html = prx.GetMiniGroupFeedData2CS(gid, name, render, memberid, begin, limit, isAdmin, view);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getMiniGroupFeedData2CS [" + prx + "] groupid=" + gid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}
	
	public List<String> getMiniGroupFeedDataByType2(int gid, String name, int render,
			int memberid, int[] types, int begin, int limit, boolean isAdmin) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(gid);
			html = prx.GetMiniGroupFeedDataByType2(gid, name, render, memberid, types, begin, limit, isAdmin);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getMiniGroupFeedDataByType2 [" + prx + "] groupid=" + gid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}
	
	public List<String> getMiniGroupFeedDatabyStype2(int gid, String name, int render,
			int memberid, int[] types, int begin, int limit, boolean isAdmin) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(gid);
			html = prx.GetMiniGroupFeedDataByStype2(gid, name, render, memberid, types, begin, limit, isAdmin);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getMiniGroupFeedDatabyStype2 [" + prx + "] groupid=" + gid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<String> res = new ArrayList<String>();
		for (int i = 0; i < html.length; ++i) {
			res.add(html[i]);
		}
		return res;
	}
	
	public List<FeedData> getHotDataByStype(int uid, String name, int render, int[] types, int begin, int limit){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedDataRes feed_data_res = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data_res = prx.GetHotDataByStype(uid, name, render, types, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getHotDataByStype [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		FeedData [] contents = feed_data_res.data;
		for(int i = 0 ; i < contents.length; ++i){
			list.add(contents[i]);
		}
		return list;
	}
	public List<FeedData> getLiveDataByStype(int uid, String name, int render, int[] types, int begin, int limit){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedDataRes feed_data_res = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data_res = prx.GetLiveDataByStype(uid, name, render, types, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getLiveDataByStype [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		FeedData [] contents = feed_data_res.data;
		for(int i = 0 ; i < contents.length; ++i){
			list.add(contents[i]);
		}
		return list;
	}
	public List<FeedData> getMiniDataByStype(int uid, String name, int render, int[] types, int begin, int limit){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedData[] feed_data = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data = prx.GetMiniDataByStype(uid, name, render, types, begin, limit);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getMiniDataByStype [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		for(int i = 0 ; i < feed_data.length; ++i){
			list.add(feed_data[i]);
		}
		return list;
	}

	public List<FeedData> getHotDataByStype2(int uid, String name, int render, int[] types, int begin, int limit, int view){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedDataRes feed_data_res = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data_res = prx.GetHotDataByStype2(uid, name, render, types, begin, limit, view);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getHotDataByStype2 [" + prx + "] userid=" + uid; 
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		FeedData [] contents = feed_data_res.data;
		for(int i = 0 ; i < contents.length; ++i){
			list.add(contents[i]);
		}
		return list;
	}
	public List<FeedData> getLiveDataByStype2(int uid, String name, int render, int[] types, int begin, int limit, int view){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedDataRes feed_data_res = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data_res = prx.GetLiveDataByStype2(uid, name, render, types, begin, limit, view);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getLiveDataByStype2 [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		FeedData [] contents = feed_data_res.data;
		for(int i = 0 ; i < contents.length; ++i){
			list.add(contents[i]);
		}
		return list;
	}
        public List<FeedData> getLiveDataByStypeCompressed(int uid, String name, int render, int[] types, int begin, int limit, int view){
                List<FeedData> list = new ArrayList<FeedData>();
                FeedDataByteRes feed_data_res = null;
                FeedViewPrx prx = null;
                try {
                        prx = getManager(uid);
                        feed_data_res = prx.GetLiveDataByStypeCompressed(uid, name, render, types, begin, limit, view);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getLiveDataByStypeCompressed [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
                FeedDataByte [] contents = feed_data_res.data;
		for(int i = 0 ; i < contents.length; ++i){
			FeedData temp = new FeedData();
			temp.feed = contents[i].feed;
			temp.source = contents[i].source;
			temp.actor = contents[i].actor;
			temp.weight = contents[i].weight;
			temp.type = contents[i].type;
			temp.time = contents[i].time;
			byte[] b = QuickLZ.decompress(contents[i].xml);
			try{
				temp.xml = new String(b, "utf-8");
			} catch (Exception e) {
				Oce.getLogger().error(e);
			}
			list.add(temp);
		}
                return list;
        }
	public List<FeedData> getMiniDataByStype2(int uid, String name, int render, int[] types, int begin, int limit, int view){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedData[] feed_data = null;
		FeedViewPrx prx = null;
		try {
	 		prx = getManager(uid);
			feed_data = prx.GetMiniDataByStype2(uid, name, render, types, begin, limit, view);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getMiniDataByStype2 [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		for(int i = 0 ; i < feed_data.length; ++i){
			list.add(feed_data[i]);
		}
		return list;
	}
        public List<FeedData> getMiniDataByStypeCompressed(int uid, String name, int render, int[] types, int begin, int limit, int view){
                List<FeedData> list = new ArrayList<FeedData>();
                FeedDataByte[] contents = null;
                FeedViewPrx prx = null;
                try {
                        prx = getManager(uid);
                        contents = prx.GetMiniDataByStypeCompressed(uid, name, render, types, begin, limit, view);
                } catch (Exception e) {
                        String err = this.getClass().getName() + ".getMiniDataByStypeCompressed [" + prx + "] userid=" + uid;
                        Oce.getLogger().error(err + " " + e);
                        throw new RuntimeException(err + " " + e, e);
		}
		for(int i = 0 ; i < contents.length; ++i){
			FeedData temp = new FeedData();
			temp.feed = contents[i].feed;
			temp.source = contents[i].source;
			temp.actor = contents[i].actor;
			temp.weight = contents[i].weight;
			temp.type = contents[i].type;
			temp.time = contents[i].time;
			byte[] b = QuickLZ.decompress(contents[i].xml);
			try{
				temp.xml = new String(b, "utf-8");
			} catch (Exception e) {
				Oce.getLogger().error(e);
			}
			list.add(temp);
                }
                return list;
        }
	public List<FeedData> getFavorite2(int uid, String name, int render, int begin, int limit, int view){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedData[] feed_data = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data = prx.GetFavorite2(uid, name, render, begin, limit, view);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getFavorite2 [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		for(int i = 0 ; i < feed_data.length; ++i){
			list.add(feed_data[i]);
		}
		return list;
	}
	public List<FeedData> getFavorite2ByStype(int uid, String name, int render, int[] types, int begin, int limit, int view){
		List<FeedData> list = new ArrayList<FeedData>();
		FeedData[] feed_data = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data = prx.GetFavorite2ByStype(uid, name, render, types, begin, limit, view);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getFavorite2ByStype [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		for(int i = 0 ; i < feed_data.length; ++i){
			list.add(feed_data[i]);
		}
		return list;
	}
        public List<FeedData> getByIds2View(int uid, String name, int render, long[] fids, int view){
                List<FeedData> list = new ArrayList<FeedData>();
                FeedData[] feed_data = null;
                FeedViewPrx prx = null;
                try {
                        prx = getManager(uid);
                        feed_data = prx.GetByIds2View(uid, name, render, fids, view);
                } catch (Exception e) {
                        String err = this.getClass().getName() + ".getByIdsView [" + prx + "] userid=" + uid;
                        Oce.getLogger().error(err + " " + e);
                        throw new RuntimeException(err + " " + e, e);
                }
                for(int i = 0 ; i < feed_data.length; ++i){
                        list.add(feed_data[i]);
                }
                return list;
        }
	public List<FeedData> getGreaterThanFeedidWithUseridByType(int appid, long feedid, int limit, int uid, int[] types, int render, int view) {
		List<FeedData> list = new ArrayList<FeedData>();
		FeedData[] feed_data = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(uid);
			feed_data = prx.GetGreaterThanFeedidWithUseridByType(appid, feedid, limit, uid, types, render, view);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getGreaterThanFeedidWithUseridByType [" + prx + "] userid=" + uid;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		for(int i = 0 ; i < feed_data.length; ++i){
			list.add(feed_data[i]);
		}
		return list;
 	}
	public List<FeedData> getUnlimitedDataByStype(int uid, int begin, int limit, int[] types, int render, int view) {
                List<FeedData> list = new ArrayList<FeedData>();
                FeedDataByte[] contents = null;
                FeedViewPrx prx = null;
                try {
                        prx = getManager(uid);
                        contents = prx.GetUnlimitedDataByStype(uid, begin, limit, types, render, view);
			for(int i = 0 ; i < contents.length; ++i){
                                FeedData temp = new FeedData();
                                temp.feed = contents[i].feed;
                                temp.source = contents[i].source;
                                temp.actor = contents[i].actor;
                                temp.weight = contents[i].weight;
                                temp.type = contents[i].type;
                                temp.time = contents[i].time;
                                byte[] b = QuickLZ.decompress(contents[i].xml);
                                temp.xml = new String(b, "utf-8");
                                list.add(temp);
                        }
                } catch (Exception e) {
                        String err = this.getClass().getName() + ".getNoEndFeedByTypes [" + prx + "] userid=" + uid;
                        Oce.getLogger().error(err + " " + e);
                        throw new RuntimeException(err + " " + e, e);
                }
                return list;
        }

        public Map<Integer, List<Integer> > getTimelineYearMonth(int uid) {
                Map<Integer, int[]> uidYearMonth;
                FeedViewPrx prx = null;
                try {
                        prx = getManager(uid);
                        uidYearMonth = prx.GetTimelineYearMonth(uid);
                } catch (Exception e) {
                        String err = this.getClass().getName() + ".getTimelineYearMonth [" + prx + "] uid=" + uid;
                        Oce.getLogger().error(err + " " + e);
                        throw new RuntimeException(err + " " + e, e);
                }
                Map<Integer, List<Integer> > res = new HashMap<Integer, List<Integer> >();
                for(Map.Entry<Integer, int[]> item : uidYearMonth.entrySet()) {
                        List<Integer> monthList = new ArrayList<Integer>();
                        int[] months = item.getValue();
                        for(int i = 0; i < months.length; ++i) {
                                monthList.add(months[i]);
                        }
                        res.put(item.getKey(), monthList);
                }
                return res;
         
        }

        public List<String> getTimelineData(int ownerid, int uid, String name, int year, int month,
                        int render, int begin, int limit) {
                String[] html = null;
                FeedViewPrx prx = null;
                try {
                        prx = getManager(uid);
                        html = prx.GetTimelineData(ownerid, uid, name, year, month, render, begin, limit);
                } catch (Exception e) {
                        String err = this.getClass().getName() + ".getTimelineData [" + prx + "] uid=" + uid;
                        Oce.getLogger().error(err + " " + e);
                        throw new RuntimeException(err + " " + e, e);
                }
                List<String> res = new ArrayList<String>();
                for (int i = 0; i < html.length; ++i) {
                        res.add(html[i]);
                }
                return res;
        }
        public List<String> getTimelineDataWithAdmin(int ownerid, int uid, 
            String name, int year, int month, int render, int begin, int limit, boolean isAdmin) {
                String[] html = null;
                FeedViewPrx prx = null;
                try {
                        prx = getManager(uid);
                        html = prx.GetTimelineDataWithAdmin(ownerid, uid, name, year, month, render, begin, limit, isAdmin);
                } catch (Exception e) {
                        String err = this.getClass().getName() + ".getTimelineData [" + prx + "] uid=" + uid;
                        Oce.getLogger().error(err + " " + e);
                        throw new RuntimeException(err + " " + e, e);
                }
                List<String> res = new ArrayList<String>();
                for (int i = 0; i < html.length; ++i) {
                        res.add(html[i]);
                }
                return res;
        }

	public List<String> getBySourceStypeAndActor(int uid, String name, long source, int stype, int actor, int render, int view) {
                String[] html = null;
                FeedViewPrx prx = null;
                try {
                        prx = getManager(uid);
                        html = prx.GetBySourceStypeAndActor(uid, name, source, stype, actor, render, view);
                } catch (Exception e) {
                        String err = this.getClass().getName() + ".getBySourceStypeAndActor [" + prx + "] uid=" + uid;
                        Oce.getLogger().error(err + " " + e); 
                        throw new RuntimeException(err + " " + e, e); 
                }
                List<String> res = new ArrayList<String>();
                for (int i = 0; i < html.length; ++i) {
                        res.add(html[i]);
                }
                return res;
        }
	
//	public static void main(String[] args) {
//		// TODO Auto-generated method stub
//
//		FeedViewHaAdapter.getInstance().getHot(22509273, "ld", 0,
//   			0, 20);
//	}
			
			
}
