package com.xiaonei.xce.socialgraph.rcdsite;

import xce.clusterstate.ReplicatedClusterAdapter;
import xce.socialgraph.RcdMessageServicePrx;
import xce.socialgraph.RcdMessageServicePrxHelper;
import xce.util.channel.Channel;

/*
 * 功能：首页推荐小站推荐缓存同步删除接口
 * 作者：熊君武
 * 时间：2012-01-11
 * 邮件：junwu.xiong@renren-inc.com
 * */

public class RcdSiteAdapter extends ReplicatedClusterAdapter {
  private static RcdSiteAdapter instance = null;
  private static final String biz = "RCDSITE";
  private static final String ctrEndpoints = "ControllerSocialGraphRcdMessageService";
  private static int interval = 120;
  
  private RcdSiteAdapter() {
	  super(ctrEndpoints, interval, Channel.newSocialGraphChannel(ctrEndpoints));
  }
  
  public static RcdSiteAdapter getInstance() {
	  if (null == instance) {
		  synchronized (RcdSiteAdapter.class) {
			  if (null == instance) {
				  try { 
					  instance = new RcdSiteAdapter();					  
				  } catch(Exception e) {
					  e.printStackTrace();
					  instance = null;
				  }
			  }
			  
		  }
	  }
	  return instance;
  }
  
//  public List<RcdSiteData> getRcdSite(int hostId, int limit) {
//	  List<RcdSiteData> result = new ArrayList<RcdSiteData>();
//	  byte[] data = BusinessCacheAdapter.getInstance().get(biz, hostId);
//	  try { 
//		  
//		  int count = 0;
//		  Items pbResult = Items.parseFrom(data);
//		  if (pbResult.getItemsCount() < limit || limit < 0) {
//			  count = pbResult.getItemsCount();
//		  } else {
//			  count = limit;
//		  }
//		  for (Item item : pbResult.getItemsList()) {
//			  if (--count < 0) {
//				  break;
//			  }
//			  result.add(new RcdSiteData(item));
//		  }
//	  } catch (InvalidProtocolBufferException e) {
//		  e.printStackTrace();
//	  }
//	  return result;
//  }
  	
  //删除推荐的minisite：首先删除cache,然后将删除的数据放入到block表中 
	public void removeRcd(long source, long block) {
		getRcdMessageServicePrxOneWay(source).removeBizRcd(source, block, biz);	//删除cache数据
		getRcdMessageServicePrxOneWay(source).addSgBlock(source, block, biz);	//加入socialgraph block表
	}	
	
	  //获取oneway代理
	private RcdMessageServicePrx getRcdMessageServicePrxOneWay(long source) {
		Ice.ObjectPrx prx0 = getValidProxy((int)source, 300);
		if (null != prx0) {
			RcdMessageServicePrx prx = RcdMessageServicePrxHelper.uncheckedCast(
					prx0.ice_timeout(300).ice_oneway());
		  	return prx;
		}
		return null;
	}
 
  
  //获取twoway代理
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
	  int hostId = 256404211;
//	  int blockId = -2079367624;
	  int blockId = 671014779;
	  RcdSiteAdapter.getInstance().removeRcd(hostId,blockId);
//	  List<RcdSiteData> result = RcdSiteAdapter.getInstance().getRcdSite(hostId, -1);
//	  for (RcdSiteData data : result) {
//		  System.out.println(data.getSiteId() + " : " + data.getShareId() + " : " + data.getShareUserId() 
//				  + " : " + data.getShareCount());
//	  }
//	 System.out.println("result size: " + result.size());
	 System.exit(0);
  }
}
