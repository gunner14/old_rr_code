package com.renren.sns.guide.view;

import java.io.Serializable;

import com.xiaonei.platform.core.head.HeadUrlUtil;

public class UserTinyView implements Serializable{

    private static final long serialVersionUID = 7004288930369760273L;
    
    private int id;

    private String name;

    private String tinyUrl;

    @SuppressWarnings("unused")
    private String Tiny_Url;

    private String hotfriend_desc;

    private int birth_day;

    private int birth_month;

    private String doing;

    // 是否在线: : 个人主页使用,后续设置，数据库中没有
    private boolean online;

    // 是否紫豆用户: : 个人主页使用,后续设置，数据库中没有
    private boolean vipMember;

    /**
     * <ul>
     * <li>个人主页使用,后续设置，数据库中没有</li>
     * </ul>
     * 
     * @return
     */
    public boolean isOnline() {
        return online;
    }

    /**
     * <ul>
     * <li>个人主页使用,后续设置，数据库中没有</li>
     * </ul>
     * 
     * @param online
     */
    public void setOnline(boolean online) {
        this.online = online;
    }

    /**
     * 
     * <ul>
     * <li>个人主页使用,后续设置，数据库中没有</li>
     * </ul>
     * 
     * @return
     */
    public boolean isVipMember() {
        return vipMember;
    }

    /**
     * <ul>
     * <li>个人主页使用,后续设置，数据库中没有</li>
     * </ul>
     * 
     * @param vipMember
     */
    public void setVipMember(boolean vipMember) {
        this.vipMember = vipMember;
    }

    public String getHotfriend_desc() {
        return hotfriend_desc;
    }

    public void setHotfriend_desc(String hotfriend_desc) {
        this.hotfriend_desc = hotfriend_desc;
    }

    public String getDoing() {
        return doing;
    }

    public void setDoing(String doing) {
        this.doing = doing;
    }

    public int getBirth_month() {
        return birth_month;
    }

    public void setBirth_month(int birth_month) {
        this.birth_month = birth_month;
    }

    public int getBirth_day() {
        return birth_day;
    }

    public void setBirth_day(int birth_day) {
        this.birth_day = birth_day;
    }

    public String getTiny_Url() {
        return HeadUrlUtil.getHeadFullUrl(tinyUrl);
    }

    public void setTiny_Url(String tiny_Url) {
        Tiny_Url = tiny_Url;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getTinyUrl() {
        return tinyUrl;
    }

    public void setTinyUrl(String tinyUrl) {
        this.tinyUrl = tinyUrl;
    }
}
