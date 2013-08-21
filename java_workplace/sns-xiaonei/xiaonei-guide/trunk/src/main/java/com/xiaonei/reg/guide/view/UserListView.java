package com.xiaonei.reg.guide.view;

import com.xiaonei.platform.core.head.HeadUrlUtil;
import com.xiaonei.platform.core.opt.network.NetworkManager;

import java.io.Serializable;
import java.util.Date;

/**
 * UserListView.java@author fdp
 */
public class UserListView implements Serializable {
    private static final long serialVersionUID = 4311643720730540839L;
    private int id;
    private String name;
    private String password;
    private int auth;
    private String email;
    private int entry;
    private int selected;
    private int star;
    private int univId;
    private int status;
    private Integer univYear;
    private String department;
    private String dorm;
    private String gender;
    private String headUrl;
    private String tinyUrl;
    private Date lastLoginTime;
    private String doing;
    private Date doingTime;
    private String highschool;
    private int stage;

    private String hotfriend_desc;

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

    public Date getDoingTime() {
        return doingTime;
    }

    public void setDoingTime(Date doingTime) {
        this.doingTime = doingTime;
    }

    public int getAuth() {
        return auth;
    }

    public void setAuth(int auth) {
        this.auth = auth;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getHead_Url() {
        return HeadUrlUtil.getHeadFullUrl(headUrl);
    }

    public String getTiny_Url() {
        return HeadUrlUtil.getHeadFullUrl(tinyUrl);
    }

    public String getDepartment() {
        return department;
    }

    public void setDepartment(String department) {
        this.department = department;
    }

    public String getDorm() {
        return dorm;
    }

    public void setDorm(String dorm) {
        this.dorm = dorm;
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public String getHeadUrl() {
        return headUrl;
    }

    public void setHeadUrl(String headUrl) {
        this.headUrl = headUrl;
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

    public int getSelected() {
        return selected;
    }

    public void setSelected(int selected) {
        this.selected = selected;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public int getUnivId() {
        return univId;
    }

    public void setUnivId(int univId) {
        this.univId = univId;

    }

    public String getUnivName() {
        return NetworkManager.getInstance().getNetById(this.univId).getName();
    }

    public Integer getUnivYear() {
        return univYear;
    }

    public void setUnivYear(Integer univYear) {
        this.univYear = univYear;
    }

    public int getStar() {
        return star;
    }

    public void setStar(int star) {
        this.star = star;
    }

    public String getTinyUrl() {
        return tinyUrl;
    }

    public void setTinyUrl(String tinyUrl) {
        this.tinyUrl = tinyUrl;
    }

    public Date getLastLoginTime() {
        return lastLoginTime;
    }

    public void setLastLoginTime(Date lastLoginTime) {
        this.lastLoginTime = lastLoginTime;
    }

    public int getEntry() {
        return entry;
    }

    public void setEntry(int entry) {
        this.entry = entry;
    }

    public String getHighschool() {
        return highschool;
    }

    public void setHighschool(String highschool) {
        this.highschool = highschool;
    }

    public int getStage() {
        return stage;
    }

    public void setStage(int stage) {
        this.stage = stage;
    }

}