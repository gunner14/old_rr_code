package com.xiaonei.platform.core.menu;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import xce.menu.CacheType;

import com.xiaonei.platform.component.application.cache.AppBaseCacheMgr;
import com.xiaonei.platform.component.application.menu.cache.MenuCacheFacade;
import com.xiaonei.platform.component.application.menu.model.CachedMenuWrapper;
import com.xiaonei.platform.component.application.menu.model.MenuSort;
import com.xiaonei.platform.component.application.model.AppBase;
import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.opt.base.AppMenuItemCacheMgr;
import com.xiaonei.xce.menucache.MenuCacheReplicaAdapter;

/**
 * @author Li Weibo
 * @since 2009-6-10
 */
public class UserMenuManager {
	private Logger logger = LoggerFactory.getLogger(UserMenuManager.class);

	private static UserMenuManager instance = new UserMenuManager();
	
	private FrontEndMenuProcessor processor = new FrontEndMenuProcessor();
	
	public static UserMenuManager getInstance() {
		return instance;
	}
	
	private UserMenuManager() {}
	
	public UserMenu get(int userId) {
		UserMenu userMenu = getNonProcessed(userId);
		if (userMenu == null) {
			return null;
		}
		try {
			processor.process(userMenu);
			processor.processTopMenu(userMenu, userId);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return userMenu;
	}
	
	public UserMenu get(int userId, String ip) {
		UserMenu userMenu = getNonProcessed(userId);
		if (userMenu == null) {
			return null;
		}
		try {
			processor.processWithIP(userMenu, ip);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return userMenu;
	}
	
	
	/**
	 * 老接口为了减少影响，不获取ugc菜单和排序（默认为guide用户）
	 * @param userId
	 * @param ip
	 * @return
	 */
	@Deprecated
	public UserMenu getNewMenuList(int userId, String ip){
		return getNewMenuList(userId, ip, true);
	}
	/**
	 * 
	 * @param userId
	 * @param ip
	 * @param isGuide 是否为guide用户，做次兼容，guide用户用老接口
	 * @return
	 */
	public UserMenu getNewMenuList(int userId, String ip ,boolean isGuide) {
		UserMenu userMenu = getNonProcessed(userId , isGuide);
		if (userMenu == null) {
			return null;
		}
		
		try {
			processor.processWithIP(userMenu, ip);
			processor.processTopMenu(userMenu, userId);
			processor.processNoRepeat(userMenu);
			processor.processRecentApp(userMenu);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return userMenu;
	}
	
	
	/**
	 * 根据userId获取用户最近使用的两个app， for timeline项目
	 * @param userId
	 * @return
	 */
	public List<AppMenuItem> getRecentApp(int userId, int limit) {
		int[] recentAppIds = null;
		List<AppMenuItem> recent = new ArrayList<AppMenuItem>();
		try {
			Map<CacheType, int[]> map = MenuCacheReplicaAdapter.getInstance().get(userId, new CacheType[]{CacheType.RECENTLY});
			recentAppIds = map.get(CacheType.RECENTLY);
		} catch(Ice.TimeoutException e) {
			System.err.println("UserMenu timeout:" + userId);
		} catch (Throwable e) {
			e.printStackTrace();
		}
		if(recentAppIds == null) {
			return null;
		}
		for(int i = 0;i < recentAppIds.length; i++) {
			AppBase appBase = AppBaseCacheMgr.getInstance().get(recentAppIds[i]);
			if(appBase != null && !appBase.isXiaoneiApp()) {
				AppMenuItem appMenuItem = AppMenuItemCacheMgr.getInstance().get(recentAppIds[i]);
				if(appMenuItem != null) {
					recent.add(appMenuItem);
				}
			}
			if(recent.size() == limit) {
				break;
			}
		}
		return recent;
	}
	
	/**
	 * 返回原汁原味，没有个各种附加需求污染过、处理过的菜单信息
	 * 
	 * @param userId
	 * @return
	 */
	public UserMenu getNonProcessed(int userId){
		return getNonProcessed( userId , true);
	}
	
	/**
	 *  不为guide用户，且能获取到菜单值为新接口用户
	 * @param userId
	 * @param isGuide
	 * @return
	 */
	public UserMenu getNonProcessed(int userId ,boolean isGuide) {
		try {
			Map<CacheType, int[]> map = MenuCacheReplicaAdapter.getInstance()
				.get(userId, new CacheType[]{CacheType.RECENTLY, CacheType.FAVORITE });
			UserMenu userMenu = new UserMenu();
			userMenu.setRecentAppIds(map.get(CacheType.RECENTLY));
			userMenu.setFavoriteAppIds(map.get(CacheType.FAVORITE));
			
			if( isGuide ){
				userMenu.setUgcAppIds(new int[0]);
				userMenu.setMenuIds( MenuSort.DEFAULT_TYPE.getMenuIds() );
			}else{
				//通用cache缓存的，取一遍
				CachedMenuWrapper wrapper = MenuCacheFacade.getInstance().get(userId);
				if(wrapper != null){
    				userMenu.setUgcAppIds( wrapper.getUgcAppIds());
    				userMenu.setMenuIds( wrapper.getMenuIds() );
    				userMenu.setNewVersion(true);
    			}
			}
			return userMenu;
		} catch(Ice.TimeoutException e) {
			System.err.println("UserMenu timeout:" + userId);
		} catch (Throwable e) {
			e.printStackTrace();
		}
		return null;
	}
	
	/**
	 * 将当前app（根据appId指定）的信息注入request对象，来辅助webpager菜单等
	 * @param appId
	 * @param request
	 */
	public void setCurrentApp(int appId, HttpServletRequest request) {
		AppMenuItem appMenu = AppMenuItemCacheMgr.getInstance().get(appId);
		request.setAttribute("currentApp", appMenu);
	}
	
	public static void main(String[] args) {
		int a=1;int b=10; 
		do { b-=a; a++; } while (b--<0);
		System.out.println(b);
	}
}
