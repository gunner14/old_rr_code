package com.xiaonei.reg.guide.view;

import com.xiaonei.platform.core.head.HeadUrlUtil;

import java.io.Serializable;

public class PendingListView implements Serializable {

    private static final long serialVersionUID = 4633947040586714992L;

    private int id;

    private int userId;

    private String name;

    private String headUrl;

    private String gender;

    private int selected;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public int getSelected() {
        return selected;
    }

    public void setSelected(int selected) {
        this.selected = selected;
    }

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getHeadUrl() {
        return HeadUrlUtil.getHeadFullUrl(headUrl);
    }

    public void setHeadUrl(String headUrl) {
        this.headUrl = headUrl;
    }
}
