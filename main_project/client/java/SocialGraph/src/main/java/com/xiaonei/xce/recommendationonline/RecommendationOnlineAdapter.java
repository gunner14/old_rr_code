package com.xiaonei.xce.recommendationonline;

import java.util.ArrayList;
import java.util.List;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.RecommendItem;
import xce.socialgraph.RecommendationManagerPrx;
import xce.socialgraph.RecommendationManagerPrxHelper;
import xce.util.channel.Channel;

public class RecommendationOnlineAdapter extends ReplicatedClusterAdapter {
	private static RecommendationOnlineAdapter instance = null;
	private static final String ctrEndpoints = "ControllerRecommendationOnline";
	private static int interval = 120;
	private RecommendationOnlineAdapter() {
		super(ctrEndpoints, interval, Channel.newSocialGraphChannel(ctrEndpoints));
	}
	
	public static RecommendationOnlineAdapter getInstance() {
		if(null == instance) {
			 synchronized (RecommendationOnlineAdapter.class) {
				 if (null == instance) {
						try {
							instance = new RecommendationOnlineAdapter();
						} catch(Exception e) {
							e.printStackTrace();
							instance = null;
						}
					}
			 }
		}
		return instance;
	}
	public List<RecommendItem> getFriendRecommendByHome(int userId, int limit) {
		List<RecommendItem> result = new ArrayList<RecommendItem>();
		RecommendItem[] itemArray = getRecommendationManagerPrxTwoWay(userId).GetFriendRecommendByHome(userId, limit);
		for (RecommendItem item : itemArray)
		{
			result.add(item);
		}
		return result;
	}
	
	//Get Twoway Proxy
	private RecommendationManagerPrx getRecommendationManagerPrxTwoWay(int userId) {
		Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
		if(null != prx0) {
			RecommendationManagerPrx prx = RecommendationManagerPrxHelper.uncheckedCast(prx0.ice_timeout(300).ice_twoway());
			return prx;
		}
		return null;
	}
	
	 public static void main(String[] args) {
			int userId = 232491085;
			int limit = -1;
			List<RecommendItem> result = RecommendationOnlineAdapter.getInstance().getFriendRecommendByHome(userId, limit);
			for (RecommendItem ii : result) {
				System.out.println(ii.id);
				System.out.println(ii.count);
			}
			System.exit(0);
		}
}