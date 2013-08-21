package com.renren.sns.guide.model;

import java.io.Serializable;
import java.util.Date;


/**
 * @author wei.xiang
 * 
 */
public class AppHomeView implements Serializable {

    /**
     * 
     */
    private static final long serialVersionUID = 7478481904781815701L;

    public String getAppLogo() {
        return appLogo;
    }

    public void setAppLogo(String appLogo) {
        this.appLogo = appLogo;
    }

    public String getAppName() {
        return appName;
    }

    public void setAppName(String appName) {
        this.appName = appName;
    }

    public String getAppUrl() {
        return appUrl;
    }

    public void setAppUrl(String appUrl) {
        this.appUrl = appUrl;
    }

    public int getAppTotalCount() {
        return appTotalCount;
    }

    public void setAppTotalCount(int appTotalCount) {
        this.appTotalCount = appTotalCount;
    }

    public int getAppFriendCount() {
        return appFriendCount;
    }

    public void setAppFriendCount(int appFriendCount) {
        this.appFriendCount = appFriendCount;
    }

    public boolean isHotApp(){
        return this.isHotApp;
    }
    
    public void setIsHotApp(boolean isHotApp){
        this.isHotApp = isHotApp;
    }
    
    public void setAppId(int appId) {
        this.appId = appId;
    }

    public int getAppId() {
        return appId;
    }

    public void setAppDailyActive(int appDailyActive) {
        this.appDailyActive = appDailyActive;
    }

    public int getAppDailyActive() {
        return appDailyActive;
    }

    public void setLastVisitTime(Date lastVisitTime) {
        this.lastVisitTime = lastVisitTime;
    }

    public Date getLastVisitTime() {
        return lastVisitTime;
    }

    public void setLastAppTimeString(String lastAppTimeString) {
        this.lastAppTimeString = lastAppTimeString;
    }

    public String getLastAppTimeString() {
        return lastAppTimeString;
    }

    public void setAppIcon(String appIcon) {
        this.appIcon = appIcon;
    }

    public String getAppIcon() {
        return appIcon;
    }

    private String appIcon;
    private String appLogo;
    private String appName;
    private String appUrl;
    private Date lastVisitTime;
    private String lastAppTimeString;
    private int appTotalCount;
    private int appDailyActive;
    private int appFriendCount;
    private boolean isHotApp = false;
    private int appId;
}
