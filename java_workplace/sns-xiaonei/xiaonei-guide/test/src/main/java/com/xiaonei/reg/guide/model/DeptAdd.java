package com.xiaonei.reg.guide.model;

import java.io.Serializable;

/**
 * DeptAdd.java
 *
 * @author drizzt drizzt.wang@gmail.com 2006-10-5 下午03:17:28
 */
public class DeptAdd implements Serializable {
    private static final long serialVersionUID = 4001252591245754735L;

    private int id;
    private int univ;
    private int user;
    private String dept;

    public DeptAdd(int univ, int usr, String dept) {
        this.univ = univ;
        this.user = usr;
        this.dept = dept;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getDept() {
        return dept;
    }

    public void setDept(String dept) {
        this.dept = dept;
    }

    public int getUniv() {
        return univ;
    }

    public void setUniv(int univ) {
        this.univ = univ;
    }

    public int getUser() {
        return user;
    }

    public void setUser(int user) {
        this.user = user;
    }
}