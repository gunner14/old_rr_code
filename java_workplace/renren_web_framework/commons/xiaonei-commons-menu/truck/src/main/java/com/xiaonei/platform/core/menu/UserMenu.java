package com.xiaonei.platform.core.menu;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang.ArrayUtils;

import com.xiaonei.platform.core.model.AppMenuItem;
import com.xiaonei.platform.core.opt.base.AppMenuItemCacheMgr;

/**
 * @author Li Weibo
 * @since 2009-6-10
 */
public class UserMenu implements Serializable {

	private static final long serialVersionUID = 1L;

	private int userId;

	private int[] recentAppIds;

	private int[] favoriteAppIds;

	private int[] ugcAppIds;

	private int[] connectAppIds;

	private int[] menuIds;

	private int[] topAppIds;

	private int[] secondTopAppIds;

	private int[] thirdTopAppIds;

	private int[] fourthTopAppIds;
	
	private int[] allTopIds;

	private List<AppMenuItem> recentMenu;

	private List<AppMenuItem> ugcMenu;

	private List<AppMenuItem> favoriteMenu;

	private List<AppMenuItem> connectMenu;

	private List<AppMenuItem> topMenu;

	private List<AppMenuItem> secondTopMenu;

	private List<AppMenuItem> thirdTopMenu;

	private List<AppMenuItem> fourthTopMenu;

	private boolean isNewVersion = false;

	/**
	 * 取用户最近使用的菜单
	 * 
	 * @return
	 */
	public List<AppMenuItem> getRecentMenu() {
		if (recentMenu == null) {
			recentMenu = renderMenu(recentAppIds);
		}
		return recentMenu;
	}

	/**
	 * 取用户最近使用的菜单
	 * 
	 * @return
	 */
	public List<AppMenuItem> getConnectMenu() {
		if (connectMenu == null) {
			connectMenu = renderMenu(connectAppIds);
		}
		return connectMenu;
	}

	/**
	 * 取用户UGC应用
	 */
	public List<AppMenuItem> getUgcMenu() {
		if (ugcMenu == null) {
			ugcMenu = renderMenu(ugcAppIds);
		}
		return ugcMenu;
	}

	/**
	 * 取菜单的顺序串
	 */
	public String getMenuIdStr() {
		if (menuIds == null || menuIds.length == 0) {
			return "";
		}

		StringBuilder sb = new StringBuilder(menuIds.length * 10);
		for (int menuId : menuIds) {
			sb.append(menuId).append(",");
		}
		sb.deleteCharAt(sb.length() - 1);
		return sb.toString();
	}

	/**
	 * 是否ugc菜单在上
	 */
	public boolean isUgcMenuTop() {
		return false;
	}

	/**
	 * 是否我的应用在上
	 */
	public boolean isRecentMenuTop() {
		return true;
	}

	/**
	 * 取用户收藏的菜单
	 * 
	 * @return
	 */
	public List<AppMenuItem> getFavoriteMenu() {
		if (favoriteMenu == null) {
			favoriteMenu = renderMenu(favoriteAppIds);
		}
		return favoriteMenu;
	}

	/**
	 * 获取左侧菜单中置顶的app列表
	 * 
	 * @return
	 */
	public List<AppMenuItem> getTopMenu() {
		if (topMenu == null) {
			topMenu = renderMenu(topAppIds);
		}
		return topMenu;
	}

	/**
	 * 获取左侧菜单中置顶的app列表 } }
	 * 
	 * @return
	 */
	public List<AppMenuItem> getSecondTopMenu() {
		if (secondTopMenu == null) {
			secondTopMenu = renderMenu(secondTopAppIds);
		}
		return secondTopMenu;
	}

	public List<AppMenuItem> getThirdTopMenu() {
		if (thirdTopMenu == null) {
			thirdTopMenu = renderMenu(thirdTopAppIds);
		}
		return thirdTopMenu;
	}

	public List<AppMenuItem> getFourthTopMenu() {
		if (fourthTopMenu == null) {
			fourthTopMenu = renderMenu(fourthTopAppIds);
		}
		return fourthTopMenu;
	}
	
	
	public int[] getAllTopAppIds(){
		if( allTopIds == null){
			allTopIds = ArrayUtils.addAll( null, topAppIds );
			allTopIds = ArrayUtils.addAll( allTopIds, secondTopAppIds );
			allTopIds = ArrayUtils.addAll( allTopIds, thirdTopAppIds );
			allTopIds = ArrayUtils.addAll( allTopIds, fourthTopAppIds );
		}
		return allTopIds;
	}

	/*private void initRecentMenu() {
		if (recentAppIds == null) {
			recentMenu = new ArrayList<AppMenuItem>(0);
		} else {
			recentMenu = new ArrayList<AppMenuItem>(recentAppIds.length);
			for (int appId : recentAppIds) {
				try {
					AppMenuItem item = AppMenuItemCacheMgr.getInstance().get(appId);
					if (item != null) {
						recentMenu.add(item);
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}

	private void initFavoriteMenu() {
		if (favoriteAppIds == null) {
			favoriteMenu = new ArrayList<AppMenuItem>(0);
		} else {
			favoriteMenu = new ArrayList<AppMenuItem>(favoriteAppIds.length);
			for (int appId : favoriteAppIds) {
				try {
					AppMenuItem item = AppMenuItemCacheMgr.getInstance().get(appId);
					if (item != null) {
						favoriteMenu.add(item);
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}*/

	private List<AppMenuItem> renderMenu(int[] appIds) {
		List<AppMenuItem> ret;
		if (appIds == null) {
			ret = new ArrayList<AppMenuItem>(0);
		} else {
			ret = new ArrayList<AppMenuItem>(appIds.length);
			for (int appId : appIds) {
				try {
					AppMenuItem item = AppMenuItemCacheMgr.getInstance().get(appId);
					if (item != null) {
						ret.add(item);
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		return ret;
	}

	public void setUserId(int userId) {
		this.userId = userId;
	}

	public int getUserId() {
		return userId;
	}

	public int[] getRecentAppIds() {
		return recentAppIds;
	}

	void setRecentAppIds(int[] recentAppIds) {
		this.recentAppIds = recentAppIds;
	}

	public int[] getFavoriteAppIds() {
		return favoriteAppIds;
	}

	void setFavoriteAppIds(int[] favoriteAppIds) {
		this.favoriteAppIds = favoriteAppIds;
	}

	public int[] getTopAppIds() {
		return topAppIds;
	}

	public void setTopAppIds(int[] topAppIds) {
		this.topAppIds = topAppIds;
	}

	public int[] getSecondTopAppIds() {
		return secondTopAppIds;
	}

	public void setSecondTopAppIds(int[] secondTopAppIds) {
		this.secondTopAppIds = secondTopAppIds;
	}

	public int[] getThirdTopAppIds() {
		return thirdTopAppIds;
	}

	public void setThirdTopAppIds(int[] thirdTopAppIds) {
		this.thirdTopAppIds = thirdTopAppIds;
	}

	public int[] getFourthTopAppIds() {
		return fourthTopAppIds;
	}

	public void setFourthTopAppIds(int[] fourthTopAppIds) {
		this.fourthTopAppIds = fourthTopAppIds;
	}

	public int[] getUgcAppIds() {
		return ugcAppIds;
	}

	public void setUgcAppIds(int[] ugcAppIds) {
		this.ugcAppIds = ugcAppIds;
	}

	public int[] getMenuIds() {
		return menuIds;
	}

	public void setMenuIds(int[] menuIds) {
		this.menuIds = menuIds;
	}

	public int[] getConnectAppIds() {
		return connectAppIds;
	}

	public void setConnectAppIds(int[] connectAppIds) {
		this.connectAppIds = connectAppIds;
	}

	public boolean isNewVersion() {
		return isNewVersion;
	}

	public void setNewVersion(boolean isNewVersion) {
		this.isNewVersion = isNewVersion;
	}

	public String toString() {
		String NEWLINE = "\n";
		StringBuilder sb = new StringBuilder();
		if (ugcAppIds != null) 
			sb.append("ugcAppIds size  is ").append(ugcAppIds.length).append(NEWLINE);
		if (connectAppIds != null) 
			sb.append("connectIds size  is ").append(connectAppIds.length).append(NEWLINE);
		if (favoriteAppIds != null) 
			sb.append("favoriteAppIds size is ")
				.append(favoriteAppIds.length).append(NEWLINE);
		if (recentAppIds != null) 
			sb.append("recentAppIds size is  ").append(recentAppIds.length).append(NEWLINE);
		return sb.toString();
	}

}
