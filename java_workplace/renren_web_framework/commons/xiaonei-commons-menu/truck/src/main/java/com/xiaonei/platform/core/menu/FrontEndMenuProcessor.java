package com.xiaonei.platform.core.menu;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.platform.component.application.cache.AppBasicCacheMgr;
import com.xiaonei.platform.component.application.cache.AppHomeMenuCacheUtil;
import com.xiaonei.platform.component.application.cache.XCacheTopMenu;
import com.xiaonei.platform.component.application.home.SpecialAppListHome;
import com.xiaonei.platform.component.application.jade.home.RecommendedAppHome;
import com.xiaonei.platform.component.application.menu.model.RecommendedApp;
import com.xiaonei.platform.component.application.model.AppBasic;
import com.xiaonei.platform.component.application.model.SpecialApp;
import com.xiaonei.platform.core.util.IPCheckTool;
import com.xiaonei.platform.core.util.RenRenAppIdCacheMgr;

/**
 * 从Menu相关的后端服务中拿到菜单数据后，放置到前端之前，再做一次处理
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2011-3-28 上午11:20:56
 */
public class FrontEndMenuProcessor {

	protected Log logger = LogFactory.getLog(this.getClass());

	public static final int DISPLAY_THRESHOLD = 20;
	
	private final static int[] EMPTY_ARRAY = new int[0];

	public UserMenu process(UserMenu userMenu) {
		int[] needPresentAppIds = SpecialAppListHome.getInstance().getAppIdsByType(
				SpecialApp.TYPE_STAY_IN_RECENT_MENU);
		List<Integer> ugcAappIds = RecommendedAppHome.getInstance().getAppsByType(
				RecommendedApp.LEFT_UGC_MENU);
		int[] recentAppIds = userMenu.getRecentAppIds();

		int[] union = union(recentAppIds, needPresentAppIds);
		int[] afterClear = clearUgcApp(union, ugcAappIds);
		if (logger.isDebugEnabled()) {
			logger.debug("recent menu after process: " + Arrays.toString(union));
		}
		userMenu.setRecentAppIds(afterClear); //回写
		return userMenu;
	}

	/**
	 * 将userMenu 的最近使用分割成 一般应用和最新连接两个部分
	 * 
	 * @param userMenu
	 * @return
	 */
	public UserMenu processRecentApp(UserMenu userMenu) {
		if (userMenu == null) {
			return null;
		}
		if (!userMenu.isNewVersion()) {
			userMenu.setConnectAppIds(new int[0]);
			return userMenu;
		}

		int[] recentAppIds = userMenu.getRecentAppIds();
		ClassifyApps cas = getClassifyApps(recentAppIds);
		userMenu.setRecentAppIds( cas.webCavnasApps);
		userMenu.setConnectAppIds( cas.connectApps );
		return userMenu;
	}

	public UserMenu processWithIP(UserMenu userMenu, String ip) {
		boolean isbj = IPCheckTool.getInstance().isBJIP(ip);
		int[] needPresentAppIds = SpecialAppListHome.getInstance().getAppIdsByType(
				SpecialApp.TYPE_STAY_IN_RECENT_MENU);
		int[] recentAppIds = userMenu.getRecentAppIds();

		int[] union = union(recentAppIds, needPresentAppIds);
		union = clearArray(union, isbj);
		if (logger.isDebugEnabled()) {
			logger.debug("recent menu after process: " + Arrays.toString(union));
		}
		userMenu.setRecentAppIds(union); //回写
		return userMenu;
	}

	/**
	 * 处理首页左侧菜单置顶应用及第二推荐位
	 * 
	 * @param userMenu
	 * @param userId
	 * @return
	 */
	public UserMenu processTopMenu(UserMenu userMenu, int userId) {

		//处理首页置顶应用，将需要置顶的应用依次与最近使用以及常用app集合作比较 如果不在最近使用和常用中 则放在置顶列表中
		List<Integer> apps = RecommendedAppHome.getInstance().getAppsByType(
				RecommendedApp.LEFT_TOP_MENU);
		List<Integer> topApps = new ArrayList<Integer>();
		//		int[] topAppIds = SpecialAppListHome.getInstance().getAppIdsByType(SpecialApp.TYPE_TOP_MENU);
		if (apps != null && apps.size() > 0) {
			for (Integer appId : apps) {
				if (!isInLeftMenu(userMenu, appId, userId)) {
					topApps.add(appId);
				}
			}
			if (topApps.size() > 0) {
				int[] topAppArray = new int[topApps.size()];
				for (int i = 0; i < topApps.size(); i++) {
					topAppArray[i] = topApps.get(i);
				}
				userMenu.setTopAppIds(topAppArray);
			}
		}

		//处理位于置顶应用下方的第二推荐位
		int[] secondAppIds = SpecialAppListHome.getInstance().getAppIdsByType(
				SpecialApp.TYPE_LEFT_SECOND_TOP_MENU);
		if (secondAppIds != null && secondAppIds.length > 0) {
			int secondApp = secondAppIds[0];
			//判断该app是否在用户收藏菜单中，如果在则直接返回
			if (!isAppInArray(secondApp, userMenu.getFavoriteAppIds())) {
				if (XCacheTopMenu.get(secondApp, userId) == null) {
					userMenu.setSecondTopAppIds(new int[] { secondApp });
				}
			}
		}

		//首页左侧菜单推广位3
		List<Integer> thirdApps = RecommendedAppHome.getInstance().getAppsByType(
				RecommendedApp.LEFT_TOP_THIRD_MENU);
		if (thirdApps != null && thirdApps.size() > 0) {
			int thdApp = thirdApps.get(0);
			String key = AppHomeMenuCacheUtil.generateKey(AppHomeMenuCacheUtil.THD_PROFIX, userId);
			Object obj = AppHomeMenuCacheUtil.get(key);
			if (obj != null && !isAppInArray(thdApp, userMenu.getFavoriteAppIds())) {
				userMenu.setThirdTopAppIds(new int[] { thdApp });
			}
		}

		//首页左侧菜单推广位4
		List<Integer> fourthApps = RecommendedAppHome.getInstance().getAppsByType(
				RecommendedApp.LEFT_TOP_FOURTH_MENU);
		if (fourthApps != null && fourthApps.size() > 0) {
			int fourthApp = fourthApps.get(0);
			String key = AppHomeMenuCacheUtil.generateKey(AppHomeMenuCacheUtil.FOUR_PROFIX, userId);
			Object obj = AppHomeMenuCacheUtil.get(key);
			if (obj != null && !isAppInArray(fourthApp, userMenu.getFavoriteAppIds())) {
				userMenu.setFourthTopAppIds(new int[] { fourthApp });
			}
		}

		return userMenu;
	}

	/**
	 * 判断某个应用是否在左侧菜单中以及删除缓存记录中
	 * 
	 * @param userMenu
	 * @param appId
	 * @param userId
	 * @return
	 */
	public boolean isInLeftMenu(UserMenu userMenu, int appId, int userId) {
		if (userMenu == null) {
			return true;
		}
		//判断是否在收藏菜单中，如果在则直接返回
		if (isAppInArray(appId, userMenu.getFavoriteAppIds())) {
			return true;
		}
		//判断是否在最近使用菜单中，如果在则直接返回
		if (isAppInArray(appId, userMenu.getRecentAppIds())) {
			return true;
		}
		//判断用户是否删除过该置顶应用，不再则添加到首页置顶
		if (XCacheTopMenu.get(appId, userId) != null) {
			return true;
		}
		return false;
	}

	/**
	 * 
	 * @param userMenu
	 * @param userId
	 * @return
	 */
	public UserMenu processTopMenu1(UserMenu userMenu, int userId) {
		int[] topAppIds = SpecialAppListHome.getInstance()
				.getAppIdsByType(SpecialApp.TYPE_TOP_MENU);
		if (topAppIds == null || topAppIds.length == 0) {
			return userMenu;
		}
		int topAppId = topAppIds[0];

		//判断是否在收藏菜单中，如果在则直接返回
		int[] favoriteAppIds = userMenu.getFavoriteAppIds();
		if (isAppInArray(topAppId, favoriteAppIds)) {
			return userMenu;
		}

		//判断是否在最近使用菜单中，如果在则删除，并将此app放到topAppIds中
		int[] recentAppIds = userMenu.getRecentAppIds();
		if (isAppInArray(topAppId, recentAppIds)) {
			ArrayList<Integer> recentTemp = new ArrayList<Integer>();
			for (int appId : recentAppIds) {
				if (topAppId != appId) {
					recentTemp.add(appId);
				}
			}
			if (recentTemp.size() > 0) {
				int[] appTemp = new int[recentTemp.size()];
				for (int i = 0; i < recentTemp.size(); i++) {
					appTemp[i] = recentTemp.get(i);
				}
				userMenu.setRecentAppIds(appTemp);
				userMenu.setTopAppIds(new int[] { topAppId });
			}
			return userMenu;
		}

		//如果左侧菜单中没有该应用，则从cache中读取，如果有数据则表明该置顶应用已被用户删除过，不添加到置顶

		ArrayList<Integer> topApps = new ArrayList<Integer>();
		if (favoriteAppIds != null && favoriteAppIds.length > 0) {
			for (int appid : topAppIds) {
				if (!isAppInArray(appid, favoriteAppIds)) {
					topApps.add(appid);
				}
			}
		}

		return null;
	}

	/**
	 * 这函数作用是去重，如果app出现在收藏菜单,以及置顶中，则不再出现在最近使用菜单中
	 * 
	 * @param userMenu
	 * @return
	 */
	public UserMenu processNoRepeat(UserMenu userMenu) {

		int[] recentAppIds = userMenu.getRecentAppIds();
		int[] favoriteAppIds = userMenu.getFavoriteAppIds();
		int[] ugcAppIds = userMenu.getUgcAppIds();
		int[] allTopAppIds = userMenu.getAllTopAppIds();

		//recent menu 不包含站内功能和fav,以及置顶应用 ，所以都要过滤
		int[] toRemoveIds = ArrayUtils.addAll(favoriteAppIds, allTopAppIds);
		
		int[] recentRemoveIds = ArrayUtils.addAll(toRemoveIds, getRenRenApps(userMenu));
		int[] newRecentIds = filterArray2(recentAppIds, recentRemoveIds);
		
		//ugc menu包含fav，置顶应用,单独过滤下
		int[] newUgcIds = filterArray2(ugcAppIds, toRemoveIds);
		
		userMenu.setRecentAppIds(newRecentIds);
		userMenu.setUgcAppIds(newUgcIds);
		return userMenu;
	}

	/**
	 * 需要过滤的人人应用（除去connect类型的）
	 * @param userMenu
	 * @return
	 */
	private int[] getRenRenApps(UserMenu userMenu) {
		int[] renrenApps = new int[ 0 ];
		
		if (userMenu.isNewVersion()) {
			renrenApps = RenRenAppIdCacheMgr.getInstance().getRenRenAppsExceptConnect();
		}
		return renrenApps;
	}

	private int[] filterArray2(int[] array1, int[] array2) {
		if(array1 == null || array1.length == 0 || array2 == null || array2.length == 0){
			return array1;
		}
		
		List<Integer> recent = new ArrayList<Integer>();
		for (int appid : array1) {
			if (!isAppInArray(appid, array2)) {
				recent.add(appid);
			}
		}

		int offset = 0;
		int[] ret = new int[recent.size()];
		for (int appId : recent) {
			ret[offset++] = appId;
		}
		return ret;
	}

	/**
	 * 将北京用户的视频菜单或非北京用户的人人视频菜单清除
	 * 
	 * @param appList
	 * @param isbj
	 * @return
	 */
	int[] clearArray(int[] appList, boolean isbj) {
		if (appList == null || appList.length == 0) return null;
		ArrayList<Integer> apps = new ArrayList<Integer>();
		//只有北京用户才显示的app列表
		int[] bjApp = SpecialAppListHome.getInstance().getAppIdsByType(SpecialApp.TYPE_BJ_USER_APP);
		//只有非北京用户才显示的app列表
		int[] nbjapp = SpecialAppListHome.getInstance().getAppIdsByType(
				SpecialApp.TYPE_NOT_BJ_USER_APP);

		for (int i = 0; i < appList.length; i++) {
			if (isbj && isAppInArray(appList[i], nbjapp)) {
				continue;
			}
			if (!isbj && isAppInArray(appList[i], bjApp)) {
				continue;
			}
			apps.add(appList[i]);
		}

		int[] ret = new int[apps.size()];
		int offset = 0;
		for (int appId : apps) {
			ret[offset++] = appId;
		}
		return ret;
	}

	/**
	 * 如果apps中包含appId，则去掉
	 * 
	 * @param apps
	 * @param appId
	 * @return
	 */
	public int[] clearApp(int[] apps, int appId) {
		if (!isAppInArray(appId, apps)) {
			return apps;
		} else {
			int[] tmp = new int[apps.length - 1];
			int index = 0;
			for (int i = 0; i < apps.length; i++) {
				if (appId != apps[i]) {
					tmp[index] = apps[i];
					index++;
				}
			}
			return tmp;
		}

	}

	//判断某个appId是否在app列表中
	boolean isAppInArray(int appId, int[] apps) {
		if (apps == null || apps.length == 0) return false;
		boolean flag = false;
		for (int i : apps) {
			if (appId == i) {
				flag = true;
				break;
			}
		}
		return flag;
	}

	/**
	 * 清除ugc应用
	 * 
	 * @param recentAppIds
	 * @param needPresentAppIds
	 * @return
	 */
	public int[] clearUgcApp(int[] recentAppIds, List<Integer> ugcApps) {
		if (recentAppIds == null || recentAppIds.length == 0) {
			return null;
		}
		if (ugcApps == null || ugcApps.size() == 0) {
			return recentAppIds;
		}
		List<Integer> recentAppIdList = new ArrayList<Integer>();
		for (int appId : recentAppIds) {
			if (!ugcApps.contains(appId)) {
				recentAppIdList.add(appId);
			}
		}
		if (recentAppIdList.size() != 0) {
			int[] tmp = new int[recentAppIdList.size()];
			for (int i = 0; i < recentAppIdList.size(); i++) {
				tmp[i] = recentAppIdList.get(i);
			}
			return tmp;
		}
		return recentAppIds;
	}

	int[] union(int[] recentAppIds, int[] needPresentAppIds) {

		if (needPresentAppIds == null || needPresentAppIds.length > DISPLAY_THRESHOLD) {
			//不合法的状态，为了容错
			return recentAppIds;
		}

		if (recentAppIds == null || recentAppIds.length == 0) {
			return needPresentAppIds;
		}

		if (needPresentAppIds.length == 0) {
			return recentAppIds;
		}

		Set<Integer> needPresentAppIdSet = new TreeSet<Integer>(); //用TreeSet是为了保证有序
		for (int appId : needPresentAppIds) {
			needPresentAppIdSet.add(appId);
		}

		List<Integer> recentAppIdList = new ArrayList<Integer>(DISPLAY_THRESHOLD
				+ needPresentAppIds.length);
		for (int i = 0; i < recentAppIds.length; i++) {

			if (i >= DISPLAY_THRESHOLD) { //截断
				break;
			}
			int appId = recentAppIds[i];
			if (needPresentAppIdSet.contains(appId)) {
				needPresentAppIdSet.remove(appId);
			}
			recentAppIdList.add(appId);
		}

		//set中剩下的补在后面
		for (int appId : needPresentAppIdSet) {
			recentAppIdList.add(appId);
		}
		int[] ret = new int[recentAppIdList.size()];
		int offset = 0;
		for (int appId : recentAppIdList) {
			ret[offset++] = appId;
		}
		return ret;
	}

	private ClassifyApps getClassifyApps(int[] recentApps) {
		if (recentApps == null) {
			return new ClassifyApps( EMPTY_ARRAY , EMPTY_ARRAY);
		}

		int[] connectAppIds = new int[recentApps.length];
		int[] webCanvasApps = new int[recentApps.length];
		int connectOffset = 0;
		int webOffset = 0;
		
		for (int appId : recentApps) {
			AppBasic basic = AppBasicCacheMgr.getInstance().get(appId);
			if (basic != null) {
				if (basic.isConnect()) {
					connectAppIds[connectOffset++] = appId;
				}
				if( basic.isWebCanvas() ){
					webCanvasApps[ webOffset ++ ] = appId;
				}
			}
		}
		int[] newConnects =  ArrayUtils.subarray(connectAppIds, 0, connectOffset);
		int[] newWebApps =  ArrayUtils.subarray(webCanvasApps, 0, webOffset);
		
		return new ClassifyApps(newConnects, newWebApps);
	}

	private class ClassifyApps {

		int[] connectApps;

		int[] webCavnasApps;

		ClassifyApps(int[] connectApps, int[] webCavnasApps) {
			super();
			this.connectApps = connectApps;
			this.webCavnasApps = webCavnasApps;
		}

	}

	public static void main(String arg[]) {
		int a[] = { 1, 4, 6, 7, 8, 9, 0, 9 };
		List<Integer> b = new ArrayList<Integer>();
		Collections.addAll(b, 1, 2, 3, 4);
		FrontEndMenuProcessor f = new FrontEndMenuProcessor();
		int c[] = f.clearUgcApp(a, b);
		for (int i = 0; i < c.length; i++) {
			System.out.println(c[i]);
		}
		System.out.println("---");
		int d[] = f.filterArray2(a, new int[] {});
		for (int t : d) {
			System.out.println(t);
		}
		int[] e = null;
		int[] v = ArrayUtils.addAll(e, null);
		System.out.println(v);

	}

}
