package com.xiaonei.xce.feed;

import java.util.Map;

import xce.edm.FeedEdmCachePrx;
import xce.edm.FeedEdmCachePrxHelper;
import xce.util.ng.channel.ChannelFactory;
import xce.util.ng.channel.ChannelFactory.ChannelType;
import xce.util.ng.proxyholder.AbstractModProxyHolder;
import Ice.ObjectPrx;

public class FeedEdmCacheAdapter extends
		AbstractModProxyHolder<Integer, FeedEdmCachePrx> {
	private static FeedEdmCacheAdapter instance;
	private static String endpoints = "@FeedEdmCache";

	public FeedEdmCacheAdapter() {
		super(ChannelFactory.getChannel(ChannelType.XCEFEED), "FEC"
				+ endpoints, 1);
	}

	public static FeedEdmCacheAdapter getInstance() {
		if (instance == null) {
			instance = new FeedEdmCacheAdapter();
		}
		return instance;
	}

	@Override
	protected int mod(Integer key, int size) {
		return key % size;
	}

	@Override
	public FeedEdmCachePrx uncheckedCast(ObjectPrx baseProxy) {
		return FeedEdmCachePrxHelper.uncheckedCast(baseProxy);
	}

	@Override
	protected ObjectPrx initProxy(ObjectPrx baseProxy) {
		return baseProxy;
	}

	private FeedEdmCachePrx getManagerOneway(int uid) {
		return uncheckedCast(getProxy(uid).ice_oneway());
	}

	private FeedEdmCachePrx getManager(int uid) {
		return getProxy(uid);
	}
  
  public void SetSendingFlag(int edm_id) throws Exception {
    getManagerOneway(edm_id).SetSendingFlag(edm_id);
  } 
  public void SetSuspendFlag(int edm_id) throws Exception {
    getManagerOneway(edm_id).SetSuspendFlag(edm_id);
  }
  public void UpdateEdmInfo(int edm_id)  throws Exception {
    getManagerOneway(edm_id).UpdateEdmInfo(edm_id);
  }
  public void SetForComplete()  throws Exception {
    getManager(0).SetForComplete();
  }
}
