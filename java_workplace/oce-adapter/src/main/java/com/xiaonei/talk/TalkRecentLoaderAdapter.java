package com.xiaonei.talk;

import java.util.Vector;

import com.xiaonei.talk.recent.loader.RecentLoaderManagerPrx;
import com.xiaonei.talk.recent.loader.RecentLoaderManagerPrxHelper;
import com.xiaonei.talk.recent.loader.TalkRecentUser;
import xce.util.channel.Channel;
import xce.util.service.ServiceAdapter;

public class TalkRecentLoaderAdapter extends ServiceAdapter {
	private static final String endpoints = "@TalkRecentLoader";
	private Vector<RecentLoaderManagerPrx> managersOneway = new Vector<RecentLoaderManagerPrx>();

	private static TalkRecentLoaderAdapter instance = new TalkRecentLoaderAdapter();

	public static TalkRecentLoaderAdapter instance() {
		return instance;
	}

	TalkRecentLoaderAdapter() {
		super(endpoints, 10, Channel.newTalkChannel(endpoints));

	}
	
	public RecentLoaderManagerPrx getManagerOneway(int id) {
		return locateProxy(managersOneway, "M", id, Channel.Invoke.Oneway,
				RecentLoaderManagerPrxHelper.class, 300);
	}
	
	public RecentLoaderManagerPrx getManager(int id) {
		return locateProxy(managersOneway, "M", id, Channel.Invoke.Twoway,
				RecentLoaderManagerPrxHelper.class, 300);
	}
	public void Update(int actor, TalkRecentUser groupinfo){
		RecentLoaderManagerPrx p = getManagerOneway(actor);
		p.Update(actor, groupinfo);
	}
	public void Delete(int actor, int id, int type){
		RecentLoaderManagerPrx p = getManagerOneway(actor);
		p.Delete(actor, id, type);
	}

}
