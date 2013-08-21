package com.xiaonei.xce.feed;

import java.util.ArrayList;
import java.util.List;

import xce.feed.FeedData;
import xce.feed.FeedDataRes;
import xce.feed.FeedViewPrx;
import xce.feed.FeedViewPrxHelper;
import xce.util.ng.channel.ChannelFactory;
import xce.util.ng.channel.ChannelFactory.ChannelType;
import xce.util.ng.proxyholder.AbstractModProxyHolder;
import Ice.ObjectPrx;
import com.xiaonei.xce.log.Oce;

@Deprecated
public class FeedViewAdapter extends
		AbstractModProxyHolder<Integer, FeedViewPrx> {

	private static FeedViewAdapter instance;
	private static String endpoints = "@FeedViewN";

	public static FeedViewAdapter getInstance() {
		if (instance == null) {
			instance = new FeedViewAdapter();
		}
		return instance;
	}

	public FeedViewAdapter() {
		super(ChannelFactory.getChannel(ChannelType.XCEFEED), "feedview"
				+ endpoints, 10);
	}

	@Override
	protected int mod(Integer key, int size) {
		return key % size;
	}

	@Override
	public FeedViewPrx uncheckedCast(ObjectPrx baseProxy) {
		return FeedViewPrxHelper.uncheckedCast(baseProxy);
	}

	@Override
	protected ObjectPrx initProxy(ObjectPrx baseProxy) {
		return baseProxy;
	}

	private FeedViewPrx getManagerOneway(int uid) {
		return uncheckedCast(getProxy(uid).ice_oneway());
	}

	private FeedViewPrx getManager(int uid) {
		return getProxy(uid);
	}

	// uid, name 始终为访问者的

	// 由调用者传入name，可以减少一次远程调用
	@Deprecated
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
	@Deprecated
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
	@Deprecated
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
	@Deprecated
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
	@Deprecated
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
	@Deprecated
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
	@Deprecated
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
	@Deprecated
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
	@Deprecated
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
	@Deprecated
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
	@Deprecated
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
	@Deprecated
	public List<String> getHot(int owner, String ownerName, int view,
			int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetHot(owner, ownerName, view, begin, limit);
		} catch(Exception e) {
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
	@Deprecated
	public List<String> getHotByType(int owner, String ownerName, int view,
			int[] types, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetHotByType(owner, ownerName, view, types, begin, limit);
		} catch(Exception e) {
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
	@Deprecated
	public List<String> getHotByStype(int owner, String ownerName, int view,
			int[] stypes, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetHotByStype(owner, ownerName, view, stypes, begin, limit);
		} catch(Exception e) {
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
	@Deprecated
	public List<String> getLive(int owner, String ownerName, int view,
			int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetLive(owner, ownerName, view, begin, limit);
		} catch(Exception e) {
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
	@Deprecated
	public List<String> getLiveByStype(int owner, String ownerName, int view,
			int[] stypes, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetLiveByStype(owner, ownerName, view, stypes, begin, limit);
		} catch(Exception e) {
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
	@Deprecated
	public List<String> getLiveByType(int owner, String ownerName, int view,
			int[] types, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(owner);
			html = prx.GetLiveByType(owner, ownerName, view, types, begin, limit);
		} catch(Exception e) {
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
	@Deprecated
	public List<String> getMiniGroupFeedData(int gid, String name, int render,
			int memberid, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(gid);
			html = prx.GetMiniGroupFeedData(gid, name, render, memberid, begin, limit);
		} catch(Exception e) {
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
	@Deprecated
	public List<String> getMiniGroupFeedDataByType(int gid, String name, int render,
			int memberid, int[] types, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(gid);
			html = prx.GetMiniGroupFeedDataByType(gid, name, render, memberid, types, begin, limit);
		} catch(Exception e) {
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
	@Deprecated
	public List<String> getMiniGroupFeedDatabyStype(int gid, String name, int render,
			int memberid, int[] types, int begin, int limit) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(gid);
			html = prx.GetMiniGroupFeedDataByStype(gid, name, render, memberid, types, begin, limit);
		} catch(Exception e) {
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
	@Deprecated
	public List<String> getMiniGroupFeedData2(int gid, String name, int render,
			int memberid, int begin, int limit, boolean isAdmin) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(gid);
			html = prx.GetMiniGroupFeedData2(gid, name, render, memberid, begin, limit, isAdmin);
		} catch(Exception e) {
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
	@Deprecated
	public List<String> getMiniGroupFeedDataByType2(int gid, String name, int render,
			int memberid, int[] types, int begin, int limit, boolean isAdmin) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(gid);
			html = prx.GetMiniGroupFeedDataByType2(gid, name, render, memberid, types, begin, limit, isAdmin);
		} catch(Exception e) {
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
	@Deprecated
	public List<String> getMiniGroupFeedDatabyStype2(int gid, String name, int render,
			int memberid, int[] types, int begin, int limit, boolean isAdmin) {
		String[] html = null;
		FeedViewPrx prx = null;
		try {
			prx = getManager(gid);
			html = prx.GetMiniGroupFeedDataByStype2(gid, name, render, memberid, types, begin, limit, isAdmin);
		} catch(Exception e) {
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
	
	
	
	
//	public static void main(String[] args) {
//	// TODO Auto-generated method stub
//    int[] arr = new int[0];
//	List<FeedData> res = FeedViewAdapter.getInstance().getNewsDataByStype(225092473, "刘迪", 0, arr,
//			0, 20);
//	FeedData fd = res.get(0);
//	System.out.println("fd:" + fd.feed + " size:" + res.size());
//}
			
			
}
