package com.xiaonei.xce.socialgraphcorelogic;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.ADDPAGE;
import xce.socialgraph.BUDDYAPPLY;
import xce.socialgraph.REFRESH;
import xce.socialgraph.SGMessage;
import xce.socialgraph.SocialGraphCoreLogicPrx;
import xce.socialgraph.SocialGraphCoreLogicPrxHelper;
import xce.socialgraph.USERLOGIN;
import xce.util.channel.Channel;

/**
 * Report SGMessage to centre
 * @author zhangnan
 *
 */
public class SocialGraphCoreLogicAdapter  extends ReplicatedClusterAdapter {
	private static SocialGraphCoreLogicAdapter instance = null;
  private static final String ctrEndpoints = "ControllerSocialGraphCoreLogic";
  private static int interval = 120;
  
  private SocialGraphCoreLogicAdapter() {
  	super(ctrEndpoints, interval, Channel.newSocialGraphChannel(ctrEndpoints));
  }
  
  public static SocialGraphCoreLogicAdapter getInstance() {
	  if (null == instance) {
		  synchronized (SocialGraphCoreLogicAdapter.class) {
			  if (null == instance) {
				  try { 
					  instance = new SocialGraphCoreLogicAdapter();					  
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
   * Report login message to centre
   * @param userId
   */
  public void ReportUserLogin(int userId) {
  	SGMessage message = new SGMessage();
  	message.id = userId;
  	message.action = USERLOGIN.value;
  	
  	getSocialGraphCoreLogicPrxOneWay(userId).SendSGMessage(message);
  }
  
  /**
   * Report applyfriends message to centres
   * @param userId
   * @param friendId
   */
  public void ReportBuddyApply(int userId, int friendId) {
  	SGMessage message = new SGMessage();
  	message.source = userId;
  	message.target = friendId;
  	message.action = BUDDYAPPLY.value;
  	
  	getSocialGraphCoreLogicPrxOneWay(userId).SendSGMessage(message);
  }
  
  /**
   * Report Add page message to centres
   * @param userId
   * @param pageId
   */
  public void ReportAddPage(int userId, long pageId) {
  	SGMessage message = new SGMessage();
  	message.source = userId;
  	message.target = pageId;
  	message.action = ADDPAGE.value;
  	
  	getSocialGraphCoreLogicPrxOneWay(userId).SendSGMessage(message);
  }
  
  /**
   * Report Refresh message to centres
   * @param userId
   */
  public void ReportRefresh(int userId) {
  	SGMessage message = new SGMessage();
  	message.id = userId;
  	message.action = REFRESH.value;
  	
  	getSocialGraphCoreLogicPrxOneWay(userId).SendSGMessage(message);
  }
  
	//Get oneway proxy
	private SocialGraphCoreLogicPrx getSocialGraphCoreLogicPrxOneWay(int userId) {
		Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
		if (null != prx0) {
			SocialGraphCoreLogicPrx prx = SocialGraphCoreLogicPrxHelper.uncheckedCast(
					prx0.ice_timeout(300).ice_oneway());
		  	return prx;
		}
		return null;
	}
  
  //Get twoway proxy
  private SocialGraphCoreLogicPrx getSocialGraphCoreLogicPrxTwoWay(int userId) {
	 Ice.ObjectPrx prx0 = getValidProxy(userId, 300);
	 if (null != prx0) {
		 SocialGraphCoreLogicPrx prx = SocialGraphCoreLogicPrxHelper.uncheckedCast(
				 prx0.ice_timeout(300).ice_twoway());
		 
		 return prx;
	 }
	 return null;
  }
  
  public static void main(String[] args) {
		int userId = 238111132;
		SocialGraphCoreLogicAdapter.getInstance().ReportUserLogin(userId);
		
		System.exit(0);
	}
}
