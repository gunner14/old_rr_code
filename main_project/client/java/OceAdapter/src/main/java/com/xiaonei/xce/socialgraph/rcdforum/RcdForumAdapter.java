package com.xiaonei.xce.socialgraph.rcdforum;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.RcdMessageServicePrx;
import xce.socialgraph.RcdMessageServicePrxHelper;
import xce.util.channel.Channel;

/**
 * Remove forum recommendation in home
 * @author zhangnan
 */
public class RcdForumAdapter extends ReplicatedClusterAdapter {
  private static RcdForumAdapter instance = null;
  private static final String biz = "RCDFORUM";
  private static final String ctrEndpoints = "ControllerSocialGraphRcdMessageService";
  private static int interval = 120;
  
  private RcdForumAdapter() {
	  super(ctrEndpoints, interval, Channel.newSocialGraphChannel(ctrEndpoints));
  }
  
  public static RcdForumAdapter getInstance() {
	  if (null == instance) {
		  synchronized (RcdForumAdapter.class) {
			  if (null == instance) {
				  try { 
					  instance = new RcdForumAdapter();					  
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
   * Remove forum recommend in home recommendation
   * @param source	hostid
   * @param block	blockid
   */
	public void removeRcd(long source, long block) {
		getRcdMessageServicePrxOneWay(source).removeBizRcd(source, block, biz);	   //remove data in cache
	}	
	
	/**
	 * Get one way proxy
	 * @param source
	 * @return
	 */
	private RcdMessageServicePrx getRcdMessageServicePrxOneWay(long source) {
		Ice.ObjectPrx prx0 = getValidProxy((int)source, 300);
		if (null != prx0) {
			RcdMessageServicePrx prx = RcdMessageServicePrxHelper.uncheckedCast(
					prx0.ice_timeout(300).ice_oneway());
		  	return prx;
		}
		return null;
	}
 
  
  /**
   * Get two way proxy
   * @param source
   * @return
   */
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
	  int blockId = 100090;
	  RcdForumAdapter.getInstance().removeRcd(hostId,blockId);
//	  List<RcdSiteData> result = RcdSiteAdapter.getInstance().getRcdSite(hostId, -1);
//	  for (RcdSiteData data : result) {
//		  System.out.println(data.getSiteId() + " : " + data.getShareId() + " : " + data.getShareUserId() 
//				  + " : " + data.getShareCount());
//	  }
//	 System.out.println("result size: " + result.size());
	 System.exit(0);
  }
}
