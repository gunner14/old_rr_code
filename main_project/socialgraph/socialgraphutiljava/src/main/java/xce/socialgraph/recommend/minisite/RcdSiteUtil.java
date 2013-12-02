/*
 * 作者：熊君武
 * 邮件：junwu.xiong@renren-inc.com
 * 说明：本文件用于首页推荐之小站推荐
 * 时间：2011-12-22, 从XOA接口 
 * 2012-02-23，直接调用jar包
 */

package xce.socialgraph.recommend.minisite;

import java.util.ArrayList;
import java.util.List;


import junit.framework.Assert;
import net.paoding.rose.scanning.context.RoseAppContext;
import com.renren.sns.tag.service.suggest.SuggestMinisiteService;
import com.renren.sns.tag.service.site.SiteFollowerCountService;
import com.renren.sns.tag.view.render.SiteRender;
//import com.renren.xoa.lite.ServiceFactories;
//import com.renren.xoa.lite.ServiceFactory;
//import com.renren.xoa.lite.ServiceFuture;
//import com.renren.xoa.lite.ServiceFutureHelper;

/*
 * 该类提供向用户推荐的小站数据，包括小站名，小站id, 小站头像，小站url,小站关注人数
 */

public class RcdSiteUtil {
	private static RcdSiteUtil instance = null;
        private static SuggestMinisiteService sgservice = null;  //for fetching minisite information
        private static SiteFollowerCountService sfservice = null;  //for fetching minisite information
	private static final String biz = "RCDSITE";
	
	private RcdSiteUtil() {
		RoseAppContext rose = new RoseAppContext(); //for fetching minisite information
		//System.out.println("rose new");

	        //System.out.println("RcdSiteUtil instance create");
		sgservice = rose.getBean(SuggestMinisiteService.class);
	        // System.out.println("getBean");
		sfservice = rose.getBean(SiteFollowerCountService.class);
	        //System.out.println("create over");
	}
    
	public static RcdSiteUtil getInstance() {
		if (null == instance) {
			synchronized (RcdSiteUtil.class) {
				if (null == instance) {
					try {
						//RoseAppContext rose = new RoseAppContext(); //for fetching minisite information
						//System.out.println("rose new");
						//System.out.println("RcdSiteUtil instance create");
						//sgservice = rose.getBean(SuggestMinisiteService.class);
						//System.out.println("getBean");
						//sfservice = rose.getBean(SiteFollowerCountService.class);
						//System.out.println("create over");
						instance = new RcdSiteUtil();
					} catch(Exception e) {
						e.printStackTrace();
						instance = null;
					}
				}
			}
		}
		return instance;
	}
	
//    //获取推荐给每个用户的小站数据，包括小站名，小站id, 小站头像，小站url,小站关注人数
//	public List<RcdSiteData> getRcdSite(int userId, int limit) {
//		List<RcdSiteData> rcdSiteList = new ArrayList<RcdSiteData>();
//		//获取XOA ServiceFactory和Service实例
//		try {
//			final ServiceFactory fact = ServiceFactories.getFactory();
//			final com.renren.sns.minisite.xoa.client.SiteService service = fact
//					.getService(com.renren.sns.minisite.xoa.client.SiteService.class);
//			int count = 0;
//			if (limit <= 0) {
//				count = 200;
//			} else {
//				count = limit;
//			}
//
//			long timeoutMillis = 500;
//			ServiceFuture<SiteInfo[]> futureInfo = service.getSuggestSites(
//					userId, count);
//			SiteInfo[] siteInfo = ServiceFutureHelper.execute(futureInfo,
//					timeoutMillis);
//			if (null == siteInfo) {		// if null
//				return rcdSiteList;
//			}
//			//Assert.assertTrue(siteInfo.length > 0);
//			for (SiteInfo item : siteInfo) {
//				int siteId = item.getId();
//				RcdSiteData rcdSiteItem = new RcdSiteData();
//				// 设置用户名
//				rcdSiteItem.setUserId(userId);
//				// 获取小站名，小站id, 小站头像，小站url
//				rcdSiteItem.setSiteId(siteId);
//				rcdSiteItem.setHead(item.getTinyHead());
//				rcdSiteItem.setName(item.getName());
//				rcdSiteItem.setUrl(item.getUrl());
//
//				// 获取小站关注人数
//				ServiceFuture<Integer> futureCount = service
//						.getFollowerCount(siteId);
//				int siteFolllowerCount = ServiceFutureHelper.execute(
//						futureCount, timeoutMillis);
//				//Assert.assertTrue(siteFolllowerCount > 0);
//				rcdSiteItem.setFollowerCount(siteFolllowerCount);
//
//				rcdSiteList.add(rcdSiteItem);
//			}
//			
//		} catch (Exception e) {
//          e.printStackTrace();
//		}
//		
//		return rcdSiteList;
//	}
	

    //获取推荐给每个用户的小站数据，包括小站名，小站id, 小站头像，小站url,小站关注人数
	public List<RcdSiteData> getRcdSite(int userId, int limit) {
		List<RcdSiteData> rcdSiteList = new ArrayList<RcdSiteData>();
		//获取XOA ServiceFactory和Service实例
		try {
			int count = 0;
			if (limit <= 0) {
				count = 200;
			} else {
				count = limit;
			}

			List<SiteRender> siteRender = sgservice.getSuggestSitesExt(userId, count);			
			if (null == siteRender) {		// if null
				return rcdSiteList;
			}
			//Assert.assertTrue(siteInfo.length > 0);
			for (SiteRender item : siteRender) {
				int siteId = item.getId();
				RcdSiteData rcdSiteItem = new RcdSiteData();
				// 设置用户名
				rcdSiteItem.setUserId(userId);
				//System.out.println("userId: "+userId);
				// 获取小站名，小站id, 小站头像，小站url
				rcdSiteItem.setSiteId(siteId);
				//System.out.println("siteId: "+siteId);
				rcdSiteItem.setHead(item.getTinyHeadSource());
				//System.out.println("headurl: "+ item.getTinyHeadSource());
				rcdSiteItem.setName(item.getName());
				//System.out.println("name: "+ item.getName());
				rcdSiteItem.setUrl(item.getProfileUrl());
				//System.out.println("url: "+ item.getProfileUrl());

				// 获取小站关注人数
				int siteFollowerCount = sfservice.getFollowerCount(siteId);
				//System.out.println("count: "+ siteFollowerCount);
				//Assert.assertTrue(siteFolllowerCount > 0);
				rcdSiteItem.setFollowerCount(siteFollowerCount);

				rcdSiteList.add(rcdSiteItem);
			}
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return rcdSiteList;
	}	
	public static void main(String[] args) {
		int userId = 309372060;
		int limit = 2;		
		List<RcdSiteData> result = RcdSiteUtil.getInstance().getRcdSite(userId, limit);
		for (RcdSiteData item : result) {
			System.out.print(item.getUserId() + " : " + item.getSiteId() + " : " + item.getName() + 
					" : " + item.getHead() + " : " + item.getUrl() + " : " + item.getFollowerCount() + "\n");
		}
	}	
}
