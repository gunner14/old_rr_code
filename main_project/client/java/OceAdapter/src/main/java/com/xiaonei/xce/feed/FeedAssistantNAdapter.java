package com.xiaonei.xce.feed;

import xce.feed.FeedAssistantPrx;
import xce.feed.FeedAssistantPrxHelper;
import xce.util.ng.channel.ChannelFactory;
import xce.util.ng.channel.ChannelFactory.ChannelType;
import xce.util.ng.proxyholder.AbstractModProxyHolder;
import Ice.ObjectPrx;

class  FeedAssistantNAdapter extends AbstractModProxyHolder<Integer, FeedAssistantPrx>{

	private static FeedAssistantNAdapter instance;
	private static String endpoints = "@FeedAssistantN";

	public FeedAssistantNAdapter() {
		super(ChannelFactory.getChannel(ChannelType.XCEFEED),
				"FA"+endpoints, 10);
	}

	public static FeedAssistantNAdapter getInstance() {
		if (instance == null) {
			instance = new FeedAssistantNAdapter();
		}
		return instance;
	}

	@Override
	protected int mod(Integer key, int size) {
		return key % size;
	}

	@Override
	public FeedAssistantPrx uncheckedCast(ObjectPrx baseProxy) {
		return FeedAssistantPrxHelper.uncheckedCast(baseProxy);
	}

	@Override
	protected ObjectPrx initProxy(ObjectPrx baseProxy) {
		return baseProxy;
	}
	
	private FeedAssistantPrx getManagerOneway(int source) {
		return uncheckedCast(getProxy(source).ice_oneway());
	}

	private FeedAssistantPrx getManager(int source) {
		return getProxy(source);
	}
}
