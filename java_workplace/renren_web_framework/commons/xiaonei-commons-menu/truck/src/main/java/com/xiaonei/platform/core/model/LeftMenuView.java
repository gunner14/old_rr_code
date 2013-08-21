package com.xiaonei.platform.core.model;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

/**
 * 用户左边菜单显示View，包含显示的菜单列表、更多的菜单列表
 * 
 * @author fusong.li@opi-corp.com 2008-3-27 下午05:37:22
 * 
 * @deprecated 旧版菜单相关，已经不再使用了
 */
public class LeftMenuView implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = -2369774540598921561L;

	private List<AppMenuItem> displayMenus;

	private List<AppMenuItem> moreMenus;
	
	private List<AppMenuItem> recentMenus;
	
	/**
	 * 用户最后一次使用的app的菜单
	 */
	private AppMenuItem latestMenu;
	
	public AppMenuItem getLatestMenu() {
		return latestMenu;
	}

	public void setLatestMenu(AppMenuItem latestMenu) {
		this.latestMenu = latestMenu;
	}

	//lazy init
	private List<AppMenuItem> allMenus = null;
	
	private List<AppMenuItem> restMenus = null;
	
	
	public List<AppMenuItem> getAllMenus() {
		if (allMenus == null) {
			allMenus = new ArrayList<AppMenuItem>();
			if (displayMenus != null) {
				for (AppMenuItem item : displayMenus) {
					allMenus.add(item);
				}
			}
			if (moreMenus != null) {
				for (AppMenuItem item : moreMenus) {
					allMenus.add(item);
				}
			}
		}
		return this.allMenus;
	}

	public List<AppMenuItem> getRestMenus() {
		//return getAllMenus();
		try {
			if (restMenus == null) {
				List<AppMenuItem> allMenuList = getAllMenus();
				//printMenus(0, "allMenuList" , allMenuList);
				//printMenus(0, "recentMenus" , recentMenus);
				//System.out.println("allMenuList.size()=" + allMenuList.size());
				//System.out.println("recentMenus.size()=" + recentMenus.size());
				restMenus = new ArrayList<AppMenuItem>(10);
				
				//从allMenuList中剔除掉最近使用的app，由于最近使用的app列表不大，所以循环找没有效率问题
				for (AppMenuItem item : allMenuList) {
					boolean found = false;
					for (AppMenuItem ri : recentMenus) {
						if (item.getAppId() == ri.getAppId()) {
							found = true;
							break;
						}
					}
					if (!found) {
						restMenus.add(item);
					}
				}
				
				//复制一份，然后排序
				/*List<AppMenuItem> tmpRecentMenus = new ArrayList<AppMenuItem>(recentMenus.size());
				for (AppMenuItem item : recentMenus) {
					tmpRecentMenus.add(item);
				}
				Collections.sort(tmpRecentMenus, new Comparator<AppMenuItem>() {
						public int compare(AppMenuItem o1, AppMenuItem o2) {
							return o1.getAppId() - o2.getAppId();
						}});
				
				for (AppMenuItem item : allMenuList) {
					if (Collections.binarySearch(tmpRecentMenus, item, new Comparator<AppMenuItem>() {
						public int compare(AppMenuItem o1, AppMenuItem o2) {
							return o1.getAppId() - o2.getAppId();
						}}) < 0) {
						restMenus.add(item);
						//System.out.println(item);
					}
				}*/
				//System.out.println("restMenus.size()=" + restMenus.size());
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		//printMenus(0, "restMenus" , restMenus);
		return restMenus;
	}

	/*private void printMenus(int uid, String title,List<AppMenuItem> menu) {
		System.out.println("llllllllllllllllllllll" + uid + "START " + title);
		for (AppMenuItem item : menu) {
			System.out.println("llllllllllllllllllllll" + uid + " " + item.getAppId() + "-" + item.getAppName());
		}
		System.out.println("llllllllllllllllllllll" + uid + "END " + title);
	} */
	
	public List<AppMenuItem> getRecentMenus() {
		return recentMenus;
	}

	public void setRecentMenus(List<AppMenuItem> recentMenus) {
		this.recentMenus = recentMenus;
	}

	//private int appCount ;
	
	/**
	 * 这个数不准确
	 * @return
	 */
	public int getAppCount() {
		int count = 0;
		if(displayMenus != null){
			count = displayMenus.size();
		}
		if(moreMenus != null){
			count += moreMenus.size();
		}
		return count;
	}

	/*public void setAppCount(int appCount) {
		this.appCount = appCount;
	}*/

	public List<AppMenuItem> getDisplayMenus() {
		return displayMenus;
	}

	public List<AppMenuItem> getMoreMenus() {
		return moreMenus;
	}

	public void setDisplayMenus(List<AppMenuItem> displayMenus) {
		this.displayMenus = displayMenus;
	}

	public void setMoreMenus(List<AppMenuItem> moreMenus) {
		this.moreMenus = moreMenus;
	}

	public void setRestMenus(List<AppMenuItem> restMenus) {
		this.restMenus = restMenus;
	}

	
	
	/*public void setAllMenus(List<AppMenuItem> allMenus) {
		this.allMenus = allMenus;
	}*/
	
}
