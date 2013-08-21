package com.xiaonei.xce.notify;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.xiaonei.xce.log.Oce;

import xce._notify.NotifyContent;
import xce._notify.NotifyIndexManagerPrx;
import xce._notify.NotifyIndexManagerPrxHelper;
import xce.feed.FeedNewsManagerPrx;
import xce.util.ng.channel.ChannelFactory;
import xce.util.ng.channel.ChannelFactory.ChannelType;
import xce.util.ng.proxyholder.AbstractModProxyHolder;
import Ice.ObjectPrx;


class  NotifyIndexAdapter extends AbstractModProxyHolder<Integer, NotifyIndexManagerPrx>{

	private static NotifyIndexAdapter instance;
	private static String endpoints = "@NotifyIndex";

	private NotifyIndexAdapter() {
		super(ChannelFactory.getChannel(ChannelType.NOTIFY),
				"M"+endpoints, 5);
	}

	public static NotifyIndexAdapter getInstance() {
		if (instance == null) {
			instance = new NotifyIndexAdapter();
		}
		return instance;
	}

	@Override
	protected int mod(Integer key, int size) {
		return key % size;
	}

	@Override
	public NotifyIndexManagerPrx uncheckedCast(ObjectPrx baseProxy) {
		return NotifyIndexManagerPrxHelper.uncheckedCast(baseProxy);
	}

	@Override
	protected ObjectPrx initProxy(ObjectPrx baseProxy) {
		return baseProxy;
	}
	
	private NotifyIndexManagerPrx getManagerOneway(int uid) {
		return uncheckedCast(getProxy(uid).ice_oneway().ice_timeout(300));
	}

	private NotifyIndexManagerPrx getManager(int uid) {
		return uncheckedCast(getProxy(uid).ice_timeout(300));
	}
	
	void removeById(int uid, long notifyId)  {
		NotifyIndexManagerPrx prx = getManagerOneway(uid);
		try{
			prx.removeById(uid, notifyId);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".removeById [ " + prx + " ] uid=" + uid ;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err,e);
		}
	}
	void removeByIds(int uid, long[] notifyIds){
		NotifyIndexManagerPrx prx = getManagerOneway(uid);
		try{
			prx.removeByIds(uid, notifyIds);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".removeByIds [ " + prx + " ] uid=" + uid ;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err,e);
		}
	}
	void removeBySource(int uid, int type, long source) {
		NotifyIndexManagerPrx prx = getManagerOneway(uid);
		try{
			prx.removeBySource(uid, type,source);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".removeBySource [ " + prx + " ] uid=" + uid ;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err,e);
		}
	}
	
	int getCount(int uid, int[] types) {
		NotifyIndexManagerPrx prx = getManager(uid);
		try{
			return prx.getCount(uid, types);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getCount [ " + prx + " ] uid=" + uid ;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err,e);
		}
	}
	int getUnreadCountByTypes(int uid, int[] types) {
		NotifyIndexManagerPrx prx = getManager(uid);
		try{
			return prx.getCount(uid, types);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getUnreadCountByTypes [ " + prx + " ] uid=" + uid ;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err,e);
		}
	}
	
	List<Integer> getFromIdSeq(int user, int[] types, int begin, int limit){
		NotifyIndexManagerPrx prx = getManager(user);
		try{
			int [] ids =  prx.getFromIdSeq(user,types,begin, limit);
			List<Integer> res = new ArrayList<Integer>();
			for(int i = 0; i < ids.length; ++i){
				res.add(ids[i]);
			}
			return res;
		} catch (Exception e) {
			String err = this.getClass().getName() + ".getFromIdSeq [ " + prx + " ] uid=" + user ;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err,e);
		}
	}
	
	void removeBySourceAndSender(int uid, int type, long source, int sender) {
		NotifyIndexManagerPrx prx = getManagerOneway(uid);
		try{
			prx.removeBySourceAndSender(uid, type, source, sender);
		} catch (Exception e) {
			String err = this.getClass().getName() + ".removeBySourceAndSender [ " + prx + " ] uid=" + uid ;
			Oce.getLogger().error(err + " " + e);
			throw new RuntimeException(err,e);
		}
	}	
	public static void main(String[] args)  {
		int[] types = {16};
		int i = 0;
		while(i < 30){
			i = i + 1;
			try{
				int res = NotifyIndexAdapter.getInstance().getCount(220678672,types);
				System.out.println("res:"+ res);
			}catch(Exception e){
				//			e.printStackTrace(System.out);
			}
		}
	}
}
