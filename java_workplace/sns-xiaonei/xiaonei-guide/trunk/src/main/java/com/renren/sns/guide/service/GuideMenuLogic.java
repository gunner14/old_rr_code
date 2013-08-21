package com.renren.sns.guide.service;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import com.xiaonei.platform.component.application.cache.AppRecentMenuItemCacheMgr;
import com.xiaonei.platform.component.application.jade.home.RecommendedAppHome;
import com.xiaonei.platform.component.application.menu.model.RecommendedApp;
import com.xiaonei.platform.component.application.model.RecentMenuItem;
import com.xiaonei.platform.core.model.AppMenuItem;

public class GuideMenuLogic {

	private static final int MAX_RETURN_SIZE = 6;
	
	private static GuideMenuLogic instance = new GuideMenuLogic();
	
	private GuideMenuLogic() {};

	public static GuideMenuLogic getInstance() {
		return instance;
	}
	/**
	 * 获取应用动态  
	 * 
	 * @param userId
	 *            用户ID
	 * @param recent
	 *            用户最近使用的应用
	 * @param fav
	 *            用户收藏的应用
	 * @return 在推荐菜单里显示的应用，这里前台需要判断，如果items为空，则不显示该模块
	 */
	public List<RecentMenuItem> getMenuRecommendedApp(int userId,List<AppMenuItem> recent, List<AppMenuItem> fav) {
		try {
			List<RecentMenuItem> items = new ArrayList<RecentMenuItem>();

			List<Integer> filterIds = generFilterIds(recent, fav);
			List<Integer> recommendIds = getRecommendIds( filterIds);

			if(recommendIds.size() != 0) {
			for (int appId : recommendIds) {
				RecentMenuItem item = AppRecentMenuItemCacheMgr.getInstance().get(appId);
				if (item != null) {
					items.add(item);
				}
			}
			}
			return items;
		} catch (Exception e) {
			return Collections.emptyList();
		}
	}
	
	/**
	 * 合并左侧已出现在菜单里的应用
	 * @param recent 最近使用的应用
	 * @param fav 收藏的应用
	 * @return
	 */
	private List<Integer> generFilterIds(List<AppMenuItem> recent, List<AppMenuItem> fav){
		List<Integer> filterIds = new ArrayList<Integer>(50);
		for (AppMenuItem item : recent) {
			if (item != null){
				filterIds.add(item.getAppId());
			}
		}
		for (AppMenuItem item : fav) {
			if (item != null) {
				filterIds.add(item.getAppId());
			}
		}
		return filterIds;
	}
	
	/**
	 * 添加推荐应用ID，同时用于所有的配置应用均出现在菜单里时不出现该模块
	 * @param filterIds
	 * @return
	 */
	private List<Integer> getRecommendIds(List<Integer> filterIds){
		List<Integer> retAppIds = new ArrayList<Integer>();
		List<Integer> appIds = RecommendedAppHome.getInstance().getAppsByType(RecommendedApp.RECOMMENDED_APP);
		if (appIds != null ) {
			for (Integer id : appIds) {
				if (!filterIds.contains(id)) {
					retAppIds.add(id);
					if (retAppIds.size() == MAX_RETURN_SIZE) {
						break;
					}
				}
			}
		}
		return retAppIds;
	}

}
