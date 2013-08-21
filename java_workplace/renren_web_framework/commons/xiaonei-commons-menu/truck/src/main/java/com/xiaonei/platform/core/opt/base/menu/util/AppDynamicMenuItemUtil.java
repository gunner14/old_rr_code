package com.xiaonei.platform.core.opt.base.menu.util;

import java.net.MalformedURLException;
import java.net.URL;

import com.xiaonei.platform.component.application.cache.AppBasicCacheMgr;
import com.xiaonei.platform.component.application.connect.utils.ConnectConstants;
import com.xiaonei.platform.component.application.home.AppAboutHome;
import com.xiaonei.platform.component.application.home.SpecialAppListHome;
import com.xiaonei.platform.component.application.model.AppAbout;
import com.xiaonei.platform.component.application.model.AppBasic;
import com.xiaonei.platform.component.application.model.SpecialApp;
import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.opt.OpiConstants;

/**
 * @author Zhang Tielei
 * 为AppMenuItem某些字段（bookmark url & urlTarget）做动态计算的工具类
 *
 */
public class AppDynamicMenuItemUtil {
	/**
	 * 将AppMenuItem对象中的动态字段（bookmark url & urlTarget）计算出来
	 * @param item
	 * @return 返回计算后的AppMenuItem对象
	 */
	public static AppMenuItem populateDynamic(AppMenuItem item) {
		int appId = item.getAppId();
		String sideNavUrl = item.getSideNavUrl();
		//动态计算bookmark url
		String bookmarkUrl = getDynamicBookmarkUrl(appId, sideNavUrl);
		//将动态计算出来的bookmark url重新存入AppMenuItem对象，注意，为了保持兼容性，是存在sideNavUrl字段
		item.setSideNavUrl(bookmarkUrl);
		//动态计算Bookmark URL的target属性，并重新存入AppMenuItem对象
		item.setUrlTarget(getUrlTargetFromBookmark(bookmarkUrl, appId));
		return item;
	}
	
	/**
	 * 动态计算Bookmark Url的算法，算法逻辑如下：
	 * 如果是WebCanvas应用，则先判断开发者有没有填写一个Bookmark Url（这个url实际保存在数据表app_menu_item的side_nav_url字段）；
	 * 如果是WapCanvas应用，则返回page页作为Bookmark Url；这是给web上用的策略，但是暂时无法区分是web环境还是wap环境。。。
	 * 如果是Connect应用，则返回Connect Url作为Bookmark Url；
	 * 如果是Desktop或MobileClient应用，则返回Install Url作为Bookmark Url。
	 * @return 动态的Bookmark Url，不确定则返回空串
	 */
	private static String getDynamicBookmarkUrl(int appId, String sideNavUrl) {
		String dynamicBookmark = "";
		AppBasic ab = AppBasicCacheMgr.getInstance().get(appId);
		if (ab != null) {
			if (ab.isWebCanvas()) {
				dynamicBookmark = sideNavUrl;
				if (sideNavUrl == null || sideNavUrl.length() == 0) {
					String canvasPageUrl = ab.getCanvasPageUrl();
					if (canvasPageUrl == null) {
						canvasPageUrl = "";
					}
					dynamicBookmark = OpiConstants.urlApps + "/" + canvasPageUrl;
				}
			}
			else if (ab.isWapCanvas()) {
				String canvasPageUrl = ab.getCanvasPageUrl();
				if (canvasPageUrl == null) {
					canvasPageUrl = "";
				}
				dynamicBookmark = OpiConstants.urlPage + "/application/" + appId;
			}
			else if(ab.isConnect()) {
//				AppConnect ac = AppConnectHome.getInstance().getAppConnect(appId);
//				if (ac != null) {
//					dynamicBookmark = ac.getConnect_url();
//				}
				dynamicBookmark = ConnectConstants.urlConnectWww() + "/redirect.do?type=appmenu&app_id=" + appId;
			}
			else if (ab.isDesktop() || ab.isMobileClient() || ab.isClient()){
				AppAbout about = AppAboutHome.getInstance().get(appId);
				if (about != null) {
					dynamicBookmark = about.getInstallUrl();
				}
			}
			else {
				//nothing to do, dynamicBookmark is empty string.
			}
		}
		return (dynamicBookmark != null) ? dynamicBookmark : "";
	}

	/**
	 * @return 根据bookmark url计算target属性，外站url返回"_blank"，否则返回null
	 * 在加载和更新AppMenuItem的时候被计算出来并缓存在urlTarget中
	 */
	private static String getUrlTargetFromBookmark(String bookmarkUrl) {
		if (bookmarkUrl == null || bookmarkUrl.length() == 0) {
			//防止空串抛异常和没必要的计算
			return null;
		}
		try {
			URL urlObj = new URL(bookmarkUrl);
			String authority = urlObj.getAuthority();
			return ((authority.endsWith("renren.com") && !authority.endsWith("connect.renren.com")) || authority.endsWith("kaixin.com")) ? null : "_blank";
		}
		catch (MalformedURLException e) {
			System.err.println("MalformedURLException: bookmarkUrl = " + bookmarkUrl);
			e.printStackTrace();
		}
		return null;
	}
	
	/**
	 * @return 根据bookmark url计算target属性，在白名单中的app点击时不会打开新窗口，否则链接地址会在新的
	 * 窗口打开
	 */
	private static String getUrlTargetFromBookmark(String bookmarkUrl, int appId) {
		if (bookmarkUrl == null || bookmarkUrl.length() == 0) {
			//防止空串抛异常和没必要的计算
			return null;
		}
		try {
			if(!SpecialAppListHome.getInstance().isSpecial(SpecialApp.TYPE_NOT_OPEN_WINDOW, appId)) {
				return "_blank";
			}
//			URL urlObj = new URL(bookmarkUrl);
//			String authority = urlObj.getAuthority();
//			return ((authority.endsWith("renren.com") && !authority.endsWith("connect.renren.com")) || authority.endsWith("kaixin.com")) ? null : "_blank";
		}
		catch (Exception e) {
			System.err.println("MalformedURLException: bookmarkUrl = " + bookmarkUrl);
			e.printStackTrace();
		}
		return null;
	}



}
