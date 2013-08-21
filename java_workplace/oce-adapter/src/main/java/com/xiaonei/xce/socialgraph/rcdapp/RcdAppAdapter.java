package com.xiaonei.xce.socialgraph.rcdapp;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.RcdMessageServicePrx;
import xce.socialgraph.RcdMessageServicePrxHelper;
import xce.util.channel.Channel;

/**
 * Remove app recommendation in home
 * @author zhangnan
 */

public class RcdAppAdapter extends ReplicatedClusterAdapter {
  private static RcdAppAdapter instance = null;
  private static final String biz = "RCDAPP";
  private static final String ctrEndpoints = "ControllerSocialGraphRcdMessageService";
  private static int interval = 120;
  
  private RcdAppAdapter() {
	  super(ctrEndpoints, interval, Channel.newSocialGraphChannel(ctrEndpoints));
  }
  
  public static RcdAppAdapter getInstance() {
	  if (null == instance) {
		  synchronized (RcdAppAdapter.class) {
			  if (null == instance) {
				  try { 
					  instance = new RcdAppAdapter();					  
				  } catch(Exception e) {
					  e.printStackTrace();
					  instance = null;
				  }
			  }
			  
		  }
	  }
	  return instance;
  }

  //Remove application in home
	public void removeRcd(long source, long block) {
		getRcdMessageServicePrxOneWay(source).removeBizRcd(source, block, biz);	   //remove data in cache
	}	
	
	//Get oneway proxy
	private RcdMessageServicePrx getRcdMessageServicePrxOneWay(long source) {
		Ice.ObjectPrx prx0 = getValidProxy((int)source, 300);
		if (null != prx0) {
			RcdMessageServicePrx prx = RcdMessageServicePrxHelper.uncheckedCast(
					prx0.ice_timeout(300).ice_oneway());
		  	return prx;
		}
		return null;
	}
 
  
  //Get twoway proxy
  private RcdMessageServicePrx getRcdMessageServicePrxTwoWay(long source) {
	 Ice.ObjectPrx prx0 = getValidProxy((int)source, 300);
	 if (null != prx0) {
		 RcdMessageServicePrx prx = RcdMessageServicePrxHelper.uncheckedCast(
				 prx0.ice_timeout(300).ice_twoway());
		 return prx;
	 }
	 return null;
  }
	
  public static void main(String[] args) {
	  //int hostId = 2147463835;
	  int hostId = 238111132;
//	  int blockId = -2079367624;
	  int blockId = 122634;
	  RcdAppAdapter.getInstance().removeRcd(hostId,blockId);
//	  List<RcdSiteData> result = RcdSiteAdapter.getInstance().getRcdSite(hostId, -1);
//	  for (RcdSiteData data : result) {
//		  System.out.println(data.getSiteId() + " : " + data.getShareId() + " : " + data.getShareUserId() 
//				  + " : " + data.getShareCount());
//	  }
//	 System.out.println("result size: " + result.size());
	 System.exit(0);
  }
}
