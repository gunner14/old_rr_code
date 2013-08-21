package com.xiaonei.xce.feed;

import java.util.ArrayList;
import java.util.List;

import xce.feed.FeedData;
import xce.feed.FeedItemManagerPrx;
import xce.feed.FocusManagerPrx;
import xce.feed.FocusManagerPrxHelper;
import xce.util.ng.channel.ChannelFactory;
import xce.util.ng.channel.ChannelFactory.ChannelType;
import xce.util.ng.proxyholder.AbstractModProxyHolder;
import Ice.ObjectPrx;
import com.xiaonei.xce.log.Oce;

public class FeedFocusAdapter extends
		AbstractModProxyHolder<Integer, FocusManagerPrx> {

	private static FeedFocusAdapter instance;
	private static String endpoints = "@FeedFocus";

	public static FeedFocusAdapter getInstance() {
		if (instance == null) {
			instance = new FeedFocusAdapter();
		}
		return instance;
	}

	public FeedFocusAdapter() {
		super(ChannelFactory.getChannel(ChannelType.XCEFEED), "M" + endpoints,
				5);
	}

	@Override
	protected int mod(Integer key, int size) {
		return key % size;
	}

	@Override
	public FocusManagerPrx uncheckedCast(ObjectPrx baseProxy) {
		return FocusManagerPrxHelper.uncheckedCast(baseProxy);
	}

	@Override
	protected ObjectPrx initProxy(ObjectPrx baseProxy) {
		return baseProxy;
	}

	private FocusManagerPrx getManagerOneway(int uid) {
		return uncheckedCast(getProxy(uid).ice_oneway());
	}

	private FocusManagerPrx getManager(int uid) {
		return getProxy(uid);
	}

	public List<FeedData> getFocusFeedData(int user, int begin, int limit) {
		FeedData[] list = null;
		FocusManagerPrx prx = null;
		try {
			prx = getManager(user);
			list = prx.GetFocusFeedData(user, begin, limit);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getFocusFeedData [" + prx + "] userid=" + user;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<FeedData> res = new ArrayList<FeedData>();
		for(int i = 0; i < list.length; ++i){
			res.add(list[i]);
		}
		return res;
	}

	public int addFocuses(int user, int target) {
		int ret = -1;
		FocusManagerPrx prx = null;
		try{
			prx = getManager(user);
			ret = (prx.AddFocuses(user, target) == true ? 1: 0);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".addFocuses [" + prx + "] userid=" + user;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		return ret;
	}
	
	public int addFocusWithCheck(int user, int target) {
		int ret = -1;
		FocusManagerPrx prx = null;
		try {
			prx = getManager(user);
			ret = prx.AddFocusWithCheck(user, target);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".addFocusWithCheck [" + prx + "] userid=" + user;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		return ret;
	}

	public void delFocuses(int user, int target) {
		FocusManagerPrx prx = null;
		try {
			prx = getManagerOneway(user);
			prx.DelFocuses(user, target);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".delFocuses [" + prx + "] userid=" + user;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
	}
	
	public List<Integer> getFocuses(int user){
		int [] l = null;
		FocusManagerPrx prx = null;
		try {
			prx = getManager(user);
			l = prx.GetFocuses(user);
		} catch(Exception e) {
			String err = this.getClass().getName() + ".getFocuses [" + prx + "] userid=" + user;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err + " " + e, e);
		}
		List<Integer> res = new ArrayList<Integer>();
		for(int i = 0;i < l.length; ++i){
			res.add(l[i]);
		}
		return res;
	}
//	public static void main(String[] args) {
//		// TODO Auto-generated method stub
//		int res = FeedFocusAdapter.getInstance().addFocusesWithCheck(347348947, 347349486);
//		System.out.println("res:" + res );
//	}
}
