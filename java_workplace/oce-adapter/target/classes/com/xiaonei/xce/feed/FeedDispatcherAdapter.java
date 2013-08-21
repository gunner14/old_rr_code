package com.xiaonei.xce.feed;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.Iterator;


import xce.feed.FeedDispatcherPrx;
import xce.feed.FeedDispatcherPrxHelper;
import xce.feed.LoadType;
import xce.util.ng.channel.ChannelFactory;
import xce.util.ng.channel.ChannelFactory.ChannelType;
import xce.util.ng.proxyholder.AbstractModProxyHolder;
import Ice.ObjectPrx;

class FeedDispatcherAdapter  extends AbstractModProxyHolder<Integer, FeedDispatcherPrx>{

	private static FeedDispatcherAdapter instance;
	private static String endpoints = "@FeedDispatcher";

	public static FeedDispatcherAdapter getInstance() {
		if (instance == null) {
			instance = new FeedDispatcherAdapter();
		}
		return instance;
	}
	

	public FeedDispatcherAdapter() {
		super(ChannelFactory.getChannel(ChannelType.XCEFEED),
				"FD"+endpoints, 10);
	}
	@Override
	protected int mod(Integer key, int size) {
		return key % size;
	}

	@Override
	public FeedDispatcherPrx uncheckedCast(ObjectPrx baseProxy) {
		return FeedDispatcherPrxHelper.uncheckedCast(baseProxy);
	}

	@Override
	protected ObjectPrx initProxy(ObjectPrx baseProxy) {
		return baseProxy;
	}
	
	
	private FeedDispatcherPrx getManagerOneway(int uid) {
		return uncheckedCast(getProxy(uid).ice_oneway());
	}

	private FeedDispatcherPrx getManager(int uid) {
		return getProxy(uid);
	}
	
	
	public void joinPageNotify(int pageId, int fansId){
		getManagerOneway(pageId).joinPageNotify(pageId,fansId);
	}
	
	public void guideBuddyReplyNotify(int newUser, int friend){
		getManagerOneway(friend).guideBuddyReplyNotify(newUser,friend);
	}
	
	public void reload(LoadType  type , int id){
		for(int i = 0; i < 10; ++i){
			getManagerOneway(i).Reload(type, id);
		}
	}

	public void reload2(LoadType type, long id){
		for(int  i = 0; i < 10; ++i){
			getManagerOneway(i).Reload2(type, id);
		}
	}

	public void addMiniSiteFeed(int siteid, int userid) {
		getManagerOneway(siteid).addMiniSiteFeed(siteid, userid);
	}

	public void addMiniSiteFeedToGuides(long source, int type, int siteid, int... guides) {
                Map<Integer, List<Integer> > vmap = new HashMap<Integer, List<Integer> >();
		for (int i=0; i < guides.length; ++i) {
			if(vmap.get(guides[i] % 10) == null) {
				vmap.put(guides[i] % 10, new ArrayList<Integer>());
				vmap.get(guides[i] % 10).add(guides[i]);
			} else {
				vmap.get(guides[i] % 10).add(guides[i]);
			}
		}
		Iterator it = vmap.entrySet().iterator();
		while(it.hasNext()){
			Map.Entry<Integer, List<Integer> > entry = (Map.Entry<Integer, List<Integer> >)it.next();
			if(entry.getValue() != null) {
				int[] guideIds = new int[entry.getValue().size()];
				for(int i=0; i < guideIds.length; ++i) {
					guideIds[i] = entry.getValue().get(i);
				}
				getManagerOneway(entry.getKey()).addMiniSiteFeedToGuides(source, type, siteid, guideIds);
			}
		}
	}
}
