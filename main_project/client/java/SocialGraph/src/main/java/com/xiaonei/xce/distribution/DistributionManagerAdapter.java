package com.xiaonei.xce.distribution;

import java.util.ArrayList;
import java.util.List;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.DistributionManagerPrx;
import xce.socialgraph.DistributionManagerPrxHelper;
import xce.socialgraph.RecommendItem;
import xce.util.channel.Channel;

/**
 * DistributionManager
 * @author zhangnan
 *
 */
public class DistributionManagerAdapter extends ReplicatedClusterAdapter {
	private static DistributionManagerAdapter instance = null;
  private static final String ctrEndpoints = "ControllerDistributionManager";
  private static int interval = 120;
  
  private DistributionManagerAdapter() {
	  super(ctrEndpoints, interval, Channel.newSocialGraphChannel(ctrEndpoints));
  }
  
  public static DistributionManagerAdapter getInstance() {
	  if (null == instance) {
		  synchronized (DistributionManagerAdapter.class) {
			  if (null == instance) {
				  try { 
					  instance = new DistributionManagerAdapter();					  
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
   * getFriendRecommendByHome
   * @param userId
   * @param limit
   * @return
   */
  public List<RecommendItem> getFriendRecommendByHome(int userId, int limit) {
  	RecommendItem[] itemArray = getDistributionManagerPrxTwoWay(userId).GetFriendRecommendByHome(userId, limit);
  	List<RecommendItem> result = new ArrayList<RecommendItem>();
  	for (RecommendItem item : itemArray) {
  		result.add(item);
  	}
  	return result;
  }
  
	//Get oneway proxy
	private DistributionManagerPrx getDistributionManagerPrxOneWay(int userId) {
		Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
		if (null != prx0) {
			DistributionManagerPrx prx = DistributionManagerPrxHelper.uncheckedCast(
					prx0.ice_timeout(300).ice_oneway());
		  	return prx;
		}
		return null;
	}
 
  
  //Get twoway proxy
  private DistributionManagerPrx getDistributionManagerPrxTwoWay(int userId) {
	 Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
	 if (null != prx0) {
		 DistributionManagerPrx prx = DistributionManagerPrxHelper.uncheckedCast(
				 prx0.ice_timeout(300).ice_twoway());
		 
		 return prx;
	 }
	 return null;
  }
  
  public static void main(String[] args) {
//  	int userId = 230618710;
//  	int userId = 256404211;
		//int userId = 238111132;
//  	int userId = 235769293;
//  	int userId = 239077714;
//  	int userId = 222880825;
//  	int userId = 222789656;
//  	int userId = 194305277;
//  	int userId = 221411118;
  	int userId = 200039999;
		int limit = -1;
		List<RecommendItem> result = DistributionManagerAdapter.getInstance().getFriendRecommendByHome(userId, limit);
		
		for (RecommendItem item : result) {
			System.out.println(item.id + ":" + item.type + ":" + item.experiment);
		}
		
		System.exit(0);
	}
}
