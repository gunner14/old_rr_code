package com.xiaonei.reg.guide.view;

import com.xiaonei.platform.core.head.HeadUrlUtil;

import java.io.Serializable;

/**
 * UserTinyView.java
 *
 * @author drizzt drizzt.wang@gmail.com 2006-7-2 18:41:46
 */
public class UserTinyView implements Serializable {
    private static final long serialVersionUID = 7004288930369760212L;
    private int id;
    private String name;
    private String tinyUrl;
    private String hotfriend_desc;

    private int birth_day;
    private int birth_month;

    private String doing;


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