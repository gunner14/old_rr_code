package com.xiaonei.xce.socialgraph.recommendationmanager;

import java.util.ArrayList;
import java.util.List;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.RecommendItem;
import xce.socialgraph.RecommendationManagerPrx;
import xce.socialgraph.RecommendationManagerPrxHelper;
import xce.util.channel.Channel;

/**
 * Adapter of recommendation platform 
 * @author zhangnan
 */
public class RecommendationManagerAdapter extends ReplicatedClusterAdapter {
  private static RecommendationManagerAdapter instance = null;
  private static final String ctrEndpoints = "ControllerRecommendationManager";
  private static int interval = 120;
  
  private RecommendationManagerAdapter() {
	  super(ctrEndpoints, interval, Channel.newSocialGraphChannel(ctrEndpoints));
  }
  
  public static RecommendationManagerAdapter getInstance() {
	  if (null == instance) {
		  synchronized (RecommendationManagerAdapter.class) {
			  if (null == instance) {
				  try { 
					  instance = new RecommendationManagerAdapter();					  
				  } catch(Exception e) {
					  e.printStackTrace();
					  instance = null;
				  }
			  }
			  
		  }
	  }
	  return instance;
  }
  
  /**
   * Get friend recommendation in home
   * @param userId
   * @param limit
   * @return
   */
  public List<RecommendItem> getFriendRecommendByHome(int userId, int limit) {
  	List<RecommendItem> result = new ArrayList<RecommendItem>();
  	
  	RecommendItem[] itemArray = getRecommendationManagerPrxTwoWay(userId).GetFriendRecommendByHome(userId, limit);
  	for (RecommendItem item : itemArray) {
  		result.add(item);
  	}
  	return result;
  }
  
  //Get twoway proxy
  private RecommendationManagerPrx getRecommendationManagerPrxTwoWay(int userId) {
  	Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
  	if (null != prx0) {
  		RecommendationManagerPrx prx = RecommendationManagerPrxHelper.uncheckedCast(
					prx0.ice_timeout(300).ice_twoway());
		  	return prx;
		}
		return null;
  }
  
  public static void main(String[] args) {
		int userId = 238111132;
		int limit = -1;
		List<RecommendItem> result = RecommendationManagerAdapter.getInstance().getFriendRecommendByHome(userId, limit);
		for (RecommendItem ii : result) {
			System.out.println(ii.id);
			System.out.println(ii.count);
			System.out.println(ii.experiment);
		}
		System.exit(0);
	}
}
