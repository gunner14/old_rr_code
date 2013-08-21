package com.xiaonei.platform.core.model;

import java.io.Serializable;

import com.xiaonei.platform.core.opt.OpiConstants;
import com.xiaonei.platform.storage.Allocator;
import com.xiaonei.platform.storage.Constants;

/**
 * 左边菜单项 TODO 还有些关于URl（static.xiaonei.com）的没有处理
 * 
 * @author fusong.li@opi-corp.com 2008-3-27 下午04:37:37
 */
public class AppMenuItem implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1847191281667661483L;
	/**
	 * 字段名 app_id
	 */
	public static final String FIELD_APP_ID = "app_id";
	/**
	 * 字段名 app_name
	 */
	public static final String FIELD_APP_NAME = "app_name";
	/**
	 * 字段名 app_icon
	 */
	public static final String FIELD_APP_ICON = "app_icon";
	/**
	 * 字段名 app_icon_24 专门用户开心网的左侧菜单
	 */
	public static final String FIELD_APP_ICON_24 = "app_icon_24";
	/**
	 * 字段名 side_nav_url
	 */
	public static final String FIELD_SIDE_NAV_URL = "side_nav_url";
    /**
     * 字段名 wap_side_nav_url
     */
    public static final String FIELD_WAP_SIDE_NAV_URL = "wap_side_nav_url";
	/**
	 * appId == -1 ，表示此app不在db中了，但是仍然把它放到cache中
	 */
	public static final int THE_APP_NOT_IN_DB = -1;
	/**
	 * appId, 如果appId == -1 ，表示此app不在db中了，但是仍然把它放到cache中
	 */
	private int appId;
	private String appName = "";
	private String appIcon = "";
	private String appIcon24 = "";
	private String sideNavUrl = "";
    private String wapSideNavUrl = "";
	private String urlTarget = null;
	public static final String DEFAULT_ICON_XN_SRC = "http://xnimg.cn/img/miss_app_small_icon_new.gif";
	public static final String DEFAULT_ICON_KX_SRC = "http://rrimg.com/img/miss_app_small_icon.gif";
	public static final String DEFAULT_ICON24_KX_SRC = "http://rrimg.com/img/miss_app_small_icon24.gif";
	public static final String DEFAULT_ICON24_XN_SRC = "http://xnimg.cn/img/miss_app_small_icon24.gif";
	
	public AppMenuItem() {
	}

	public AppMenuItem(int appId) {
		this.appId = appId;
	}

	public String getAppIcon() {
		 if (appIcon == null || appIcon.trim().length() == 0) {
			 if(appId>1000000){
					return DEFAULT_ICON_KX_SRC;
				}
				return DEFAULT_ICON_XN_SRC;
				}
		 else if (appIcon != null && !appIcon.startsWith("http://")) {
			return getAppPicUrl(appIcon);
		}else {
			return appIcon;
		}
	}

	public void setAppIcon(String appIcon) {
		this.appIcon = appIcon;
	}

	public String getAppIcon24() {
		if (appIcon24 == null || appIcon24.trim().length() == 0) {
			if(appId>1000000){
				return DEFAULT_ICON24_KX_SRC;
			}else{
				return DEFAULT_ICON24_XN_SRC;
			}
		}else if (appIcon24 != null && !appIcon24.startsWith(com.xiaonei.platform.core.opt.OpiConstants.urlStatic)) {
			return getAppPicUrl(appIcon24); 
		} else {
			return appIcon24;
		}
	}


	public void setAppIcon24(String appIcon24) {
		this.appIcon24 = appIcon24;
	}
	
	public int getAppId() {
		return appId;
	}

	public void setAppId(int appId) {
		this.appId = appId;
	}

	public String getAppName() {
		return appName;
	}

	public void setAppName(String appName) {
		this.appName = appName;
	}

	/**
	 * 为了保持兼容，函数名与实际意义有所差别了。
	 * bookmarkUrl的值在加载和更新AppMenuItem的时候被计算出来
	 * @return 返回cache过的Bookmark Url
	 */
	public String getSideNavUrl() {
		if (sideNavUrl != null) {
			//双域名自动替换
			if (OpiConstants.isXiaonei()) {
				return sideNavUrl.replace("kaixin.com", "renren.com");
			} else {
				return sideNavUrl.replace("renren.com", "kaixin.com");
			}
		} else {
			return sideNavUrl;
		}
	}

	public void setSideNavUrl(String sideNavUrl) {
		this.sideNavUrl = sideNavUrl;
	}
	
	/**
	 * @return 返回Bookmark URL的target属性，返回null表示默认target
	 * target属性通常用来表示当用户点击时是重新打开新页面还是在当前页面下载
	 * 使用菜单的各业务应该调用此函数
	 */
	public String getUrlTarget() {
		return urlTarget;
	}
	
	public void setUrlTarget(String urlTarget) {
		this.urlTarget = urlTarget;
	}
	
	@Override
	public String toString() {
		StringBuffer sb = new StringBuffer();
		sb.append("appId").append("=").append(appId).append("\n");
		sb.append("appName").append("=").append(appName).append("\n");
		sb.append("appIcon").append("=").append(appIcon).append("\n");
		sb.append("appIcon24").append("=").append(appIcon24).append("\n");
		sb.append("sideNavUrl").append("=").append(sideNavUrl).append("\n");
        sb.append("wapSideNavUrl").append("=").append(wapSideNavUrl).append("\n");

		return sb.toString();
	}

	/**
	 * 得到App的图片（icon、logo、main）图片的完整URL地址
	 * 
	 * @param pattern
	 * @return
	 */
	public static String getAppPicUrl(String pattern) {
		try {
			if (pattern == null || pattern.trim().length() == 0) {
				return "about:blank";
			}
			String url = Allocator.getInstance().locate(pattern, Constants.NODE_TYPE_NORMAL);
			if (url == null)
				url = com.xiaonei.platform.core.opt.OpiConstants.urlHead + "/photos/" + pattern;
			return url;
		} catch (Exception e) {
			e.printStackTrace(System.err);
			return "about:blank";
		}
	}

	@Override
	public boolean equals(Object obj) {
		if (obj == null || !(obj instanceof AppMenuItem)) {
			return false;
		}
		return this.appId == ((AppMenuItem) obj).getAppId();
	}

	@Override
	public int hashCode() {
		throw new RuntimeException("Illegal operation, hashCode() not implemented.");
	}
    
    /**
     * @return the wapSideNavUrl
     */
    public String getWapSideNavUrl() {
        if (wapSideNavUrl != null) {
            //双域名自动替换
            if (OpiConstants.isXiaonei()) {
                return wapSideNavUrl.replace("kaixin.com", "renren.com");
            } else {
                return wapSideNavUrl.replace("renren.com", "kaixin.com");
            }
        } else {
            return wapSideNavUrl;
        }
    }

    
    /**
     * @param wapSideNavUrl the wapSideNavUrl to set
     */
    public void setWapSideNavUrl(String wapSideNavUrl) {
        this.wapSideNavUrl = wapSideNavUrl;
    }
	
}
