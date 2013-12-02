package com.renren.renrengame.utils.applist;

import java.util.ArrayList;

import org.apache.log4j.Logger;

import com.xiaonei.platform.component.application.model.AppBase;
import com.xiaonei.platform.component.application.utils.AppIdCacheManager;
import com.xiaonei.platform.core.menu.UserMenu;
import com.xiaonei.platform.core.menu.UserMenuManager;
import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.opt.base.AppMenuItemCacheMgr;

public class RecentGames {
	private final Logger log = Logger.getLogger(RecentGames.class.getName());
	
	public ArrayList<GameItemInfo> getRecentGame(int uid, 
			ArrayList<GameItemInfo> webGameInfoList) {
		ArrayList<GameItemInfo> recentGameInfoList = new ArrayList<GameItemInfo>();
		int[] appIds = getUserAppIds(uid);
		if (appIds!=null) {
			for (int i=0; i<appIds.length; ++i) {								
                if (appIsGame(appIds[i], webGameInfoList)) 
                	recentGameInfoList.add(getGameInfo(appIds[i]));                	                             
			}
		}			
		return recentGameInfoList;
	}
	
	private boolean appIsGame(int appId, ArrayList<GameItemInfo> webGameInfoList) {
		boolean isGame =false;
		try {
            isGame = AppIdCacheManager.getInstance().appTypeMatch(
          		  AppIdCacheManager.GAME, String.valueOf(appId));
          }catch(Exception e){ 
          	log.warn("RecentGame.GetRecentGame:  exception---> appid:"+appId);
          	return false;
          }
          if (!isGame) {
          	String strAppId = String.valueOf(appId);
          	for (int j=0; j<webGameInfoList.size(); ++j) {
          		if (strAppId.equals(webGameInfoList.get(j).ID)) {
          			isGame = true;
          			break;
          		}
          	}
          }
          return isGame;
	}
	private GameItemInfo getGameInfo(int appId) {
    	GameItemInfo gii = new GameItemInfo();
    	gii.ID = String.valueOf(appId);
    	
		AppMenuItem appMenuItem = AppMenuItemCacheMgr.getInstance().get(appId);
		if (appMenuItem != null ) {			
			gii.name = appMenuItem.getAppName();
			gii.logo = appMenuItem.getAppIcon();
			gii.url = appMenuItem.getSideNavUrl();
			gii.url += (gii.url.indexOf('?') == -1 ? "?" : "&")+50238;
			
			AppBase appbase = com.xiaonei.platform.component.application.cache.AppBaseCacheMgr
									.getInstance().get(appMenuItem.getAppId());
			if (appbase != null)
				gii.logo75 = appbase.getLogo();			
		}   	
		return gii;
	}
	
	private int[] getUserAppIds(int uid) {
		UserMenu userMenu = UserMenuManager.getInstance().get(uid);
		if (userMenu==null)
			return null;
		return userMenu.getRecentAppIds();	
	}

}
