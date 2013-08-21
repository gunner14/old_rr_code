package com.xiaonei.talk;

import java.util.Vector;

import talk.window.IMWindowManagerPrx;
import talk.window.IMWindowManagerPrxHelper;
import xce.util.channel.Channel;
import xce.util.service.ServiceAdapter;

public class IMWindowAdapter extends ServiceAdapter {
	private static final String endpoints = "@IMWindow";
	private Vector<IMWindowManagerPrx> managersOneway = new Vector<IMWindowManagerPrx>();;
	private Vector<IMWindowManagerPrx> managersTwoway = new Vector<IMWindowManagerPrx>();;

	private static IMWindowAdapter instance = new IMWindowAdapter();

	public static IMWindowAdapter instance() {
		return instance;
	}

	public IMWindowAdapter() {
		super(endpoints, 1, Channel.newTalkChannel(endpoints));
		//super(endpoints, 1, Channel.newIMWindowChannel(endpoints));
	}

	public IMWindowManagerPrx getManager(int id) {
		return locateProxy(managersTwoway, "M", id, Channel.Invoke.Twoway,
				IMWindowManagerPrxHelper.class, 300);
	}
	
	public IMWindowManagerPrx getManagerOneway(int id) {
		return locateProxy(managersOneway, "M", id, Channel.Invoke.Oneway,
				IMWindowManagerPrxHelper.class, 300);
	}

  public void ShowWindow(String window_title, int uid) {
		getManagerOneway(uid).ShowWindow(window_title, uid);
  }

  public void ShowNotify(talk.window.NotifyInfo ni, int uid) {
		getManagerOneway(uid).ShowNotify(ni, uid);
  }
}
