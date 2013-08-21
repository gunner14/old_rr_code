package com.xiaonei.platform.core.opt.jdbc.home;

import java.util.List;

import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.opt.base.AppMenuItemCacheMgr;
import com.xiaonei.platform.core.opt.jdbc.dao.AppMenuItemDAO;

/**
 * 
 * @author fusong.li@opi-corp.com 2008-3-27 下午04:33:25
 */
public class AppMenuItemHome {

	private static AppMenuItemHome instance = new AppMenuItemHome();

	public static AppMenuItemHome getInstance() {
		return instance;
	}

	private AppMenuItemHome() {
		super();
	}
	/**
	 * 根据appId得到App的MenuItem
	 * @param appId
	 * @return
	 */
	public AppMenuItem get(int appId){
		try {
			return AppMenuItemDAO.getInstance().get(appId);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return null;
	}
	
	/**
	 * 根据appId从主库读取appmenuitem对象，大访问量需求慎用此方法
	 * @param appId
	 * @return
	 */
	public AppMenuItem getFromMaster(int appId){
		try {
			return AppMenuItemDAO.getInstance().getFromMaster(appId);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return null;
	}
	/**
	 * 
	 * @param appIds 逗号分隔的appId串
	 * @return
	 */
	public List<AppMenuItem> gets(String appIds){
		try {
			if(appIds!=null && appIds.trim().length()>0){
				return AppMenuItemDAO.getInstance().gets(appIds);
			}
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return null;
	}
	/**
	 * 更新app_menu_item表的app_icon ，同时更新缓存
	 * @param appIcon
	 * @param appIcon24  24X24尺寸的icon，如果没有请传入空串“”
	 * @param appId
	 */
	public void updateIcon(String appIcon,String appIcon24,int appId){
		appIcon = (appIcon == null?"":appIcon);
		appIcon24 = (appIcon24 == null?"":appIcon24);
		try {
			//更新DB
			AppMenuItemDAO.getInstance().updateIcon(appIcon,appIcon24, appId);
			//通知菜单同步listener
			notifyListener(appId);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
	}
	/**
	 * 更新app_menu_item表的app_icon ，同时更新缓存
	 * @param appIcon 16 尺寸
	 * @param appId
	 */
	public void updateIcon(String appIcon,int appId){
		appIcon = (appIcon == null?"":appIcon);
		try {
			//更新DB
			AppMenuItemDAO.getInstance().updateIcon(appIcon, appId);
			//通知菜单同步listener
			notifyListener(appId);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
	}
	/**
	 * 更新app_menu_item表的app_icon_24 ，同时更新缓存
	 * @param appIcon24  24X24尺寸的icon，如果没有请传入空串“”
	 * @param appId
	 */
	public void updateIcon24(String appIcon24,int appId){
		appIcon24 = (appIcon24 == null?"":appIcon24);
		try {
			//更新DB
			AppMenuItemDAO.getInstance().updateIcon24(appIcon24, appId);
			//通知菜单同步listener
			notifyListener(appId);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
	}
	private void notifyListener(int appId){
		
		AppMenuItemCacheMgr.getInstance().notifiyToUpdate(appId);
		/*try {
			//通知菜单同步listener
			Map<String, String> map = new HashMap<String, String>();
			map.put(AppMenuItem.FIELD_APP_ID,String.valueOf(appId));
			SvcDescriptorAdapter.getInstance().notify("com.xiaonei.platform.core.opt.base.AppMenuItemDescriptorListener", map);
		} catch (Exception e) {
			// TODO: handle exception
		}*/
	}
	
	/**
	 * 
	 * @param appName
	 * @param appIcon
	 * @param appIcon24 24X24尺寸的icon，如果没有请传入空串“”
	 * @param sideNavUrl
	 * @param appId
	 * @return
	 */
	public int update(String appName,String appIcon, String appIcon24, String sideNavUrl ,int appId){
		appIcon = (appIcon == null?"":appIcon);
		appIcon24 = (appIcon24 == null?"":appIcon24);
		try {
			int id = AppMenuItemDAO.getInstance().update(appName, appIcon, appIcon24, sideNavUrl, appId);
			//通知菜单同步listener
			notifyListener(appId);
			return id;
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return 0;
	}
	/**
	 * 
	 * @param appName
	 * @param sideNavUrl
	 * @param appId
	 * @return
	 */
	public int update(String appName, String sideNavUrl ,int appId){
		try {
			int id = AppMenuItemDAO.getInstance().update(appName, sideNavUrl, appId);
			//通知菜单同步listener
			notifyListener(appId);
			return id;
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return 0;
	}
	/**
	 * 
	 * @param appName
	 * @param appId
	 * @return
	 */
	public int update(String appName,int appId){
		try {
			int id = AppMenuItemDAO.getInstance().update(appName, appId);
			//通知菜单同步listener
			notifyListener(appId);
			return id;
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return 0;
	}
	
	
	/**
	 * 
	 * @return
	 */
	public List<AppMenuItem> getAll(){
		try {
			return AppMenuItemDAO.getInstance().getAll();
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return null;
	}


	/**
	 * 增加一条记录
	 * @param menuItem
	 * @return
	 */
	public int save(AppMenuItem menuItem){
		try {
			return AppMenuItemDAO.getInstance().save(menuItem);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return 0;
	}
	

	/**
	 * 删除一条记录
	 * @param appId
	 * @return
	 */
	public int delete(int appId){
		try {
			return AppMenuItemDAO.getInstance().delete(appId);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
		return 0;
	}
	/**
	 * 更新SideNavUrl
	 * @param sideNavUrl
	 * @param appId
	 */
	public void updateSideNavUrl(String sideNavUrl, int appId) {
        sideNavUrl = (sideNavUrl == null ? "" : sideNavUrl);
        try {
            //
            AppMenuItemDAO.getInstance().updateSideNavUrl(sideNavUrl, appId);
            //
            notifyListener(appId);
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
    }

    /**
     * 更新WapSideNavUrl
     * @param wapSideNavUrl
     * @param appId
     */
    public void updateWapSideNavUrl(String wapSideNavUrl, int appId) {
        wapSideNavUrl = (wapSideNavUrl == null ? "" : wapSideNavUrl);
        try {
            //
            AppMenuItemDAO.getInstance().updateWapSideNavUrl(wapSideNavUrl, appId);
            //
            notifyListener(appId);
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
    }
    /**
     * 同时更新 web 和 wap 的应用访问URL地址
     * @param sideNavUrl
     * @param wapSideNavUrl
     * @param appId
     */
    public void updateWebWapSideNavUrl(String sideNavUrl ,String wapSideNavUrl, int appId) {
        sideNavUrl = (sideNavUrl == null ? "" : sideNavUrl);
        wapSideNavUrl = (wapSideNavUrl == null ? "" : wapSideNavUrl);
        try {
            //
            AppMenuItemDAO.getInstance().updateWebWapSideNavUrl(sideNavUrl, wapSideNavUrl, appId);
            //
            notifyListener(appId);
        } catch (Exception e) {
            e.printStackTrace(System.err);
        }
    }
}
